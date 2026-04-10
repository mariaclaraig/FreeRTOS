



#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

#define LED1 2
#define LED2 27

// Mapeamento de eventos
#define TASK1_FLAG (1 << 0) // 1 
#define TASK2_FLAG (1 << 1) // 10

TaskHandle_t task1handle, task2handle;
TimerHandle_t timerhandle;
EventGroupHandle_t eventos;

/* prototipagem */
void task1(void *pvParameters);
void task2(void *pvParameters);
void callBackTimer1(TimerHandle_t xTimer);
int tempo = 0;


void setup(){
    Serial.begin(9600);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);

    eventos = xEventGroupCreate();
    timerhandle = xTimerCreate("Timer 1", pdMS_TO_TICKS(1000), pdTRUE, (void *) 0, callBackTimer1);
    xTaskCreate(task1, "Task 1", configMINIMAL_STACK_SIZE+1024, NULL, 1, &task1handle);
    xTaskCreate(task2, "Task 2", configMINIMAL_STACK_SIZE+1024, NULL, 1, &task2handle);

    xTimerStart(timerhandle, 0);

}

void loop(){


}

void task1(void *pvParameters){

    EventBits_t xBits;

    while(1){
        xBits = xEventGroupWaitBits(eventos, TASK1_FLAG, pdTRUE, pdTRUE, portMAX_DELAY);
        Serial.println("Task 1 saiu do estado de bloqueio");
        digitalWrite(LED1, !digitalRead(LED1));        
        vTaskDelay(pdMS_TO_TICKS(500));

    }
}


void task2(void *pvParameters){
    
    EventBits_t xBits;

    while(1){
        xBits = xEventGroupWaitBits(eventos, TASK2_FLAG, pdTRUE, pdTRUE, portMAX_DELAY);
        Serial.println("Task 2 saiu do estado de bloqueio");
        digitalWrite(LED2, !digitalRead(LED2));
        vTaskDelay(pdMS_TO_TICKS(500));

    }
}


void callBackTimer1(TimerHandle_t xTimer){
    tempo++;
    if(tempo == 5){
        xEventGroupSetBits(eventos, TASK1_FLAG);
    }
    else if(tempo == 10){
        xEventGroupSetBits(eventos, TASK2_FLAG);
        tempo = 0;
    }
}


/*
void setup(){
    Serial.begin(9600);
}

void loop(){}

*/