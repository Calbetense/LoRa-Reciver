
#pragma once

/*Includes*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include <esp_log.h>

#include <esp_wifi.h>
#include <esp_system.h>
#include <esp_event.h>
#include <esp_event_loop.h>
#include <nvs_flash.h>

#include "esp_http_client.h"

#include "lora.h"

#include <string.h>

/*Defines*/

#define WIFI_SSID "IABS-REC"
#define WIFI_PASS "sustentabilidade"

#define SPREADSHEET_ID "AKfycbzcyzndCjZdTTxk6c81hTpXzdCjtw6--AkWKRXAAeBq1kCo2Ek" 
#define URL "https://script.google.com/macros/s/"SPREADSHEET_ID"/exec?" // Change here the GET parameters

/*Structures*/
typedef enum {
    O2,
    Temp,
    Cont
}Id_t;

typedef struct
{
    Id_t id;
    long data;
}Data_t;


/* FreeRTOS event group to signal when we are connected & ready to make a request */
EventGroupHandle_t wifi_event_group;

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
extern int CONNECTED_BIT;

/*declarations*/
    // HTTP
    esp_err_t _http_event_handle(esp_http_client_event_t *evt);
    void trigger_http_request(const char *url);

    // LoRa
    void lora_utils_init();
    Data_t lora_utils_receive();

    // WiFI
    esp_err_t wifi_event_handler(void *ctx, system_event_t *event);
    void initialise_wifi(void);
