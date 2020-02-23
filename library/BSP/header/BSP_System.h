/************************************************************
 * File:    BSP_System.h                                    *
 * Author:  Asst.Prof.Dr.Santi Nuratch                      *
 *          Embedded Computing and Control Laboratory       *
 *          ECC-Lab, INC, LMUTT, Thailand                   *
 * Update:  16 February 2020                                *
 ************************************************************/

#ifndef __BSP_SYSTEM__H__

    #define __BSP_SYSTEM__H__

    #include <BSP_Config.h>

    extern inline void System_IsrPerformTick( void );

    /*******************************************************
     * System_TimerInit
     * Initializes the system timer, the Timer1.
     *******************************************************/
    void System_TimerInit(void);

    /*******************************************************
     * System_TimerStart
     * Starts the system ticker, the Timer1.
     *******************************************************/
    void System_TimerStart(void);

    /*******************************************************
     * System_TimerStop
     * Starts the system ticker, the Timer1.
     *******************************************************/
    void System_TimerStop(void);

    /*******************************************************
     * System_SetTickCallback
     * Adds a callback function for the system tick.
     *******************************************************/
    void System_SetTickCallback(callback_t callback);

#endif // __BSP_SYSTEM__H__
