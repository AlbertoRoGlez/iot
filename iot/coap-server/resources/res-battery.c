#include "contiki.h"
#include "coap-engine.h"
#include "lib/random.h"
#include <stdio.h>

static void
res_get_handler(
    coap_message_t *request,
    coap_message_t *response,
    uint8_t *buffer,
    uint16_t preferred_size,
    int32_t *offset
);

RESOURCE(
    res_battery,
    "title=\"Battery status\";rt=\"battery\"",
    res_get_handler,
    NULL,
    NULL,
    NULL
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
    int battery = random_rand() % 100;
    unsigned int accept = -1;

    coap_get_header_accept(request, &accept);

    if(accept == -1){
        coap_set_header_content_format(response, APPLICATION_JSON);
        snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, "{\"battery\":%d}" , battery);
        coap_set_payload(response, buffer, strlen((char *)buffer));
    }else{
        coap_set_status_code(response, NOT_ACCEPTABLE_4_06);
        const char *message = "Only application/json as content-type is supported.";
        coap_set_payload(response, message, strlen(message));
    }
}