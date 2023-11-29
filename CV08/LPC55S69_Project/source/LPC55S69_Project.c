/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    LPC55S69_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC55S69_cm33_core0.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

// STRCMP Compare
int comparePasswords(const char *password1, const char *password2) {
    return strcmp(password1, password2) == 0;
}

// Constant Time Compare
int constantTimeCompare(const char *str1, const char *str2, size_t len) {
    uint32_t diff = 0;

    for (size_t i = 0; i < len; i++) {
        diff |= str1[i] ^ str2[i];
    }

    return diff == 0;
}


int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
    	        volatile static int i = 0;
        char password1[100];
        char password2[100];
        uint8_t status;

        while(1) {
            i++;
            __asm volatile ("nop");

            PRINTF("------------------------- \n\n\r");
            PRINTF("Hello, I am your terminal :) \n\n\r");

            // Reading passwords
            PRINTF("Enter first string:\n\r");
            SCANF("%99s", password1);
            PRINTF("%d \n\r",atoi(password1));
            PRINTF("Enter second string:\n\r");
            SCANF("%99s", password2);
            PRINTF("%d \n\r",atoi(password2));

            uint32_t DWT1, DWT2;
            DWT1 = DWT->CYCCNT;

            // Compare
            status = constantTimeCompare(password1, password2, strlen(password1));

            DWT2 = DWT->CYCCNT;
            PRINTF("\r\nCycles in function: %d\n\r\n", DWT2 - DWT1);

            // Printing status
            if (status) {
                PRINTF("Strings are the same.\n\r");
            } else {
                PRINTF("Strings are not the same.\n\r");
            }
        }
        return 0;
    }
