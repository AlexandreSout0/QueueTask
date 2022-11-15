#include <Arduino.h>

xQueueHandle fila_de_temperaturas;
float temperatura_media;

void leitura_de_sensores(void *pvParameters);
void calcula_media_movel(void *pvParameters);


void leitura_de_sensores(void *pvParameters)
{
  float temperatura;
  while(true)
  {
    //leitura temperatura sensor
    temperatura = 20.0 + ((float) rand() / (float)(RAND_MAX/10));
    long resposta = xQueueSend(fila_de_temperaturas, &temperatura, 1000 / portTICK_PERIOD_MS);
    if(resposta == true)
    {
      Serial.println("Temperatura adicionada a fila");
    }
    else
    {
      Serial.println("Falha ao enviar para fila");
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void calcula_media_movel(void *pvParameters)
{
  float temperatura = 0.0;
  float temperatura_anterior = 0.0;
  while(true)
  {
    if(xQueueReceive(fila_de_temperaturas, &temperatura, 5000 / portTICK_PERIOD_MS))
    {
      temperatura_media = (temperatura_anterior + temperatura) / 2.0;
      temperatura_anterior = temperatura;
      Serial.printf("Temperatura média %f", temperatura_media);
    }
    else
    {
      Serial.println("Fila não disponivel");
    }
   // vTaskDelay(1000 / portTICK_PERIOD_MS);

  }

}

void setup() 
{
  fila_de_temperaturas = xQueueCreate(5,sizeof(float));
  Serial.begin(9600);
  xTaskCreate(&leitura_de_sensores,"Lê Temperatura", 2048, NULL,1,NULL);
 // xTaskCreate(&calcula_media_movel, "Calcula temperatura média movel", 2048, NULL,2,NULL);
}

void loop() 
{
  // put your main code here, to run repeatedly:
}