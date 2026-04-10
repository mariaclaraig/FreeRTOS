/* *******************************************
* Exemplo 13, software timers 
* 
*
******************************************** */

#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"

#define LED1 27
#define LED2 26
#define BT 25

TaskHandle_t task1handle;
TimerHandle_t timer1, timer2;

void task1(void *pvParameters);

void callBackTimer1(TimerHandle_t xTimer);
void callBackTimer2(TimerHandle_t xTimer);

void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BT, INPUT_PULLUP);

  timer1 = xTimerCreate("Timer 1", pdMS_TO_TICKS(1000), pdTRUE, 0, callBackTimer1);
  timer2 = xTimerCreate("Timer 2", pdMS_TO_TICKS(10000), pdFALSE, 0, callBackTimer2);
  
  /* pdTRUE na criação do timer é o autoreload; se fosse pdFALSE, o timer
  seria disparado apenas uma vez*/

  xTaskCreate(task1, "Task 1", configMINIMAL_STACK_SIZE+1024, NULL, 1, &task1handle);

  xTimerStart(timer1, 0);
}

void loop() {

  vTaskDelay(pdMS_TO_TICKS(1000));

}

void task1(void *pvParameters){

  uint8_t debouncingTime = 0;

  while(1){
    if((digitalRead(BT) == LOW) && (xTimerIsTimerActive(timer2) == pdFALSE)){
      debouncingTime++;
      if(debouncingTime >= 10){
        debouncingTime = 0;
        digitalWrite(LED2,HIGH);
        xTimerStart(timer2,0);
        xTimerStop(timer1,0);
        Serial.println("Iniciando o Timer 2");
      }
    }
    else
    {
      debouncingTime = 0;
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}


void callBackTimer1(TimerHandle_t xTimer){

  digitalWrite(LED1, !digitalRead(LED1));

}

void callBackTimer2(TimerHandle_t xTimer){
  digitalWrite(LED2, LOW); /* a ideia é fazer ações rápidas, pois o tempo que fica preso 
  no callback pode prejudicar o desempenho do sistema*/
  xTimerStart(timer1,0);
}