/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include <stdint.h>
#include "xparameters.h"
#include "xil_io.h"

#define FIFO_BASE  0x43C10000
#define FPGA_REV_REG   (FIFO_BASE + 0x00)
#define FIFO_RST_REG   (FIFO_BASE + 0x04)
#define WR_DATA_REG    (FIFO_BASE + 0x08)
#define RD_REQ_REG     (FIFO_BASE + 0x0C)
#define RD_DATA_REG    (FIFO_BASE + 0x10)
#define STATUS_REG     (FIFO_BASE + 0x14)
#define DATA_COUNT_REG (FIFO_BASE + 0x18)
#define CONFIG_REG     (FIFO_BASE + 0x1C)
#define THRESHOLD_REG  (FIFO_BASE + 0x20)
int main() {
    // Read FPGA revision
    xil_printf("FPGA REV: 0x%08X\n\r", Xil_In32(FPGA_REV_REG));

    // Reset FIFO
    Xil_Out32(FIFO_RST_REG, 1);

    // Push data
    for (int i = 1; i <= 5; i++) {
        Xil_Out32(WR_DATA_REG, i * 10);
        xil_printf("Pushed: %d\n\r", i * 10);
    }

    xil_printf("Count: %d\n\r", Xil_In32(DATA_COUNT_REG));

    // Pop data
    for (int i = 0; i < 5; i++) {
        Xil_Out32(RD_REQ_REG, 1);
        Xil_In32(STATUS_REG);
        xil_printf("Read: 0x%08X\n\r", Xil_In32(RD_DATA_REG));
    }

    xil_printf("\n\r--- Register Dump ---\n\r");
    xil_printf("FPGA REV:   0x%08X\n\r", Xil_In32(FPGA_REV_REG));
    xil_printf("STATUS:     0x%08X\n\r", Xil_In32(STATUS_REG));
    xil_printf("CONFIG:     0x%08X\n\r", Xil_In32(CONFIG_REG));
    xil_printf("THRESHOLD:  0x%08X\n\r", Xil_In32(THRESHOLD_REG));
    return 0;
}
