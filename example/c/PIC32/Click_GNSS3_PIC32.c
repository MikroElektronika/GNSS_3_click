/*
Example for GNSS3 Click

    Date          : jan 2019.
    Author        : Katarina Perendic

Test configuration PIC32 :
    
    MCU                : P32MX795F512L
    Dev. Board         : EasyPIC Fusion v7
    PIC32 Compiler ver : v4.0.0.0

---

Description :

The application is composed of three sections :

- System Initialization - Initializes all necessary GPIO pins, UART used for
the communcation with GPS module and UART used for infromation logging
- Application Initialization - Initializes driver, power on module
- Application Task - running in parallel core state machine.
If response detected parser will hang information of Latitude, Longitude and Altitude .

Additional Functions :

All additional functions such as timer initialization and default handler. 

Notes :

- The GPS module returns data in the form of NMEA responses
- For example, we used the GGA response

- $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47

For parsing, use the GPS Parser function to send the following form of arguments:
The name of the NMEA response that you want to parse, the position of the data that you need.
As a response - you will get a separate buffer with the requested data

*/

#include "Click_GNSS3_types.h"
#include "Click_GNSS3_config.h"
#include "Click_GNSS3_timer.h"

uint8_t pFlag = 0;
uint8_t dispFlag = 0;
char demoBuffer[ 500 ] = {0};
char demoCommand[ 16 ] = "$GPGGA";

void gnss3_default_handler( uint8_t *rsp, uint8_t *evArgs )
{
    char *tmp;
    tmp = rsp;

    mikrobus_logWrite( tmp, _LOG_TEXT );
    if(*rsp == '$')
    {
        memset(&demoBuffer[0], 0, 500);
        strcpy(demoBuffer, tmp);
    }
}

void systemInit()
{
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_RST_PIN, _GPIO_OUTPUT );
    mikrobus_uartInit( _MIKROBUS1, &_GNSS3_UART_CFG[0] );
    mikrobus_logInit( _LOG_USBUART_B, 9600 );
    mikrobus_logWrite(" ---- System Init ---- ", _LOG_LINE);
}

void applicationInit()
{
// TIMER INIT
    gnss3_configTimer();

// DRIVER INIT
    gnss3_uartDriverInit((T_GNSS3_P)&_MIKROBUS1_GPIO, (T_GNSS3_P)&_MIKROBUS1_UART);
    gnss3_coreInit( gnss3_default_handler, 1500 );

// MODULE POWER ON
    gnss3_hfcEnable( 1 );
    gnss3_modulePower( 1 );

    Delay_ms( 5000 );
    timerCounter = 0;
}

void applicationTask()
{
    char *pLat;
    char *pLong;
    char *pAlt;
    char rspCom[ 50 ] = {0};
        
// CORE STATE MACHINE
    gnss3_process();

    if(timerCounter > 5000)
    {
        pFlag++;
        if(pFlag > 2)
        {
            pFlag = 0;
        }
        timerCounter = 0;
        dispFlag = 1;
    }

    if(pFlag == 0 && dispFlag == 1)
    {
        mikrobus_logWrite("  ", _LOG_LINE);
        mikrobus_logWrite( " ---------------------------------------- ", _LOG_LINE);
        pLat = gnss3_parser(&demoBuffer[0], &demoCommand[0], 2);

        if(pLat == 0)
        {
            mikrobus_logWrite( " Latitude : No data available!", _LOG_LINE);
        }
        else
        {
            strcpy(&rspCom[0], pLat);
            mikrobus_logWrite( " Latitude : ", _LOG_TEXT);
            mikrobus_logWrite(rspCom, _LOG_LINE);
        }
        dispFlag = 0;
        mikrobus_logWrite( " ---------------------------------------- ", _LOG_LINE);
    }

    if(pFlag == 2 &&  dispFlag == 1)
    {
        mikrobus_logWrite("  ", _LOG_LINE);
        mikrobus_logWrite( " ---------------------------------------- ", _LOG_LINE);
        pAlt = gnss3_parser(&demoBuffer[0], &demoCommand[0], 9);
        if(pAlt == 0)
        {
            mikrobus_logWrite( " Altitude : No data available!", _LOG_LINE);
        }
        else
        {
            strcpy(&rspCom[0], pAlt);
            mikrobus_logWrite( " Altitude : ", _LOG_TEXT);
            mikrobus_logWrite(rspCom, _LOG_LINE);
        }
        dispFlag = 0;
        mikrobus_logWrite( " ---------------------------------------- ", _LOG_LINE);
    }

    if(pFlag == 1 && dispFlag == 1)
    {
        mikrobus_logWrite("  ", _LOG_LINE);
        mikrobus_logWrite( " ---------------------------------------- ", _LOG_LINE);
        pLong = gnss3_parser(&demoBuffer[0], &demoCommand[0], 4);
        if(pLong == 0)
        {
            mikrobus_logWrite( " Longitude : No data available!", _LOG_LINE);
        }
        else
        {
            strcpy(&rspCom[0], pLong);
            mikrobus_logWrite( " Longitude : ", _LOG_TEXT);
            mikrobus_logWrite(rspCom, _LOG_LINE);
        }
        dispFlag = 0;
        mikrobus_logWrite( " ---------------------------------------- ", _LOG_LINE);
    }
}

void main()
{
    systemInit();
    applicationInit();

    while (1)
    {
        applicationTask();
    }
}

/* -------------------------------------------------------------------------- */