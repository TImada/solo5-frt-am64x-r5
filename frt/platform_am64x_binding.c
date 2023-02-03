/*
 * Copyright (c) 2022-2023 Takayuki Imada <takayuki.imada@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * platform_am64x_binding.c: Binding layer specific to TI AM64x.
 */

#define _GNU_SOURCE

#include "DebugP.h"
#include "FreeRTOS.h"
#include "lwip2enet.h"
#include "lwip/netif.h"
#include "platform_tender.h" /* Obtain MAX_NETDEV */

extern struct netif solo5_netif[MAX_NETDEV];

/* Initialization */
/* CPU initialization */
void platform_cpu_init(void)
{
    return;
}

/* Timer initialization */
void platform_timer_init(void)
{
    return;
}

/* Console initialization */
void platform_console_init(void)
{
    return;
}

/* Block device initialization */
void platform_block_init(void)
{
    return;
}

/* Network device initialization */
void platform_net_init(void)
{
    unsigned int i;
    struct netif *netif;
    Lwip2Enet_Handle hLwip2Enet;

    /* Wait until each target network port becomes ready */
    for (i = 0U; i < MAX_NETDEV; i++) {
        netif = &solo5_netif[i];
        if (netif == NULL) {
            continue;
        }
        hLwip2Enet = (Lwip2Enet_Handle)netif->state;

        while (hLwip2Enet->linkIsUp == false) {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }

    vTaskDelay(500 / portTICK_PERIOD_MS);
    return;
}

/* Console */
int platform_puts(const char *buf, int n)
{
    /* TODO:
     * We do not use DebugP_log(s) to insert '\r' befor '\n'.
     * Too primitive implementation, this should be replaced
     * by a smarter scheme.
     */
    int i;

    for(i = 0; i < n; i++) {
        switch(buf[i]){
            case '\n':
                /* Insert '\r' internally before '\n' */
                DebugP_uartLogWriterPutChar('\r');
                DebugP_uartLogWriterPutChar('\n');
                break;
            default:
                DebugP_uartLogWriterPutChar(buf[i]);
                break;
        }
    }
    return n;
}
