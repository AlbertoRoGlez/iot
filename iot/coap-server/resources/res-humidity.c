#include "contiki.h"
#include "coap-engine.h"
#include "virtual-sensor.h"
#include <stdio.h>

static void
res_get_handler(
    coap_message_t *request,
    coap_message_t *response,
    uint8_t *buffer,
    uint16_t preferred_size,
    int32_t *offset
);

static void res_periodic_handler(void);

PERIODIC_RESOURCE(
    res_humidity,
    "title=\"Humidity\";rt=\"humidity\";obs",
    res_get_handler,
    NULL,
    NULL,
    NULL,
    5000,
    res_periodic_handler
);

static void
res_get_handler(
    coap_message_t *request,
    coap_message_t *response,
    uint8_t *buffer,
    uint16_t preferred_size,
    int32_t *offset
)
{
    float humidity = read_humidity();
    unsigned int accept = -1;

    if(accept == APPLICATION_JSON || accept == -1){
        coap_set_header_content_format(response, APPLICATION_JSON);
        snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, "{\"humidity\":\"%.2f\"}", humidity);
        coap_set_payload(response, (uint8_t *)buffer, strlen((char *)buffer));
    }else{
        coap_set_status_code(response, NOT_ACCEPTABLE_4_06);
        const char *message = "Only application/json as content-type is supported.";
        coap_set_payload(response, message, strlen(message));
    }
}

static void 
res_periodic_handler()
{
    coap_notify_observers(&res_humidity);
}