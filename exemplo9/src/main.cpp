/* *******************************************
* Exemplo 9, testando o uso de um semáforo binário 
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

/*QueueHandle_t fila;*/
TaskHandle_t task1handle, taskInterrupcaoHandle;
SemaphoreHandle_t semaphore;

int contGlobal; /* variável global utilizada para o exemplo de semáforo binário */

void task1(void *pvParameters);
void taskInterrupcao(void *pvParameters);

void setup(){

  Serial.begin(9600);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  semaphore = xSemaphoreCreateBinary();
  
  xTaskCreate(task1, "Task 1", configMINIMAL_STACK_SIZE+1024, NULL, 1, &task1handle);
  xTaskCreate(taskInterrupcao, "Interrupção", configMINIMAL_STACK_SIZE, NULL, 1, &taskInterrupcaoHandle);
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
      if(tecla == 'p'){
        cont++;
        contGlobal = cont;
        xSemaphoreGive(semaphore); /* essa função é responsável por liberar o semáforo */
        digitalWrite(LED1, HIGH);
        vTaskDelay(pdMS_TO_TICKS(50));
        digitalWrite(LED1, LOW);
      }
      if(tecla == 'm'){
        cont--;
        contGlobal = cont;
        xSemaphoreGive(semaphore); /* essa função é responsável por liberar o semáforo */
        digitalWrite(LED2, HIGH);
        vTaskDelay(pdMS_TO_TICKS(50));
        digitalWrite(LED2, LOW);
        if(cont <= 0){
          cont = 1;
          Serial.println("Estado do contador chegou ao mínimo, não é possível decrementar");
        }
      }
      else if(tecla != 'p' && tecla != 'm'){
        Serial.println("Pressione a tecla 'p' para incrementar contador e 'm' para decrementar.");
      }
    }  
  }
}


void task1(void *pvParameters){

  while(1){
    xSemaphoreTake(semaphore, portMAX_DELAY); /* essa função é responsável por "pegar" o semáforo,
    ou seja,
    a task só vai continuar a execução quando o semáforo for liberado */
    Serial.println("Semáforo recebido");
    Serial.println("Valor do ADC:  " + String(contGlobal));

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