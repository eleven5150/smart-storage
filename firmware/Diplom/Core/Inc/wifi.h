#ifndef INC_WIFI_H_
#define INC_WIFI_H_

#include "stdio.h"
#include "retarget.h"
#include "string.h"
#include "stdlib.h"
#include "usart.h"
#include "stm32f0xx_hal.h"

typedef struct {
    char* buf;
    int pos;
    int remain;
    int size;
} stream_t;

stream_t* stream_init(stream_t* stream, char* buf, int size);
void stream_increase_remain(stream_t* stream, int size);
char circular_stream_peek_char(stream_t* stream);
char circular_stream_read_char(stream_t* stream);
int circular_stream_skip(stream_t* stream, int count);
int circular_stream_read_string(stream_t* stream, char* buf, int size);
char* circular_stream_read_astring(stream_t* stream, int size);
int circular_stream_lookup_char(stream_t* stream, char ch);
char* circular_stream_read_astring_until(stream_t* stream, char ch);
void ESP_MessageHandler(stream_t* stream, volatile int *cur_remain, volatile int *prv_remain);

#define ESP_RX_BUFFER_SIZE 256

#endif /* INC_WIFI_H_ */
