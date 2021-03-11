#pragma once

/************ Includes ***************/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include <esp_log.h>

#include <esp_wifi.h>
#include <esp_system.h>
#include <esp_event.h>
#include <esp_event_loop.h>
#include <nvs_flash.h>

#include "esp_http_client.h"

#include "lora.h"
#include <mqtt_client.h>

#include "secrets.h"

/************** Defines ****************/
// WiFi SSID and password, and SPREADSHEET_ID in the secrets.h document
#define URL "https://script.google.com/macros/s/"SPREADSHEET_ID"/exec?"     //Make sure the SpreadSheet ID is the correct one

#define ORP_TELEGRAM
#ifdef ORP_TELEGRAM
#define URL_ORP_LUCAS "https://api.telegram.org/"TELEGRAM_ID"/sendMessage?chat_id="CHAT_ID_LUCAS"&text="
#define URL_ORP_DANI "https://api.telegram.org/"TELEGRAM_ID"/sendMessage?chat_id="CHAT_ID_DANI"&text="
#endif

#define MQTT

#define URL_RSSI "https://script.google.com/macros/s/"RSSI_ID"/exec?"     


#define DEBUG

/*Defines needed of LoRa Library*/
#define CONFIG_CS_GPIO      18
#define CONFIG_RST_GPIO     14
#define CONFIG_MISO_GPIO    19
#define CONFIG_MOSI_GPIO    27
#define CONFIG_SCK_GPIO     5

/*Structures*/
typedef enum {
    O2,
    Temp,
    Cont,
    Orp
}Id_t;

/* All the data will reach with an identifier */
typedef struct
{
    Id_t id;
    float data;
}Data_t;


/* FreeRTOS event group to signal when we are connected & ready to make a request */
EventGroupHandle_t wifi_event_group;

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
extern int CONNECTED_BIT;

/****************** Declarations ***************/
// HTTP
esp_err_t _http_event_handle(esp_http_client_event_t *evt);
void trigger_http_request(const char *url);

// LoRa
void lora_utils_init();
Data_t lora_utils_receive();

// WiFI
esp_err_t wifi_event_handler(void *ctx, system_event_t *event);
void initialise_wifi(void);

// MQTT
esp_mqtt_client_handle_t mqtt_init();