/*******************************************************************
    Copyright (C) 2012 FreakLabs
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.

    Originally written by Christopher Wang aka Akiba.
    Please post support questions to the FreakLabs forum.

*******************************************************************/
/*!
    \file 
    \ingroup


*/
/**************************************************************************/
#pragma once

#include <stdint.h>
#include <SPI.h>

// define interrupt here
#define PCF2127_INTP INT0_vect

#define SPI_READ    0x80
#define SPI_WRITE   0x00
#define SPI_SUBADDR 0x20

/**************************************************************************/
/*!
    PCF2127AT Registers
*/
/**************************************************************************/
enum
{
    PCF_CONTROL_1           = 0x00,
    BIT_EXT_TEST            = 7,  
    BIT_STOP                = 5,
    BIT_TSF_1               = 4,
    BIT_POR_OVRD            = 3,
    BIT_12_24               = 2,
    BIT_MI                  = 1,
    BIT_SI                  = 0,

    PCF_CONTROL_2           = 0x01,
    BIT_MSF                 = 7,
    BIT_WDTF                = 6,
    BIT_TSF2                = 5,
    BIT_AF                  = 4,
    BIT_CDTF                = 3,
    BIT_TSIE                = 2,
    BIT_AIE                 = 1,
    BIT_CDTIE               = 0,

    PCF_CONTROL_3           = 0x02,
    FIELD3_PWRMNG           = 5,
    BIT_BTSE                = 4,
    BIT_BF                  = 3,
    BIT_BLF                 = 2,
    BIT_BIE                 = 1,
    BIT_BLIE                = 0,

    PCF_SECONDS             = 0x03,
    PCF_MINUTES             = 0x04,
    PCF_HOURS               = 0x05,
    PCF_DAY                 = 0x06,
    PCF_WEEKDAY             = 0x07,
    PCF_MONTH               = 0x08,
    PCF_YEAR                = 0x09,
    PCF_SECOND_ALM          = 0x0A,
    PCF_MINUTE_ALM          = 0x0B,
    PCF_HOUR_ALM            = 0x0C,
    PCF_DAY_ALM             = 0x0D,
    PCF_WEEKDAY_ALM         = 0x0E,
    PCF_CLKOUT_CTL          = 0x0F,
    PCF_WATCHDOG_TIM_CTL    = 0x10,
    PCF_WATCHDOG_TIM_VAL    = 0x11,

    PCF_TIMESTMP_CTL        = 0x12,
    BIT_TSM                 = 7,
    BIT_TSOFF               = 6,
    FIELD5_1_O_16           = 0,

    PCF_SEC_TIMESTP         = 0x13,
    PCF_MIN_TIMESTP         = 0x14,
    PCF_HOUR_TIMESTP        = 0x15,
    PCF_DAY_TIMESTP         = 0x16,
    PCF_MON_TIMESTP         = 0x17,
    PCF_YEAR_TIMESTP        = 0x18,
    PCF_AGING_OFFSET        = 0x19,
    PCF_RAM_ADDR_MSB        = 0x1A,
    PCF_RAM_ADDR_LSB        = 0x1B,
    PCF_RAM_WRT_CMD         = 0x1C,
    PCF_RAM_RD_CMD          = 0x1D
};

class PCF2127
{
public:
    enum
    {
        JANUARY     = 0,
        FEBRUARY    = 1,
        MARCH       = 2, 
        APRIL       = 3,
        MAY         = 4,
        JUNE        = 5, 
        JULY        = 6,
        AUGUST      = 7, 
        SEPTEMBER   = 8,
        OCTOBER     = 9,
        NOVEMBER    = 10,    
        DECEMBER    = 11
    };

    enum
    {
        SUNDAY      = 0,
        MONDAY      = 1,
        TUESDAY     = 2,
        WEDNESDAY   = 3,
        THURSDAY    = 4,
        FRIDAY      = 5,
        SATURDAY    = 6
    };

    PCF2127(uint8_t devAddr, uint8_t mode, uint8_t i2c, uint8_t cs);
    void writeTime(uint8_t hour, uint8_t minutes, uint8_t seconds);
    void readTime(uint8_t *hour, uint8_t *minutes, uint8_t *seconds);
    void writeDate(uint8_t year, uint8_t month, uint8_t day, uint8_t weekday);
    void readDate(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *weekday);

    void timestampEnb(uint8_t pin, uint8_t tsMode = 0);
    void timestampTrig();
    void readTimestampDate(uint8_t *year, uint8_t *month, uint8_t *day);
    void readTimestampTime(uint8_t *hour, uint8_t *minutes, uint8_t *seconds);

    void alarmEnb(uint8_t almTimeEnb, uint8_t almDayEnb, uint8_t almWeekdayEnb = 0);
    void alarmWriteDate(uint8_t day, uint8_t weekday = 0);
    void alarmWriteTime(uint8_t hour, uint8_t minutes, uint8_t seconds);

    void timerInit(uint8_t mode, uint8_t src);
    void timerLoad(uint8_t val);

    void clkStop(uint8_t enb);
    void secondsIntpEnb(uint8_t enb);
    void secondsIntpClr();
    void minutesIntpEnb(uint8_t enb);
    void minutesIntpClr();

    void write(uint8_t addr, uint8_t data);
    uint8_t read(uint8_t addr);
    void readModWriteBit(uint8_t addr, uint8_t pos, uint8_t val);
    uint8_t bcdEncode(uint8_t val);
    uint8_t bcdDecode(uint8_t val);

private:
    int deviceAddr;
    uint8_t i2cEnb;
    uint8_t csPin;
    uint8_t tsPin;
    uint8_t hourMode;
};
