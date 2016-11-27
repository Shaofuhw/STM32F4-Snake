/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 *
 * COPYRIGHT(c) 2016 STMicroelectronics
 *
 * Redistribution and use in source and binaFrutasY forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binaFrutasY form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLAFrutasY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEOFrutasY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "usbd_cdc_if.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery_gyroscope.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"

/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;

/* USER CODE BEGIN Variables */
#define ABS(x)         (x < 0) ? (-x) : x
#define NFrutas 3
#define LongitudInicial 10
uint16_t Xpos[20] = {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
uint16_t Ypos[20] = {50, 50, 50, 50, 50 ,50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50};
uint8_t State = 0;
uint8_t Longitud = LongitudInicial;
uint8_t FrutaON = 0;
uint8_t FrutasX[NFrutas], FrutasY[NFrutas];
uint8_t GOver = 0;
TS_StateTypeDef TouchScreen;

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
//TASKS
void GyroTask(void const * argument);
void LCDTask();
void GameTask();

//FUNCIONES
void ComprobarGyro(float, float, float*);
void PrintLCD(uint16_t XposTemp, uint16_t YposTemp);
void ActualizarPosicion();
void PararSnake();
void CrearFruta();
void ShiftArray();
void ComprobarFruta();
void ComprobarCuerpo();
void PrintGameOver();

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */
	srand(time(NULL));
	/* USER CODE END Init */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* Create the thread(s) */
	/* definition and creation of defaultTask */
	osThreadDef(GyroTask, GyroTask, 2, 1, 128);
	defaultTaskHandle = osThreadCreate(osThread(GyroTask), NULL);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	xTaskCreate( LCDTask, "LCDTask", 128, NULL, 1, NULL );
	xTaskCreate( GameTask, "GameTask", 128, NULL, 2, NULL );
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */

//Lee el valor del giroscopio, y establece una máquina de estados según hacia donde sea la lectura
void GyroTask(void const * argument)
{
	/* USER CODE BEGIN StartDefaultTask */
	float Buffer[3] = {0};
	float Xval, Yval = 0;
	BSP_GYRO_Reset();
	/* Infinite loop */
	while (1)
	{
		/* Read Gyro Angular data */
		BSP_GYRO_GetXYZ(Buffer);

		/* Update autoreload and capture compare registers value */
		Xval = ABS((Buffer[0]));
		Yval = ABS((Buffer[1]));

		ComprobarGyro(Xval, Yval, Buffer);
		osDelay(100);
	}
	/* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Application */

//Actualiza el LCD con la posición del SNAKE
void LCDTask(){

	uint16_t XposTemp = 0;
	uint16_t YposTemp = 0;

	/* Clear the LCD */
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_Clear(LCD_COLOR_WHITE);

	for(;;){
		PrintLCD(XposTemp, YposTemp);
		osDelay(75);
	}
}

//Se encarga de actualizar la posición del Snake según el estado proporcionado por el GYRO
void GameTask(){
	for(;;){
		ActualizarPosicion();
		CrearFruta();
		PararSnake();
		ComprobarFruta();
		ComprobarCuerpo();
		osDelay(75);
	}
}

void ComprobarGyro(float Xval, float Yval, float* Buffer){
	/*
		X1 = Arriba
		X2 = Abajo
		Y1 = Izquierda
		Y2 = Derecha
	 */
	if(Xval>Yval){
		if(Buffer[0] > 20000.0f){
			//X1
			//Evitar que pueda girar 180º
			if(Ypos[0] + 10 != Ypos[1]){
				State = 1;
			}
		}else if(Buffer[0] < -20000.0f){
			//X2
			if(Ypos[0] - 10 != Ypos[1]){
				State = 3;
			}
		}
	}else if (Yval>Xval){
		if(Buffer[1] < -20000.0f){
			//Y1
			if(Xpos[0] - 10 != Xpos[1]){
				State = 4;
			}
		}else if(Buffer[1] > 20000.0f){
			//Y2
			if(Xpos[0] + 10 != Xpos[1]){
				State = 2;
			}
		}
	}
}

void PrintLCD(uint16_t XposTemp, uint16_t YposTemp){
	//La pantalla se actualiza unicamente si ha cambiado la posición del Snake
	if((YposTemp != Ypos[0] || XposTemp != Xpos[0]) && State != 0){
		XposTemp = Xpos[0];
		YposTemp = Ypos[0];
		BSP_LCD_Clear(LCD_COLOR_WHITE);
		//Pintar Cuerpo
		for(uint8_t i = 0; i < Longitud; i++){
			BSP_LCD_DrawCircle(Xpos[i], Ypos[i], 5);
		}
		//Pintar Ojos
		if(State == 1){
			BSP_LCD_DrawCircle(Xpos[0]+5, Ypos[0]+5, 2);
			BSP_LCD_DrawCircle(Xpos[0]-5, Ypos[0]+5, 2);
		}else if(State == 3){
			BSP_LCD_DrawCircle(Xpos[0]+5, Ypos[0]-5, 2);
			BSP_LCD_DrawCircle(Xpos[0]-5, Ypos[0]-5, 2);
		}else if(State == 2){
			BSP_LCD_DrawCircle(Xpos[0]+5, Ypos[0]+5, 2);
			BSP_LCD_DrawCircle(Xpos[0]+5, Ypos[0]-5, 2);
		}else if(State == 4){
			BSP_LCD_DrawCircle(Xpos[0]-5, Ypos[0]+5, 2);
			BSP_LCD_DrawCircle(Xpos[0]-5, Ypos[0]-5, 2);
		}
		//Pintar Fruta
		if(FrutaON == 1){
			for(uint8_t i = 0; i < NFrutas; i++){
				BSP_LCD_FillCircle(FrutasX[i], FrutasY[i], 4);
			}
		}
		//GAME OVER
		if(GOver == 1){
			vTaskSuspendAll();
			State = 0;
			TouchScreen.X = 0;
			TouchScreen.Y = 0;
			PrintGameOver();
			while(GOver == 1){
				BSP_TS_GetState(&TouchScreen);
				if(TouchScreen.X >= 50 && TouchScreen.X <= 190 && TouchScreen.Y >= 150 && TouchScreen.Y <= 250){
					GOver = 0;
					xTaskResumeAll ();
				}
			}
		}
	}
}

/*Incrementa la posición del Snake en X o en Y dependiendo del valor de State.
La posición se guarda en un array debido a que el tamaño del Snake es variable*/
void ActualizarPosicion(){
	char string[50];
	//Arriba
	if(State == 1){
		if(Ypos[0] <= 300){
			ShiftArray();
			Ypos[0] += 10;
		}
	}//Derecha
	else if(State == 2){
		if(Xpos[0] <= 220){
			ShiftArray();
			Xpos[0] += 10;
		}
	}//Abajo
	else if(State == 3){
		if(Ypos[0] >= 20){
			ShiftArray();
			Ypos[0] -= 10;
		}
	}//Izquierda
	else if(State == 4){
		if(Xpos[0] >= 20){
			ShiftArray();
			Xpos[0] -= 10;
		}
	}
	sprintf(string, "Gusano: %d, %d\n\r", Xpos[0], Ypos[0]);
	CDC_Transmit_HS((uint8_t*)string, strlen(string));
}

//Para parar el Snake, es necesario pulsar el botón de usuario
void PararSnake(){
	if(BSP_PB_GetState(BUTTON_KEY) == SET || GOver == 1){
		State = 0;
	}
}

//Crear Frutas para aumentar tamaño Snake
void CrearFruta(){
	//Genera una posición aleatoria
	uint8_t Valido = 0;
	uint8_t NValidos = 0;
	char string[100];
	while(NValidos < 3){
		while(Valido == 0 && FrutaON == 0){
			FrutasX[NValidos] = (rand() % 23)*10;
			FrutasY[NValidos] = (rand() % 31)*10;
			Valido = 1;
			if(FrutasX[NValidos] <= 10 || FrutasY[NValidos] <= 10 || (FrutasX[NValidos]%10 != 0) || (FrutasY[NValidos]%10 != 0)){
				Valido = 0;
			}else{
				for(uint8_t i = 0; i < Longitud; i++){
					if((FrutasX[0] == Xpos[i] && FrutasY[0] == Ypos[i])){
						Valido = 0;
					}
				}
			}
		}
		Valido = 0;
		NValidos += 1;
	}
	if(FrutaON == 0 && NValidos == 3){
		FrutaON = 1;
		sprintf(string, "Frutas: %d, %d - %d, %d - %d, %d\n\r", FrutasX[0], FrutasY[0], FrutasX[1], FrutasY[1], FrutasX[2], FrutasY[2]);
		CDC_Transmit_HS((uint8_t*)string, strlen(string));
	}
}

//Desplaza arrays de posición para añadir nuevos valores
void ShiftArray(){
	uint8_t i = Longitud-1;
	while(i > 0){
		i--;
		Xpos[i+1] = Xpos[i];
		Ypos[i+1] = Ypos[i];
	}
}

//Comprueba si se ha comido una fruta
void ComprobarFruta(){
	for(uint8_t i = 0; i < NFrutas; i++){
		if(FrutasX[i] == Xpos[0] && FrutasY[i] == Ypos[0]){
			Longitud += 2;
			FrutaON = 0;
		}
	}
}

//Comprobar si te comes a ti mismo
void ComprobarCuerpo(){
	if(State != 0){
		for(uint8_t i = 1; i < Longitud; i++){
			if(Xpos[0] == Xpos[i] && Ypos[0] == Ypos[i]){
				GOver = 1;
			}
		}
	}
}

void PrintGameOver(){
	char Puntos[40];
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font24);
	BSP_LCD_DisplayStringAt(0, 50, "GAME OVER", CENTER_MODE);
	BSP_LCD_DrawRect(50, 150, 140, 100);
	BSP_LCD_DisplayStringAt(0, 188 , "JUGAR", CENTER_MODE);
	BSP_LCD_SetFont(&Font16);
	sprintf(Puntos, "Puntos: %d", (Longitud-LongitudInicial)/2);
	BSP_LCD_DisplayStringAt(0, 90, Puntos, CENTER_MODE);
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
