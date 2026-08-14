#include "device_driver.h"

typedef struct {
    char buffer[RING_BUF_SIZE];
    volatile int head; // Write Index
    volatile int tail; // Read Index
} RingBuffer;

static RingBuffer rx_q;

void RingBuf_Init(void)
{
    rx_q.head = 0;
    rx_q.tail = 0;
}

int RingBuf_IsFull(void)
{
    return ((rx_q.head + 1) % RING_BUF_SIZE) == rx_q.tail;
}

int RingBuf_IsEmpty(void)
{
    return rx_q.head == rx_q.tail;
}

int RingBuf_Put(char c)
{
    if (RingBuf_IsFull()) return 0;
    rx_q.buffer[rx_q.head] = c;
    rx_q.head = (rx_q.head + 1) % RING_BUF_SIZE;
    return 1;
}

int RingBuf_Get(char *c)
{
    if (RingBuf_IsEmpty()) return 0;
    *c = rx_q.buffer[rx_q.tail];
    rx_q.tail = (rx_q.tail + 1) % RING_BUF_SIZE;
    return 1;
}