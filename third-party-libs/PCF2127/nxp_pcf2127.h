/*
 *  PCF2127A (Integrated RTC, TCXO and quartz crystal) demo 
 *  (library)
 *
 *  PCF2127A is a "real time clock (RTC)" module which is including a Xtal and TCXO 
 *  http://www.nxp.com/pip/PCF2127A_2.html
 *
 *  This is a just simple operation sample of the PCF2127A.
 *  In this sample, the PCF2127A is interfaced by I2C through pin9 and 10 of mbed. 
 *  And also the mbed-pin8 is connected to RTC's /INT pin. 
 *  The RTC chip is set to generate periodical interrupt in every seconds. 
 *  This interrupt triggers the update of the terminal and LCD screen. 
 *  
 *  RTC initializing part is ported from...
 *    http://mbed.org/users/roen/notebook/real-time/
 *
 *  Released under the MIT License: http://mbed.org/license/mit
 *
 *  revision 1.0    05-Jun-2010     (a) 1st release
 *  revision 1.1    05-Jun-2010     (a) class name changed
 *                              
 */


#ifndef        MBED_NXP_PCF2127A
#define        MBED_NXP_PCF2127A


#include    "mbed.h"


//  PCF2127A IIC address
#define     PCF2127A_addr       0xA2

//  PCF2127A registers
#define     Control_1           0x00
#define     Control_2           0x01
#define     Control_3           0x02
#define     Seconds             0x03
#define     Minutes             0x04
#define     Hours               0x05
#define     Days                0x06
#define     Weekdays            0x07
#define     Months              0x08
#define     Years               0x09
#define     Second_alarm        0x0A
#define     Minute_alarm        0x0B
#define     Hour_alarm          0x0C
#define     Day_alarm           0x0D
#define     Weekday_alarm       0x0E
#define     CLKOUT_ctl          0x0F
#define     Watchdg_tim_ctl     0x10
#define     Watchdg_tim_val     0x11
#define     Timestp_ctl         0x12
#define     Sec_timestp         0x13
#define     Min_timestp         0x14
#define     Hour_timestp        0x15
#define     Day_timestp         0x16
#define     Mon_timestp         0x17
#define     Year_timestp        0x18
#define     Aging_offset        0x19

class NXP_PCF2127A {
public:

    NXP_PCF2127A(
        PinName sda,
        PinName sdl,
        char dev_address = PCF2127A_addr,
        char vControl_1 = 0x03,
        char vControl_2 = 0x00,
        char vControl_3 = 0x60
    )
            : i2c( sda, sdl ), device_address( dev_address ) {
        set_register( Control_1, vControl_1 );
        set_register( Control_2, vControl_2 );
        set_register( Control_3, vControl_3 );
    }

    ~NXP_PCF2127A() {
    }

    int is_init_required( void ) {
        return ( read_register( Seconds ) & 0x80 ? 1 : 0 );
    }

    void set_time( void ) {
        struct tm   dt, *dtp;
        char        buf[ 8 ];
        char        c;

        dtp = &dt;

//#define TIME_SETTING_TEST
#ifndef TIME_SETTING_TEST
        printf("Enter current date and time:\r\n");
        printf("YYYY MM DD HH MM SS[enter]\r\n");
        scanf("%d %d %d %d %d %d", &(dtp->tm_year), &(dtp->tm_mon), &(dtp->tm_mday), &(dtp->tm_hour), &(dtp->tm_min), &(dtp->tm_sec) );
        printf("%d/%d/%d - %d:%d:%d\r\n", (dtp->tm_year), (dtp->tm_mon), (dtp->tm_mday), (dtp->tm_hour), (dtp->tm_min), (dtp->tm_sec) );

#else
        dtp->tm_year    = 2010;
        dtp->tm_mon     = 12;
        dtp->tm_mday    = 31;
        dtp->tm_hour    = 23;
        dtp->tm_min     = 59;
        dtp->tm_sec     = 50;

#endif

        // adjust for tm structure required values
        dtp->tm_year = dtp->tm_year - 1900;
        dtp->tm_mon  = dtp->tm_mon - 1;

        buf[ 0 ]    = Seconds;
        buf[ 1 ]    = i2bcd( dtp->tm_sec  );
        buf[ 2 ]    = i2bcd( dtp->tm_min  );
        buf[ 3 ]    = i2bcd( dtp->tm_hour );
        buf[ 4 ]    = i2bcd( dtp->tm_mday );
        buf[ 5 ]    = i2bcd( dtp->tm_wday );
        buf[ 6 ]    = i2bcd( dtp->tm_mon  + 1   );
        buf[ 7 ]    = i2bcd( dtp->tm_year - 100 );

        c    = read_register( Seconds );
        while ( c == read_register( Seconds ) )
            ;

        i2c.write( device_address, buf, 8 );
    }


    time_t time( time_t *tp ) {
        struct tm   dt, *dtp;
        time_t      t;
        char        buf[ 8 ]    = { Seconds };

        dtp = &dt;

        i2c.write( device_address, buf, 1 );
        i2c.read( device_address, buf, 7 );

        dtp->tm_sec     = bcd2i( buf[ 0 ] );
        dtp->tm_min     = bcd2i( buf[ 1 ] );
        dtp->tm_hour    = bcd2i( buf[ 2 ] );
        dtp->tm_mday    = bcd2i( buf[ 3 ] );
        dtp->tm_wday    = bcd2i( buf[ 4 ] );
        dtp->tm_mon     = bcd2i( buf[ 5 ] ) - 1;
        dtp->tm_year    = bcd2i( buf[ 6 ] ) + 100;

        t   = mktime( dtp );

        if ( tp )
            *tp  = t;

        return( t );
    }

    void set_alarm( char addr, char s ) {
        char    v;

        v   = i2bcd( s );
        set_register( addr, v );
    }

    void clear_intr( void ) {
        set_register( Control_2, 0x00 );
    }

private:
    I2C     i2c;
    char    device_address;

    void set_register( char addr, char data ) {
        char    b[ 2 ];

        b[ 0 ]    = addr;
        b[ 1 ]    = data;

        i2c.write( device_address, b, 2 );
    }

    char read_register( char addr ) {
        char    data;

        data    = addr;
        i2c.write( device_address, &data, 1 );
        i2c.read( device_address, &data, 1 );

        return ( data );
    }

    char i2bcd( char n ) {
        return ( ((n / 10) << 4) | (n % 10) );
    }

    char bcd2i( char bcd ) {
        return ( ((bcd >> 4) * 10) + (bcd & 0x0F) );
    }
}
;
#endif  // end of "#ifndef MBED_NXP_PCF2127A"
