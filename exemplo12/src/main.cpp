/* *******************************************
* Exemplo 12, testando o uso de um semáforo MUTEX 
* para acesso a recursos e inversão de propriedade
*
******************************************** */

#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define LED1 27
#define LED2 26

SemaphoreHandle_t mutex;
TaskHandle_t task1handle, task2handle;

void task1(void *pvParameters);
void task2(void *pvParameters);

void enviaInformacao(int i){

  xSemaphoreTake(mutex, portMAX_DELAY);
  Serial.println("Enviando informação da Task: " + String(i));
  /* vTaskDelay(pdMS_TO_TICKS(1000)); */
  xSemaphoreGive(mutex);
   /* Nessa função, o semáforo é utilizado para garantir o acesso exclusivo
   a um recurso compartilhado, nesse caso sendo a Serial. */
}

void setup() {
  
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  
  mutex = xSemaphoreCreateMutex();

  xTaskCreate(task1, "Task 1", configMINIMAL_STACK_SIZE+1024, NULL, 1, &task1handle);
  xTaskCreate(task2, "Task 2", configMINIMAL_STACK_SIZE+1024, NULL, 4, &task2handle);
  /* A task 1 tem propriedade menor que a task 2 */
}

void loop() {

  digitalWrite(LED1, HIGH);
  vTaskDelay(pdMS_TO_TICKS(100));
  digitalWrite(LED1, LOW);
  vTaskDelay(pdMS_TO_TICKS(1000));

}

void task1(void *pvParameters){

  while(1){
    /* xSemaphoreTake(mutex, portMAX_DELAY); */
    enviaInformacao(1);
    /* delay(500); */
    /* xSemaphoreGive(mutex); */
    vTaskDelay(pdMS_TO_TICKS(500));

  }
}

void task2(void *pvParameters){

  while(1){
    /* xSemaphoreTake(mutex, portMAX_DELAY); */ 
    enviaInformacao(2);
    /* delay(500); */
    /* xSemaphoreGive(mutex); */
    vTaskDelay(pdMS_TO_TICKS(500));

  }
}