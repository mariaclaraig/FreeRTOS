/*   **************************************** 
* Exemplo 4 para passagem de parâmetros na criação de tasks no FreeRTOS
*
*
*********************************************/

/* Biblioteca Arduino */
#include <Arduino.h>

/* Bibliotecas FreeRTOS */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Mapeamento de pinos */
#define LED1 2
#define LED2 4

/* Variáveis para armazenamento da handle das tasks */
/* O handle das tasks serve para controlar e manipular as tasks */
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;
TaskHandle_t task3Handle = NULL;

/* Protótipos das tasks */
void taskBlink(void *pvParameters);
void task2(void *pvParameters);


/* variáveis auxiliares*/
int valor = 500;


void setup(){
  // put your setup code here, to run once:
  Serial.begin(9600);

  /* Criação Task 1: blink do LED*/
  xTaskCreate(
    taskBlink, /* função da task */
    "Task 1", /* nome da task */
    configMINIMAL_STACK_SIZE, /* tamanho da pilha */
    (void*)LED1,  /* passa o pino do LED como parâmetro, pode alterar para LED2 (fazer testes) */
    1, /* prioridade da task*/
    &task1Handle /* handle da task*/
  );

  /* Criação Task 2: imprimir contador */
  xTaskCreate(task2, "Task 2", configMINIMAL_STACK_SIZE+1024, (void*)valor, 2, &task2Handle); 
    /* o configMINIMAL_STACK_SIZE+1024 é o tamanho da pilha da task */

  /* Criação Task 3: Blink do outro LED*/
  xTaskCreate(taskBlink, "Task 3", configMINIMAL_STACK_SIZE, (void*)LED2, 1, &task3Handle);
}


/* para uma task, é atribuída uma handle, e para uma handle, é possivel controlar e manipular uma task */



void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(1000);
}





void taskBlink(void *pvParameters){

  int pin = (int)pvParameters; 
  pinMode(pin,OUTPUT);

  while (1)
  {
    digitalWrite(pin, !digitalRead(pin)); /* alterna o estado do LED */
    vTaskDelay(pdMS_TO_TICKS(200)); /* espera 200ms para alternar */    
  }
}





void task2(void *pvParameters){
  
  int cont = (int)pvParameters; /* recupera o valor passado como parâmetro */;
  
  while(1)
  {
    Serial.println("Task 2: " + String(cont++));
     
    vTaskDelay(pdMS_TO_TICKS(1000)); /* espera 1s para imprimir o próximo valor */
  }
}