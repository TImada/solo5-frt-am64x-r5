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

/*
 * platform_am64x.c: TI AM64x specific platform layer.
 */

#define _GNU_SOURCE

#include "ti_drivers_config.h"
#include "ti_board_config.h"

#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"

#include "ClockP_freertos_priv.h"

/* Initialization */
/* Early platform initialization */
void early_platform_init(void)
{
    System_init();
    Board_init();
    Drivers_open();
    Board_driversOpen();
    return;
}

/* Timer */
/* Get the timer count */
uint64_t platform_get_timer_count(void)
{
    return ClockP_getTimerCount(gClockCtrl.timerBaseAddr);
}

/* 
 * Timer count adjustment:
 * This function can be useful if the target timer device employs a 
 * specialized counting scheme.
 * Usually, this function should return the argument itself.
 */
uint64_t platform_adjusted_tcount(uint64_t tc)
{
    return (0xFFFFFFFFULL - tc);
}

