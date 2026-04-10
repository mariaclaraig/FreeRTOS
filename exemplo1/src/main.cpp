/*   **************************************** 
* Exemplo 1 para criação de tasks no FreeRTOS
*
*
*********************************************/

/* Biblioteca Arduino */
#include <Arduino.h>

/* Bibliotecas FreeRTOS */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Mapeamento de pinos */
#define LED 2

/* Variáveis para armazenamento da handle das tasks */
/* O handle das tasks serve para controlar e manipular as tasks */
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;

/* Protótipos das tasks */
void task1(void *pvParameters);
void task2(void *pvParameters);

void setup(){
  // put your setup code here, to run once:
  Serial.begin(9600);

  /* Criação Task 1: blink do LED*/
  xTaskCreate(
    task1, 
    "Task 1", 
    configMINIMAL_STACK_SIZE, 
    NULL, 
    1, 
    &task1Handle
  );

  /* Criação Task 2: imprimir contador */
  xTaskCreate(
    task2,
    "Task 2",
    configMINIMAL_STACK_SIZE+1024,
    NULL,
    2,
    &task2Handle
  );
}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(1000);
}

void task1(void *pvParameters){

  pinMode(LED,OUTPUT);

  while (1)
  {
    digitalWrite(LED,!digitalRead(LED)); /* acende o LED */
    vTaskDelay(pdMS_TO_TICKS(200)); /* espera 200ms para alternar */    
  }
}

void task2(void *pvParameters){
  
  int cont = 0;
  
  while(1)
  {
    Serial.println("Task 2: " + String(cont++));
    vTaskDelay(pdMS_TO_TICKS(1000)); /* espera 1s para imprimir o próximo valor */
  }
}