#include "wifi.h"

stream_t* stream_init(stream_t* stream, char* buf, int size) {
    stream->buf = buf;
    stream->size = size;
    stream->pos = 0;
    stream->remain = 0;
    return stream;
}

void stream_increase_remain(stream_t* stream, int size) {
    stream->remain += size;
}

char circular_stream_peek_char(stream_t* stream) {
    if (stream->remain == 0)
        return 0;
    uint16_t pos = stream->pos % stream->size;
    return stream->buf[pos];
}

char circular_stream_read_char(stream_t* stream) {
    char ch = circular_stream_peek_char(stream);
    if (ch != 0) {
        stream->pos += 1;
        stream->remain -= 1;
    }
    return ch;
}

int circular_stream_skip(stream_t* stream, int count) {
    if (stream->remain < count)
        return -1;
    stream->pos += count;
    stream->remain -= count;
    return count;
}

int circular_stream_read_string(stream_t* stream, char* buf, int size) {
    int k = 0;
    for (k = 0; k < size; k++) {
        buf[k] = circular_stream_read_char(stream);
        if (buf[k] == 0)
            return -1;
    }
    return 0;
}

char* circular_stream_read_astring(stream_t* stream, int size) {
    char* buf = (char*) calloc(size + 1, sizeof(char));
    if (circular_stream_read_string(stream, buf, size) < 0) {
        free(buf);
        return NULL;
    }
    return buf;
}

int circular_stream_lookup_char(stream_t* stream, char ch) {
    int k = 0;
    for (k = 0; k < stream->remain; k++) {
        int offset = stream->pos + k;
        uint16_t pos = offset % stream->size;
        char sch = stream->buf[pos];
        if (sch == ch)
            return offset;
    }
    return -1;
}

char* circular_stream_read_astring_until(stream_t* stream, char ch) {
    int offset = circular_stream_lookup_char(stream, ch);
    if (offset == -1)
        return NULL;
    int size = offset - stream->pos;
    return circular_stream_read_astring(stream, size);
}

void ESP_MessageHandler(stream_t* stream, volatile int *cur_remain, volatile int *prv_remain)
{

    HAL_Delay(100);
    // get current remain from DMA buffer
    *cur_remain = huart1.hdmarx->Instance->CNDTR;

    // check if something incomming
    int addend = 0;
    if (*cur_remain > *prv_remain)
    {
        int size1 = *prv_remain;
        int size2 = stream->size - *cur_remain;
        addend = size1 + size2;
    }
    else if (*cur_remain < *prv_remain)
    {
        addend = *prv_remain - *cur_remain;
    }
    else
    {
        return;
    }
    stream_increase_remain(stream, addend);

    // processing messages
    while (1)
    {
        // filter out all bad characters
        while (circular_stream_peek_char(stream) == '\0')
        {
            if (circular_stream_skip(stream, 1) == -1)
            {
                break;
            }
        }

        char* msg = circular_stream_read_astring_until(stream, '\r');
        if (msg == NULL)
        {
            break;
        }
        DEBUG_PRINT(DEBUG_PRINT_INFO, "[ESP] %s\r\n", msg);

        free(msg);

        // get rid of '\r' char
        circular_stream_read_char(stream);
        // drop next char if it is '\n'
        if ('\n' == circular_stream_peek_char(stream))
        {
            circular_stream_read_char(stream);
        }
    }
    *prv_remain = *cur_remain;
}




