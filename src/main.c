/*
    RECIVER:

    Colects the data from the LoRa channel and sends ir to a Google Sheet, via http GET request.
*/

#include <main.h>

static const char* TAG = "MAIN";

static void http_and_lora(void *pvParameters)
{
    Data_t toSend;
    char* url = URL;

    while (1) {
        toSend = lora_utils_receive();

        switch(toSend){
            case Temp:
                strcat(url, "temp="+toSend.data); //Mirar si esto funciona
                break;
            case O2:
                strcat(url, "o2="+toSend.data);
                break;
            case Cont:
                strcat(url, "cont="+toSend.data);
                break; 
            default: 
                ESP_LOGE(TAG, "No se ha recibido nada");
                vTaskDelay(LOQUESEA);
                return;
        }
        trigger_http_request(url);      
        vTaskDelay(pdMS_TO_TICKS(5000)); // Send data every 5s CADA CUÁNTO SERÄ LO ÓPTIMO??
    }
}

void app_main() {
 
    ESP_LOGI(TAG, "Start program");

    initialise_wifi();
    lora_utils_init();

    xTaskCreate(&http_and_lora, "http_and_lora", 1024, NULL, 5, NULL);
}