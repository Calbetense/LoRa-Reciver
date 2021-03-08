/*
    RECIVER:

    Colects the data from the LoRa channel and sends it to a Google Sheet, via http GET request.
    Loop: Get the data, complets the url with the info and send to Internet
*/
      
#include <main.h>

static const char* TAG = "MAIN";

static void http_and_lora(void *pvParameters)
{
    //Declarations
    Data_t toSend;
    
    char url[sizeof(URL)+22];               // Gambiarra! Definir esse número (variable=toSend.data)
    #ifdef ORP_TELEGRAM
    char url_orp[sizeof(URL_ORP_LUCAS)+22];
    #endif
    char url_rssi[sizeof(URL_RSSI)+22];

    //Loop
    while (1) {
        vTaskDelay(50);                     // Little rest for the processor

        toSend = lora_utils_receive();
        if(toSend.id == 404) continue;

        switch(toSend.id){
            case Temp:
                sprintf(url, "%stemp=%.2f", URL, toSend.data);
                break;
            case O2:
                sprintf(url, "%so2=%.2f",   URL, toSend.data);
                break;
            case Cont:
                sprintf(url, "%scont=%.2f", URL, toSend.data);
                break; 
            case Orp:
                sprintf(url, "%sorp=%.2f",  URL, toSend.data);
                //ESP_LOGI(TAG, "%s", url);
                break; 
            default: 
                sprintf(url, "%s", URL);
        }
        
        trigger_http_request(url);  

        #ifdef ORP_TELEGRAM
        /*Temporally, sends ORP to Telegram*/
        if(toSend.id == Orp){
            //LUCAS
            sprintf(url_orp, "%sorp=%.2fmV",  URL_ORP_LUCAS, toSend.data);
            trigger_http_request(url_orp);    
            //DANI
            /*sprintf(url_orp, "%sorp=%.2fmV",  URL_ORP_DANI, toSend.data);
            trigger_http_request(url_orp);      */
            
            ESP_LOGI(TAG, "%s", url_rssi);
            trigger_http_request(url_rssi);
        }
        #endif

    }
    
}

void app_main() {

    #ifdef DEBUG
    ESP_LOGI(TAG, "Start program, RX");
    #endif

    initialise_wifi();
    lora_utils_init();

    vTaskDelay(pdMS_TO_TICKS(5000));        // Gambiarra!! How long will this last to init EXACTLY??

    xTaskCreate(&http_and_lora, "http_and_lora", 4086, NULL, 5, NULL);

    vTaskDelete(NULL);
}