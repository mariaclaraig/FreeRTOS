/*   **************************************** 
* Exemplo 7 para criar criar filas, adicionar e ler
* dados da fila através de duas tasks 
*
******************************************** */

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define LED 27

QueueHandle_t fila_x;

TaskHandle_t task1Handle, task2Handle;

void task1(void *pvParameters);
void task2(void *pvParamaters);

void setup() {
  
  BaseType_t returned;

  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  fila_x = xQueueCreate(5, sizeof(int)); /* cria uma fila capaz de armazenar 5 inteiros */

  if(fila_x == NULL){
    Serial.println("Não foi possível criar fila");
    while (1); /* o while permite que o programa continue parado aqui até que a condição seja desfeita */
  }

  returned = xTaskCreate(task1, "Task 1", configMINIMAL_STACK_SIZE+1024, NULL, 1, &task1Handle);

  /* leitura do retorno, a título de aprendizado e curiosidade */
  if(returned == pdFAIL){
    Serial.println("Não foi possível criar task 1");
    while (1); /* o while permite que o programa continue parado aqui até que a condição seja desfeita */
  }

  returned = xTaskCreate(task2, "Task 2", configMINIMAL_STACK_SIZE+1024, NULL, 1, &task2Handle);

  /* leitura do retorno, a título de aprendizado e curiosidade */
  if(returned == pdFAIL){
    Serial.println("Não foi possível criar task 2");
    while (1); /* o while permite que o programa continue parado aqui até que a condição seja desfeita */
  }

}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(500));
}




void task1(void *pvParameters){

  int cont = 0;

  while(1){
    if(cont<20){
      xQueueSend(fila_x, &cont, portMAX_DELAY);
      cont++;
      digitalWrite(LED, !digitalRead(LED));
    }

    else{
      cont = 0;
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void task2(void *pvParameters){

  int valor_recebido = 0;

  while(1){
    if(xQueueReceive(fila_x, &valor_recebido, pdMS_TO_TICKS(1000)) == pdTRUE){
      Serial.println("Valor recebido: " + String(valor_recebido));
    }
    else{
      Serial.println("TIMEOUT");
      digitalWrite(LED, LOW);
    }
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