#include <Arduino.h>

#include <TinyGPS++.h>
#include <tinygpsplus.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED 2
#define BT 25

TinyGPSPlus gps;
TaskHandle_t taskTrataBThandle;

void taskTrataBT(void *pvParameters);

void callBackBT(void){
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveFromISR(taskTrataBThandle, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken); // solicita troca de contexto se for necessário
}

void setup(){
  Serial.begin(9600);

  
  Serial1.begin(9600, SERIAL_8N1, 32, 33);  
  pinMode(LED, OUTPUT);
  pinMode(BT, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BT), callBackBT, FALLING);

  xTaskCreate(taskTrataBT, "taskTrataBT", configMINIMAL_STACK_SIZE, NULL, 1, &taskTrataBThandle);
}

void loop(){

  while(Serial1.available()){
    gps.encode(Serial1.read());
  }
  
  Serial.print("Latitude: ");
  Serial.print(gps.location.lat(), 6);
  Serial.print(" | Longitude: ");
  Serial.println(gps.location.lng(), 6);
  
  vTaskDelay(pdMS_TO_TICKS(1000));
  String resposta = Serial1.readString();

}

void taskTrataBT(void *pvParameters){
  uint32_t quantidade = 0;
  while(1){
    
    quantidade = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    Serial.println("Tratando ISR botão: " + String(quantidade));
    digitalWrite(LED, !digitalRead(LED));
    delay(500);
  }
}