/************************************************************
 * Example 5. PWM-based beep sound generator                *
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
 *   2) Change value of the TARGET_EXAMPLE to 5.
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
 * Simple RTOS Task
 ************************************************************/
void Beep_TaskCode( void * pvParameters ) {
    for(;;) {
        Led_Inv(LED_ID_0);
        vTaskDelayMs(500);
    }
}

/************************************************************
 * Beep Ending Callback (Play notes)
 ************************************************************/
void Beep_Callback( void *event ) {

    const float Notes[] = {
        BEEP_Do, BEEP_Re, BEEP_Me, BEEP_Fa,
        BEEP_So, BEEP_La, BEEP_Si, BEEP_DO
    };

    beep_event_t *evt = (beep_event_t *)event;  // Get event.

    uint16_t idx = evt->counter%8;  // Index (the counter is increased by 1 evey time the callback is performed)
    float freq   = Notes[idx]*1.0;  // Frequency
    Beep_Play(freq, 200 + idx*10);  // Play the target note.

    // Note:
    //   This function requires a lot of requorces and activates Tx ISR.
    //   To avoid the stack overflow, it should be called at the last line,
    //   or apply it in the PERFORM_CRITICAL_SECTION()
    Uart1_Printf("idx: %d, freq: %4.2f\r\n", idx, freq);
}


/************************************************************
 * Main function
 * Intitializes all peripherals and modules.
 ************************************************************/
int main(void) {

    /*********************************
     * 1. INITIALIZE THE SYSTEM
     *********************************/
    System_Init();

    /*********************************
     * 2. UART1
     *********************************/
    Uart1_Init(115200, 128, 128);
    Uart1_Printf("Example 5: Beep Sound\r\n");

    /*********************************
     * 3. BEEP
     *********************************/
    Beep_SetCallback(Beep_Callback);

    /************************************************
     * 4. Create task (If the task uses printf(), or
     * complex methematic operations, the stack size
     * should be larg enough, e.e., 256 or 512 bytes
     ************************************************/
    xTaskCreate(Beep_TaskCode, "Beep", 512, NULL, tskIDLE_PRIORITY, NULL);

    /*********************************
     * 8. START THE SYSTEM
     *********************************/
    System_Start();                                 // Starts the system.

    return 0;
}
