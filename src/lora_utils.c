/**/

#include "main.h"

static const char* TAG = "LoRa";

void lora_utils_init(){
    lora_init();
    lora_set_bandwidth(125E3);
    lora_set_spreading_factor(9);
    lora_set_frequency(915e6);      // Brasil's Legal frequancy
    lora_enable_crc();

    ESP_LOGI(TAG, "LoRa init");
}

Data_t lora_utils_receive(){           
    Data_t buf;

    lora_receive();    // put into receive mode
    while(lora_received()) {
        lora_receive_packet((uint8_t *)&buf, sizeof(buf));
        //ESP_LOGI(TAG, "Received: %f from %d,  with RSSI: %d\n", buf.data, buf.id, lora_packet_rssi());
        return buf;
    }

    buf.id = 404;   //Not a message
    return buf;    
}
