# LoRa-Reciver

Tekes the samples from another [LoRa-Sender](https://github.com/Calbetense/LoRa-Sender) and sends via HTTP to a Google Sheets and Telegram. It has the option to send via MQTT as well. 

All sensible info is in the include/secrets.h file, but it's not upload here for obvious reasons. 

If want to use this code either create the same file whith your data or just add it on the main.h defines.

```C
// WiFI
#define WIFI_SSID "XXXXXXXXXXXXX"
#define WIFI_PASS "xxxxxxxxxxxxx"

// HTTP - Google + Telegram
#define SPREADSHEET_ID "xxxxxxxxxxxxxxxx-xxxxxxxxxxxxxxxx"

#define TELEGRAM_ID "botxxxxxxxxxx:xxxxxxxxxxxxxxxx"
#define CHAT_ID "xxxxxxxxx"

// MQTT
#define MQTT_HOST       "mqtt.xxxxxxx.com"
#define MQTT_PORT       1883
#define MQTT_USERNAME   "XXXXXXXXXX"
#define MQTT_PASSWORD   "xxxxxxxxxx"
```
