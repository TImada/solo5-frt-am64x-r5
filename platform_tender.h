/*
 * Copyright (c) 2022 Takayuki Imada <takayuki.imada@gmail.com>
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

/* Macros for the console */
#define PLATFORM_PRINTF DebugP_log
#define PLATFORM_PERROR PLATFORM_PRINTF

/* Initialization */
/* Early platform initialization */
void early_platform_init(void);

/* Timer */
/* Get the timer count */
uint64_t platform_get_timer_count(void);

/* Timer count adjustment */
uint64_t platform_adjusted_tcount(uint64_t tc);
