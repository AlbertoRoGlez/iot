#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "sys/log.h"
#include <string.h>

#define LOG_MODULE "Broadcast Sending App"
#define LOG_LEVEL LOG_LEVEL_INFO
#define SEND_INTERVAL (8 * CLOCK_SECOND)

PROCESS(broadcast_sender, "Broadcast Sender Example");
AUTOSTART_PROCESSES(&broadcast_sender);

PROCESS_THREAD(broadcast_sender, ev, data){
    static struct etimer periodic_timer;
    static unsigned counter = 0;

    PROCESS_BEGIN();

    nullnet_buf = (uint8_t *) &counter;
    nullnet_len = sizeof(counter);

    etimer_set(&periodic_timer, SEND_INTERVAL);

    while(1){
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        LOG_INFO("Sending %u\n", counter);
        memcpy(nullnet_buf, &counter, sizeof(counter));
        nullnet_len = sizeof(counter);

        NETSTACK_NETWORK.output(NULL);
        counter++;

  	    etimer_reset(&periodic_timer);
    }

    PROCESS_END();   
}
