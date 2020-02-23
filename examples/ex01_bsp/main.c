/************************************************************
 * Example 1. Using the BSP with the BSP System Tick        *
 ************************************************************
 * File:    main.c                                          *
 * Description:                                             *
 *          Demonstrates how to use the BSP with the SYS.   *
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
 *   2) Change value of the TARGET_EXAMPLE to 1.
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
 * Keys (PSW0,..., PSW3) changed callback function.
 ************************************************************/
void KeyChanged_Callback( void *event ) {
    switch_event_t *evt = (switch_event_t *)event;
    Uart1_Printf( "id: %d, stateCode: 0x%.2X, stateName: %s\r\n", evt->id, evt->state, evt->sname );
    if( evt->state == PSW_STATE_DOWN ) {
        Led_Inv(evt->id);
        Beep_Play(500 + (400*evt->id), 100);
    }
}


/************************************************************
 * Uarts (UART1 & UART2) ISR byte received callback function.
 * This function will be called direcly from the UART ISR.
 ************************************************************/
void Uarts_RxIsrCallback(void *event) {
    uart_event_t *evt = (uart_event_t *)event;                          // Get the uart isr event.
    Uart_Printf(evt->id, "UART%i-ISR: %c\r\n", evt->id, evt->byte);     // Print the information.

    Led_Flash(LED_ID_2, 200);
}


/************************************************************
 * Uarts (UART1 & UART2) byte received callback function.
 * This function will be called from the internal BSP executor.
 ************************************************************/
void Uarts_RxCallback(void *event) {
    uart_event_t *ue = (uart_event_t *)event;                           // Get the uart event.
    Uart_Printf(ue->id, "UART%i-STD: %c\r\n", ue->id, ue->byte);        // Print the information.

    Led_Flash(LED_ID_3, 200);
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
    Uart1_Init(115200, 64, 128);                    // Initializes Uart1 with the given baoudrate and buffer sizes of the RX and TX.
    Uart1_SetRxIsrCallback(Uarts_RxIsrCallback);    // Sets Rx ISR callback function of the Uart1.
    Uart1_SetRxCallback(Uarts_RxCallback);          // Sets callback function of the Uart1 Rx.
    Uart1_Printf("UART1, Example1: Using the BSP.\r\n");

    // Note: Tx callback functions of the Uart1 can be used if required.
    // Uart1_SetTxIsrCallback(Uarts_TxIsrCallback);
    // Uart1_SetTxCallback(Uarts_TxCallback);

    /*********************************
     * 3. UART2
     *********************************/
    Uart2_Init(115200, 64, 128);                    // Initializes Uart2 with the given baoudrate and buffer sizes of the RX and TX.
    Uart2_SetRxIsrCallback(Uarts_RxIsrCallback);    // Sets Rx ISR callback function of the Uart2.
    Uart2_SetRxCallback(Uarts_RxCallback);          // Sets callback function of the Uart2 Rx.
    Uart2_Printf("UART2, Example1: Using the BSP.\r\n");        // Prints a message to the Uart2.

    // Note: Tx callback functions of the Uart2 can be used if required.
    // Uart2_SetTxIsrCallback(Uarts_TxIsrCallback);
    // Uart2_SetTxCallback(Uarts_TxCallback);


    /*********************************
     * 4. SWITCHES
     *********************************/
    Psw_SetKeyChangedCallback(PSW_ID_0, KeyChanged_Callback);   // Add callback function of PSW0.
    Psw_SetKeyChangedCallback(PSW_ID_1, KeyChanged_Callback);   // Add callback function of PSW1.
    Psw_SetKeyChangedCallback(PSW_ID_2, KeyChanged_Callback);   // Add callback function of PSW2.
    Psw_SetKeyChangedCallback(PSW_ID_3, KeyChanged_Callback);   // Add callback function of PSW3.

    /*********************************
     * 5. START THE SYSTEM
     *********************************/
    System_Start();                                 // Starts the system.

    return 0;
}
