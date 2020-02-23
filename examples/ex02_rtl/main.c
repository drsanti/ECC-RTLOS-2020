/************************************************************
 * Example 2. Using the RTL, BSP and SYS                    *
 ************************************************************
 * File:    main.c                                          *
 * Description:                                             *
 *          Demonstrates how to use the RTL, BSP and SYS .  *
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
 *   2) Change value of the TARGET_EXAMPLE to 2.
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
    timer_event_t *te = (timer_event_t *)event;                     // Get the timer event.
    Uart1_Printf( "TId: %i, TCnt: %d\r\n", te->id, te->counter);    // Print the information.
}

/************************************************************
 * Main function
 * Intitializes all peripherals and modules.
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
    Uart1_Printf("Example2: Using the RTL.\r\n");   // Prints a message.


    /*********************************
     * 3. CREATE SOFTWARE TIMERS
     *********************************/
    Timer_Create(1000,  Timer_Callback);            // Create a software timer.
    Timer_Create(1500,  Timer_Callback);            // Create a software timer.

    /*********************************
     * 4. START THE SYSTEM
     *********************************/
    System_Start();                                 // Starts the system.

    return 0;
}
