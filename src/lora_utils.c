/**/

#include "main.h"

void lora_utils_init(){
   lora_init();
   lora_set_frequency(915e6);
   lora_enable_crc();
}

Data_t lora_utils_receive(){            // HAY QUE HACER MUCHAS PRUEBAS!! 
    uint8_t buf[32];
    int x;
    
    lora_receive();    // put into receive mode
    while(lora_received()) {
        x = lora_receive_packet(buf, sizeof(buf));  // Guarda la info en el buf
        buf[x] = 0;                                 // Por qué lo elimina??
        printf("Received: %s\n", buf);              // Está imprimiendo Letra a letra, byte o qué? 
        lora_receive();                             // Por qué vuelve a hacer eso?? Sirve si se lo quito??
    }
    vTaskDelay(1);
    
    return (Data_t) buf;    //NO ESTOY MUY SEGURO DE ESTO
}
