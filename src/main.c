/*
    RECIVER:

    Colects the data from the LoRa chennel and sends ir to a Google Sheet, via http GET request.
*/

#include <main.h>

static const char* TAG = "MAIN";

static void http_request_task(void *pvParameters)
{
    while (1) {
        trigger_http_request(URL);
        vTaskDelay(pdMS_TO_TICKS(5000)); // Send data every 5s
    }
}

void app_main() {
 
    ESP_LOGI(TAG, "Start program");

    initialise_wifi();
    
    xTaskCreate(&http_request_task, "http_request_task", 10240, NULL, 5, NULL);
}