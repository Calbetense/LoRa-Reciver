/*
    RECIVER:

    Colects the data from the LoRa channel and sends it to a Google Sheet, via http GET request. 
    Also, if desired, it can send message to Telegram (http) and to a MQTT broker

    Configure the wanted methods at main.h's defines, and the credentials you have to define them at a secrets.h document that it's not included in GIT
*/
      
#include <main.h>

static const char* TAG = "MAIN";

static void http_and_lora(void *pvParameters)
{
    //Declarations
    Data_t toSend;
    esp_mqtt_client_handle_t client = mqtt_init();  // MQTT Sesion  //TODO - Init method at app_main()
    
    char url[sizeof(URL)+22];               // Gambiarra! Definir esse número (variable=toSend.data + letras do nome da variavel)
    #ifdef ORP_TELEGRAM
    char url_orp[sizeof(URL_ORP_LUCAS)+22];
    #endif
    char url_rssi[sizeof(URL_RSSI)+22];

    #ifdef MQTT
    //const char* topic[70];       // sure 70 is the max length ??                  // TODO
    char data[sizeof(float)*2];
    #endif

    //Loop
    while (1) {
        vTaskDelay(50);                     // Little rest for the processor

        toSend = lora_utils_receive();
        if(toSend.id == 404) continue;

        switch(toSend.sender){
            case BERCARIO_01:
                break;
            default:
                ESP_LOGI(TAG, "Where the hell are the sensors it placed???");
        }

        switch(toSend.id){
            case Temp:
                sprintf(url, "%stemp=%.2f", URL, toSend.data);
                
                #ifdef MQTT           
                sprintf(data, "%.2f", toSend.data);
                esp_mqtt_client_publish(client, "/iabs/petrolina/projetocamarao/bebedouro/bercario/agua/temperatura", data, 0, 2, 0);
                #endif
                
                break;
            case O2:
                sprintf(url, "%so2=%.2f",   URL, toSend.data);

                #ifdef MQTT           
                sprintf(data, "%.2f", toSend.data);
                esp_mqtt_client_publish(client, "/iabs/petrolina/projetocamarao/bebedouro/bercario/agua/oxigeniodissolvido", data, 0, 2, 0);
                #endif

                break;
            case Orp:
                sprintf(url, "%sorp=%.2f",  URL, toSend.data);
                #ifdef MQTT           
                sprintf(data, "%.2f", toSend.data);
                esp_mqtt_client_publish(client, "iabs/petrolina/projetocamarao/bebedouro/bercario/agua/orp", data, 0, 2, 0);
                #endif

                break; 
            case Cont:
                sprintf(url, "%scont=%.2f", URL, toSend.data);
                
                #ifdef MQTT           
                sprintf(data, "%.2f", toSend.data);
                //esp_mqtt_client_publish(client, "/iabs/petrolina/projetocamarao/sensorcontinuidade/", data, 0, 2, 0);
                #endif

                break; 
            default: 
                sprintf(url, "%s", URL);
        }
        
        //Google spreadSheets - General information
        trigger_http_request(url); 
    
        //Google spreadsheets - LoRa RSSI
        sprintf(url_rssi, "%srssi=%d",  URL_RSSI, lora_packet_rssi());
        ESP_LOGI(TAG, "%s", url_rssi);
        trigger_http_request(url_rssi);

        #ifdef ORP_TELEGRAM
        /*Temporally, sends ORP to Telegram*/
        if(toSend.id == Orp){
            //LUCAS
            sprintf(url_orp, "%sorp=%.2fmV",  URL_ORP_LUCAS, toSend.data);
            trigger_http_request(url_orp);    
            //DANI
            /*sprintf(url_orp, "%sorp=%.2fmV",  URL_ORP_DANI, toSend.data);
            trigger_http_request(url_orp);      */
        }else if(toSend.id == O2){
            //LUCAS
            sprintf(url_orp, "%sD.O.=%.2fmg/L",  URL_ORP_LUCAS, toSend.data);
            //trigger_http_request(url_orp); // Activate when calibration's ready
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