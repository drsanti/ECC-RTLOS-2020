/************************************************************
 * Example 3. Using the RTOS, BSP, and TRL libraries        *
 ************************************************************
 * File:    main.c                                          *
 * Description:                                             *
 *          Demonstrates how to use the RTOS, BSP and RTL.  *
 *          (Please see closely in the app.h and config.cfg)*
 * Author:  Asst.Prof.Dr.Santi Nuratch                      *
 *          Embedded Computing and Control Laboratory       *
 *          ECC-Lab, INC, KMUTT, Thailand                   *
 ************************************************************
 * Update:  18 February 2020 (Initial)                      *
 * Update:  23 February 2020                                *
 *          Use the app.h to include/exclude modules.       *
 ************************************************************/

/************************************************************
 * Instuction:
 *   1) Open the library/ECC/app.h.
 *   2) Change value of the TARGET_EXAMPLE to 3.
 ************************************************************/


/************************************************************
 * Include the application header.
 ************************************************************/
#include <app.h>

/************************************************************
 * Include the system header.
 ************************************************************/
#include <ecc.h>


/************************************************************
 * Callback function of the software timers.
 ************************************************************/
void Timer_Callback(void *event) {
    timer_event_t *te = (timer_event_t *)event; // Get the timer event.
    Uart1_Printf( "Timer: %i, Counter: %d\r\n", te->id, te->counter);   // Prints the information.
    Led_Inv(te->id);    // Toggle the LED spedified by the te->id
}


/************************************************************
 * Keys (PSW0,..., PSW3) changed callback function.
 ************************************************************/
void KeyChanged_Callback(void *event) {
    switch_event_t *evt = (switch_event_t *)event;   // Get the switch event.
    Uart1_Printf( "id: %d, stateCode: 0x%.2X, stateName: %s\r\n", evt->id, evt->state, evt->sname);    // Print the information.

    if( evt->state == PSW_STATE_DOWN ) {
        Beep_Play( 500 + (evt->id) * 300, 50 );
    }
}


/************************************************************
 * Uarts (UART1 & UART2) ISR byte received callback function.
 * This function will be called direcly from the UART ISR.
 ************************************************************/
void Uarts_RxCallback(void *event) {
    Uart1_Printf( "recv-std: %c\r\n", ((uart_event_t *)event)->byte);
}


/************************************************************
 * Uarts (UART1 & UART2) byte received callback function.
 * This function will be called from the internal BSP executor.
 ************************************************************/
void Uarts_RxIsrCallback(void *event) {
    Uart1_Printf( "recv-isr: %c\r\n", ((uart_event_t *)event)->byte);
}


/************************************************************
 * Main function
 * Intitializes all peripherals and softwares modules.
 ************************************************************/
int main(void) {

    /*********************************
     * 1. INITIALIZE THE SYSTEM
     *********************************/
    System_Init();                                  // Initializes system modules.


    /*********************************
     * 2. UART1
     *********************************/
    Uart1_Init(115200, 64, 128);                    // Initializes Uart1.
    Uart1_SetRxIsrCallback(Uarts_RxIsrCallback);    // Sets Rx ISR callback function of the Uart1.
    Uart1_SetRxCallback(Uarts_RxCallback);          // Sets callback function of the Uart1 Rx.
    Uart1_Printf("Example 3: Using the RTOS, BSP and RTL.\r\n");   // Prints a message.


    /*********************************
     * 3. Create Software Timers
     *********************************/
    Timer_Create(200,  Timer_Callback);             // Create a software timer.
    Timer_Create(500,  Timer_Callback);             // Create a software timer.
    Timer_Create(700,  Timer_Callback);             // Create a software timer.
    Timer_Create(900,  Timer_Callback);             // Create a software timer.


    /*********************************
     * 5. SWITCHES
     *********************************/
    Psw_SetKeyChangedCallback(PSW_ID_0, KeyChanged_Callback);   // Add callback function of PSW0.
    Psw_SetKeyChangedCallback(PSW_ID_1, KeyChanged_Callback);   // Add callback function of PSW1.
    Psw_SetKeyChangedCallback(PSW_ID_2, KeyChanged_Callback);   // Add callback function of PSW2.
    Psw_SetKeyChangedCallback(PSW_ID_3, KeyChanged_Callback);   // Add callback function of PSW3.


    /*********************************
     * 6. START THE SYSTEM
     *********************************/
    System_Start();                                 // Starts the system.

    return 0;
}
