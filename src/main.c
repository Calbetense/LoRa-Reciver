/*
    RECIVER:

    Colects the data from the LoRa channel and sends it to a Google Sheet, via http GET request.
*/

#include <main.h>

static const char* TAG = "MAIN";

static void http_and_lora(void *pvParameters)
{
    Data_t toSend;
    
    char url[sizeof(URL)+22];   //gambiarra!

    while (1) {
        vTaskDelay(50); 

        toSend = lora_utils_receive();
        if(toSend.id == 404) continue;

        switch(toSend.id){
            case Temp:
                sprintf(url, "%stemp=%.2f", URL, toSend.data);
                break;
            case O2:
                sprintf(url, "%so2=%.2f", URL, toSend.data);
                ESP_LOGI(TAG, "%s", url);
                break;
            case Cont:
                sprintf(url, "%scont=%.2f", URL, toSend.data);
                break; 
            default: 
                sprintf(url, "%s", URL);
        }
        
        trigger_http_request(url);      
    }
}

void app_main() {

    ESP_LOGI(TAG, "Start program, RX");

    initialise_wifi();
    lora_utils_init();

    vTaskDelay(pdMS_TO_TICKS(5000)); 

    xTaskCreate(&http_and_lora, "http_and_lora", 4086, NULL, 5, NULL);

    vTaskDelete(NULL);
}