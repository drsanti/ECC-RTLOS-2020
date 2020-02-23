/************************************************************
 * File:    BSP_Led.h                                       *
 * Author:  Asst.Prof.Dr.Santi Nuratch                      *
 *          Embedded Computing and Control Laboratory       *
 *          ECC-Lab, INC, KMUTT, Thailand                   *
 * Update:  16 February 2020                                *
 ************************************************************/


#ifndef __BSP_LED_H__

    #define __BSP_LED_H__

    #include <BSP_Mcu.h>

    /*******************************************************
     * LED IDs
     *******************************************************/
    #define LED_ID_0        0
    #define LED_ID_1        1
    #define LED_ID_2        2
    #define LED_ID_3        3

    /*******************************************************
     * LED IDs
     *******************************************************/
    #define LED_MAX_COUNT   4

    /*******************************************************
     * LED STATUS (ACTIVE LOW)
     *******************************************************/
    #define LED_ON          0
    #define LED_OFF         1

    /*******************************************************
     * LED TRIS DIRECTION
     *******************************************************/
    #define LED_TRIS_OUT    0
    #define LED_TRIS_INP    1

    /*******************************************************
     * LED TRIS
     *******************************************************/
    #define LED0_TRIS   TRISAbits.TRISA2
    #define LED1_TRIS   TRISAbits.TRISA4
    #define LED2_TRIS   TRISBbits.TRISB2
    #define LED3_TRIS   TRISBbits.TRISB3

    /*******************************************************
     * LED LAT (USED FOR WRITING)
     *******************************************************/
    #define LED0_LAT    LATAbits.LATA2
    #define LED1_LAT    LATAbits.LATA4
    #define LED2_LAT    LATBbits.LATB2
    #define LED3_LAT    LATBbits.LATB3

    /*******************************************************
     * LED PORT (USED FOR READING)
     *******************************************************/
    #define LED0_PORT   PORTAbits.RA2
    #define LED1_PORT   PORTAbits.RA4
    #define LED2_PORT   PORTBbits.RB2
    #define LED3_PORT   PORTBbits.RB3


    /*******************************************************
     * LED SET MACROS
     *******************************************************/
    #define Led0_Set() LED0_LAT=LED_ON
    #define Led1_Set() LED1_LAT=LED_ON
    #define Led2_Set() LED2_LAT=LED_ON
    #define Led3_Set() LED3_LAT=LED_ON

     /*******************************************************
     * LEDs CLR MACROS
     *******************************************************/
    #define Led0_Clr() LED0_LAT=LED_OFF
    #define Led1_Clr() LED1_LAT=LED_OFF
    #define Led2_Clr() LED2_LAT=LED_OFF
    #define Led3_Clr() LED3_LAT=LED_OFF

    /*******************************************************
     * LEDs INV MACROS
     *******************************************************/
    #define Led0_Inv() LED0_LAT=!LED0_PORT
    #define Led1_Inv() LED1_LAT=!LED1_PORT
    #define Led2_Inv() LED2_LAT=!LED2_PORT
    #define Led3_Inv() LED3_LAT=!LED3_PORT

    /*******************************************************
     * LEDs GET MACROS
     *******************************************************/
    #define Led0_Get() !LED0_PORT
    #define Led1_Get() !LED1_PORT
    #define Led2_Get() !LED2_PORT
    #define Led3_Get() !LED3_PORT


    /*******************************************************
     * Led_Set
     * Turn on the LED specified by the id.
     * Parameter:
     * - id: Id of the target LED (LED_ID_0, LED_ID_1, LED_ID_2, LED_ID_3).
     *******************************************************/
    void Led_Set(uint8_t id);


    /*******************************************************
     * Led_Clr
     * Turn off the LED specified by the id.
     * Parameter:
     * - id: Id of the target LED (LED_ID_0, LED_ID_1, LED_ID_2, LED_ID_3).
     *******************************************************/
    void Led_Clr(uint8_t id);


    /*******************************************************
     * Led_Inv
     * Inverse or toggle the LED specified by the id.
     * Return the current status of the target LED.
     * Parameter:
     * - id: Id of the target LED (LED_ID_0, LED_ID_1, LED_ID_2, LED_ID_3).
     *******************************************************/
    bool Led_Inv(uint8_t id);


    /*******************************************************
     * Led_Get
     * Return status of the LED (LED_ON or LED_OFF) specified by the id.
     * Parameter:
     * - id: Id of the target LED (LED_ID_0, LED_ID_1, LED_ID_2, LED_ID_3).
     *******************************************************/
    bool Led_Get(uint8_t id);


    /*******************************************************
     * Led_Write
     * Write a byte data to the LEDs (Only 4-bit LSB are taken).
     * Parameter:
     * - data: A byte data (0x00 - 0x0F).
     *******************************************************/
    void Led_Write(uint8_t data);


    /*******************************************************
     * Led_Read
     * Return a byte data of the LEDs (0x00 - 0x0F).
     *******************************************************/
    uint8_t Led_Read(void);

#endif // __BSP_LED_H__
