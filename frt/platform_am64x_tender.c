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

#include <assert.h>

#include "FreeRTOS.h"

#include "ti_drivers_config.h"
#include "ti_board_config.h"

#include <networking/enet/utils/include/enet_board.h>
#include <networking/enet/utils/include/enet_apputils.h>
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"
#include <networking/enet/core/include/per/cpsw.h>
#include "ti_enet_config.h"
#include "ti_enet_open_close.h"

#include "ClockP_freertos_priv.h"

#include "lwip/sys.h"
#include "lwip/tcpip.h"

extern void EnetApp_addMCastEntry(Enet_Type enetType,
                                  uint32_t instId,
                                  uint32_t coreId,
                                  const uint8_t *testMCastAddr,
                                  uint32_t portMask);


/* Initialization */
/* Early platform initialization before FreeRTOS becomes ready */
void early_platform_init_1(void)
{
    System_init();
    Board_init();

    return;
}

/* Early platform initialization after FreeRTOS became ready */
void early_platform_init_2(void)
{
    Enet_Type enetType;
    uint32_t instId, status;
    const uint8_t BCastAddr[6] =
    {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };

    Drivers_open();
    Board_driversOpen();

    EnetApp_getEnetInstInfo(&enetType, &instId);
    EnetAppUtils_enableClocks(enetType, instId);
    status = EnetApp_driverOpen(enetType, instId);
    assert(status == ENET_SOK);

    EnetApp_addMCastEntry(enetType,
                          instId,
                          EnetSoc_getCoreId(),
                          BCastAddr,
                          CPSW_ALE_ALL_PORTS_MASK);

#if LWIP_TCPIP_CORE_LOCKING
    if (sys_mutex_new(&lock_tcpip_core) != ERR_OK) {
      LWIP_ASSERT("failed to create lock_tcpip_core", 0);
    }
#endif /* LWIP_TCPIP_CORE_LOCKING */

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

