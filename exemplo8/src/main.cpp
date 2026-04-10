/* *******************************************
* Exemplo 8 para enviar valores a uma fila (sem
* uso de botão na protoboard, simulando uma task
* com um botão do teclado do computador).
******************************************** */


#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"


#define LED 2

QueueHandle_t fila;
TaskHandle_t task1handle, taskInterrupcaoHandle;

void task1(void *pvParameters);
void taskInterrupcao(void *pvParameters);

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  fila = xQueueCreate(255, sizeof(int));

  xTaskCreate(task1, "Task 1", configMINIMAL_STACK_SIZE+1024, NULL, 1, &task1handle);
  xTaskCreate(taskInterrupcao, "Interrupção", configMINIMAL_STACK_SIZE, NULL, 1,&taskInterrupcaoHandle);
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}

/* simulando o uso de um botão da protoboard, mas através de outra task */
void taskInterrupcao(void *pvParameters){

  int cont = 0;
  char tecla;

  while(1){
    if(Serial.available()){
      tecla = Serial.read();
      if(tecla == '*'){
        cont++;
        xQueueSend(fila, &cont, portMAX_DELAY);
        digitalWrite(LED, HIGH);
        vTaskDelay(pdMS_TO_TICKS(50));
        digitalWrite(LED, LOW);
      }
      else{
        Serial.println("Pressione a tecla '*' para incrementar contador.");
      }
    }  
  }
}


void task1(void *pvParameters){

  int valorRecebido;

  while(1){

    xQueueReceive(fila, &valorRecebido, portMAX_DELAY);
    Serial.println("Valor recebido na fila: " + String(valorRecebido));
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