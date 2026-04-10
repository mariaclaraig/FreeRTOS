/* *******************************************
* Exemplo 11, testando o uso de um semáforo contador 
*
*
******************************************** */
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define LED1 27
#define LED2 26

TaskHandle_t task1handle, taskInterrupcaoHandle;
SemaphoreHandle_t semaphore;

int contGlobal = 0;

void task1(void *pvParameters);
void taskInterrupcao(void *pvParameters);

void setup(){
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  semaphore = xSemaphoreCreateCounting(10, 0);
  
  xTaskCreate(task1, "Task 1", configMINIMAL_STACK_SIZE+2048, NULL, 1, &task1handle);
  xTaskCreate(taskInterrupcao, "Interrupção", configMINIMAL_STACK_SIZE+2048, NULL, 1, &taskInterrupcaoHandle);
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void taskInterrupcao(void *pvParameters){
  int cont = 0;
  char tecla;

  while(1){
    if(Serial.available()){
      tecla = Serial.read();

      if(tecla == 'p'){
        cont++;
        contGlobal = cont;
        xSemaphoreGive(semaphore);
        digitalWrite(LED1, HIGH);
        vTaskDelay(pdMS_TO_TICKS(50));
        digitalWrite(LED1, LOW);
      }
      else if(tecla == 'm'){
        if(cont > 0){
          cont--;
          contGlobal = cont;
          xSemaphoreGive(semaphore);
          digitalWrite(LED2, HIGH);
          vTaskDelay(pdMS_TO_TICKS(50));
          digitalWrite(LED2, LOW);
        } else {
          Serial.println("Contador já está em zero.");
        }
      }
      else{
        Serial.println("Pressione 'p' para incrementar e 'm' para decrementar.");
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100)); 
  }
}

void task1(void *pvParameters){
  while(1){
    xSemaphoreTake(semaphore, portMAX_DELAY);
    Serial.println("Evento processado. Contador: " + String(contGlobal));
  }
}

/*
#include <Arduino.h>

void setup() {
  Serial.begin(115200);
}

void loop() {
}

*/