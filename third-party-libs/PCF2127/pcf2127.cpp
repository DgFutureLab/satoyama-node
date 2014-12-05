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
#include "pcf2127.h"
#include <Wire.h>
#include <SPI.h>

// For handling Arduino 1.0 compatibility and backwards compatibility
#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

/**************************************************************************/
/*!

*/
/**************************************************************************/
PCF2127::PCF2127(uint8_t devAddr, uint8_t mode, uint8_t i2c, uint8_t cs)
{
    uint8_t tmp;

    deviceAddr = devAddr;
    i2cEnb = i2c;
    csPin = cs;
    hourMode = mode;

    if (i2cEnb)
    {
        Wire.begin();
    }
    else
    {
        pinMode(csPin, OUTPUT);
        digitalWrite(csPin, HIGH);
        SPI.begin();
    }

    tmp = 0;
    tmp = (hourMode) ? (tmp | (1<<2)) : (tmp & ~(1<<2)); 
    write(PCF_CONTROL_1, tmp);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
uint8_t PCF2127::bcdEncode(uint8_t val)
{
    return ((val / 10) << 4) | (val % 10);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
uint8_t PCF2127::bcdDecode(uint8_t val)
{
    return ((val >> 4) * 10) + (val & 0x0F);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::writeTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    // make sure no overflow occurs
    if (hours > 24) return;
    if (minutes > 59) return;
    if (seconds > 59) return;

    clkStop(1);
    write(PCF_HOURS, bcdEncode(hours));
    write(PCF_MINUTES, bcdEncode(minutes));
    write(PCF_SECONDS, bcdEncode(seconds));
    clkStop(0);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::readTime(uint8_t *hour, uint8_t *minutes, uint8_t *seconds)
{
    *hour = bcdDecode(read(PCF_HOURS));
    *minutes = bcdDecode(read(PCF_MINUTES));
    *seconds = bcdDecode(read(PCF_SECONDS));
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::writeDate(uint8_t year, uint8_t month, uint8_t day, uint8_t weekday)
{
    // make sure no overflow occurs
    if (year > 99) return;
    if (month > 12) return;
    if (day > 31) return;
    if (weekday > 6) return;

    clkStop(1);
    write(PCF_YEAR, bcdEncode(year));
    write(PCF_MONTH, bcdEncode(month));
    write(PCF_DAY, bcdEncode(day));
    write(PCF_WEEKDAY, weekday);
    clkStop(0);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::readDate(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *weekday)
{
    *year = bcdDecode(read(PCF_YEAR));
    *month = bcdDecode(read(PCF_MONTH));
    *day = bcdDecode(read(PCF_DAY));
    *weekday = bcdDecode(read(PCF_WEEKDAY));
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::timestampEnb(uint8_t pin, uint8_t tsMode)
{
    tsPin = pin;
    pinMode(tsPin, OUTPUT);
    digitalWrite(tsPin, HIGH);

    // enable timestamp and set mode
    readModWriteBit(PCF_TIMESTMP_CTL, BIT_TSOFF, 1);
    readModWriteBit(PCF_TIMESTMP_CTL, BIT_TSM, tsMode);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::timestampTrig()
{
    digitalWrite(tsPin, LOW);
    digitalWrite(tsPin, HIGH);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::readTimestampDate(uint8_t *year, uint8_t *month, uint8_t *day)
{
    *year = bcdDecode(read(PCF_YEAR_TIMESTP));
    *month = bcdDecode(read(PCF_MON_TIMESTP));
    *day = bcdDecode(read(PCF_DAY_TIMESTP));
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::readTimestampTime(uint8_t *hour, uint8_t *minutes, uint8_t *seconds)
{
    *hour = bcdDecode(read(PCF_HOUR_TIMESTP));
    *minutes = bcdDecode(read(PCF_MIN_TIMESTP));
    *seconds = bcdDecode(read(PCF_SEC_TIMESTP));
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::alarmEnb(uint8_t almTimeEnb, uint8_t almDayEnb, uint8_t almWeekdayEnb)
{
    if (almTimeEnb)
    {
        write(PCF_HOUR_ALM, 1<<7);
        write(PCF_MINUTE_ALM, 1<<7);
        write(PCF_SECOND_ALM, 1<<7);
    }

    if (almDayEnb)
    {
        write(PCF_DAY_ALM, 1<<7);
    }

    if (almWeekdayEnb)
    {
        write(PCF_WEEKDAY_ALM, 1<<7);
    }

    // enable alarm interrupt enable
    readModWriteBit(PCF_CONTROL_2, BIT_AIE, 1);    
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::alarmWriteTime(uint8_t hour, uint8_t minutes, uint8_t seconds)
{
    write(PCF_HOUR_ALM, hour);
    write(PCF_MINUTE_ALM, minutes);
    write(PCF_SECOND_ALM, seconds);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::alarmWriteDate(uint8_t day, uint8_t weekday)
{
    write(PCF_DAY_ALM, day);
    write(PCF_WEEKDAY_ALM, weekday);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::clkStop(uint8_t enb)
{
    enb &= 0x1;
    readModWriteBit(PCF_CONTROL_1, BIT_STOP, enb);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::secondsIntpEnb(uint8_t enb)
{
    enb &= 0x1;
    readModWriteBit(PCF_CONTROL_1, BIT_SI, enb);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::secondsIntpClr()
{
    readModWriteBit(PCF_CONTROL_2, BIT_MSF, 0);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::minutesIntpEnb(uint8_t enb)
{
    enb &= 0x1;
    readModWriteBit(PCF_CONTROL_1, BIT_MI, enb);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::minutesIntpClr()
{
    readModWriteBit(PCF_CONTROL_2, BIT_MSF, 0);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::timerInit(uint8_t mode, uint8_t src)
{
    uint8_t val = mode << 6;
    val |= src & 0x3;
    write(PCF_WATCHDOG_TIM_CTL, val);

    // set timer intp enb
    readModWriteBit(PCF_CONTROL_2, BIT_CDTIE, 1);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::timerLoad(uint8_t val)
{
    write(PCF_WATCHDOG_TIM_VAL, val);

    // clear timer interrupt
    readModWriteBit(PCF_CONTROL_2, BIT_CDTF, 0);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void PCF2127::write(uint8_t addr, uint8_t data)
{
    if (i2cEnb)
    {
        // i2c transactions should be atomic in case there are more than one
        // device on i2c bus and irqs are being used
        cli();
        Wire.beginTransmission(deviceAddr);
        Wire.write(addr);
        Wire.write(data);
        Wire.endTransmission();
        sei();
    }
    else
    {
        uint8_t val = 0;
        val |= SPI_SUBADDR;
        val |= addr;

        // spi transactions should be atomic in case there are more than one device
        // on the spi bus and irqs are being used.
        cli();
        digitalWrite(csPin, LOW);
        SPI.transfer(val);
        SPI.transfer(data);
        digitalWrite(csPin, HIGH);
        sei();
    }
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
uint8_t PCF2127::read(uint8_t addr)
{
    uint8_t indata;

    if (i2cEnb)
    {
        cli();
        Wire.beginTransmission(deviceAddr);
        Wire.write(addr);
        Wire.endTransmission();
    
        Wire.requestFrom(deviceAddr, 1);
        while (!Wire.available())
        {
            // spin here until data is available
            ;
        }

        indata = Wire.read();
        sei();
    }
    else
    {
        uint8_t val = 0;
        val |= SPI_READ;
        val |= SPI_SUBADDR;
        val |= addr;

        // spi transactions should be atomic in case there are more than one device
        // on the spi bus and irqs are being used.
        cli();
        digitalWrite(csPin, LOW);
        SPI.transfer(val);
        indata = SPI.transfer(0);
        digitalWrite(csPin, HIGH);
        sei();
    }
    return indata;
}

/**************************************************************************/
/*!
    read modify write one bit in a register
*/
/**************************************************************************/
void PCF2127::readModWriteBit(uint8_t addr, uint8_t pos, uint8_t val)
{
    uint8_t tmp = read(addr);

    if (val)
    {
        tmp |= 1<<pos;
    }
    else
    {
        tmp &= ~(1<<pos);
    }
    write(addr, tmp);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
//ISR(PCF2127_INTP)
//{
//}
