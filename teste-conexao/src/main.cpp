#include <Arduino.h>

#include <TinyGPS++.h>
#include <tinygpsplus.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_task_wdt.h"


#define LED 2

TinyGPSPlus gps;
TaskHandle_t taskStatusHandle;
TaskHandle_t taskSetupHandle;

/*

void taskSetup(void *pvParameters);
*/
void taskStatus(void *pvParameters);

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8N1, 32, 33);
  pinMode(LED, OUTPUT);

  xTaskCreate(taskStatus, "Task de Status", 4096, NULL, 1, &taskStatusHandle);

}

void loop(){

  
  gps.encode(Serial1.read());

  String resposta = Serial1.readString();
  Serial.println("Resposta:" + resposta);
  // esp_task_wdt_add(NULL);
  vTaskDelay(pdMS_TO_TICKS(1));
  
  // while(Serial1.available()){
  //   gps.encode(Serial1.read());
  vTaskDelay(pdMS_TO_TICKS(1000));
  // String resposta = Serial1.readString();
}

void taskStatus(void *pvParameters){

  while(1){

  digitalWrite(LED, !digitalRead(LED));
  vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
