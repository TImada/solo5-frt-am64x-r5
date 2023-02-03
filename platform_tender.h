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

#define _GNU_SOURCE

#include "DebugP.h"
#include "lwip2lwipif.h"

/* Base task priority for MirageOS related tasks */
#define MIRAGE_BASE_PRI (14)

/* Macros for the console */
#define PLATFORM_PRINTF DebugP_log
#define PLATFORM_PERROR PLATFORM_PRINTF

/* Macros for networking */
#define LWIP_INPUT_TASK
#define LWIP_NETIF_INIT LWIPIF_LWIP_init /* AM64x ENET initialization handler */

/* Supported number of each I/O device type */
#define MAX_NETDEV (1U) /* Currently support only one network device */

/* Initialization */
/* Early platform initialization before FreeRTOS becomes ready */
void early_platform_init_1(void);
/* Early platform initialization after FreeRTOS became ready */
void early_platform_init_2(void);

/* Timer */
/* Get the timer count */
uint64_t platform_get_timer_count(void);

/* Timer count adjustment */
uint64_t platform_adjusted_tcount(uint64_t tc);
