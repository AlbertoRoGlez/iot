#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "sys/log.h"
#include <string.h>

#define LOG_MODULE "Broadcast Receiver App"
#define LOG_LEVEL LOG_LEVEL_INFO
#define WAIT_INTERVAL (5 * CLOCK_SECOND)

PROCESS(broadcast_receiver, "Broadcast Receiver Example");
AUTOSTART_PROCESSES(&broadcast_receiver);

void input_callback(const void * data, uint16_t len,
    const linkaddr_t * source, const linkaddr_t * destination){
    if(len == sizeof(unsigned)){
        unsigned counter;
        memcpy(&counter, data, sizeof(counter));

        LOG_INFO("Received %u from ", counter);
        LOG_INFO_LLADDR(source);
        LOG_INFO("\n");
    }
}

PROCESS_THREAD(broadcast_receiver, ev, data){
    static struct etimer periodic_timer;
    static unsigned counter;    

    PROCESS_BEGIN();

    nullnet_buf = (uint8_t *) &counter;
    nullnet_len = sizeof(counter);
    nullnet_set_input_callback(input_callback);

    etimer_set(&periodic_timer, WAIT_INTERVAL);

    while(1){
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        LOG_INFO("Waiting for data..\n");

        etimer_reset(&periodic_timer);
    }

    PROCESS_END();   
}
