/************************************************************
 * Example 7. Blinker                                       *
 ************************************************************
 * File:    main.c                                          *
 * Description:                                             *
 *          Demonstrates how to use the RTOS, BSP and RTL.  *
 *          (Please see closely in the app.h and config.cfg)*
 * Author:  Asst.Prof.Dr.Santi Nuratch                      *
 *          Embedded Computing and Control Laboratory       *
 *          ECC-Lab, INC, LMUTT, Thailand                   *
 ************************************************************
 * Update:  18 February 2020 (Initial)                      *
 * Update:  23 February 2020                                *
 *          Use the app.h to include/exclude modules.       *
 ************************************************************/

/************************************************************
 * Instuction:
 *   1) Open the library/ECC/app.h.
 *   2) Change value of the TARGET_EXAMPLE to 7.
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
 * RTOS Task calls the Flashers and Blinkers
 ************************************************************/
void Blink_Flash_TaskCode( void * pvParameters ) {

    for(;;) {

        /****************************************************
         * Flash (Uncomment to check the result)
         ****************************************************/
        /*
        Led_Flash(LED_ID_0, 100);
        Led_Flash(LED_ID_1, 200);
        Led_Flash(LED_ID_2, 300);
        Led_Flash(LED_ID_3, 400);
        */

        /****************************************************
         * Blink (Uncomment to check the result)
         ****************************************************/
        /*
        Led_Blink(LED_ID_0, 100, 10);
        Led_Blink(LED_ID_1, 200, 10);
        Led_Blink(LED_ID_2, 300, 10);
        Led_Blink(LED_ID_3, 400, 10);
        */

        vTaskDelayMs(1000);
    }
}


/************************************************************
 * State Changed Callback for all LEDs
 ************************************************************/
void LED_Callback(void *event) {

    led_event_t *evt = (led_event_t *)event;

    if(evt->state == LED_STATE_BEGIN) {
        //Beep_Play(500+((evt->counter*20)%1000), 20+((evt->counter*4)%200));
    }

    // Note:
    //   Be careful to use the Uart1_Printf() with out critical section protection.
    Uart1_Printf("led: %d, sta: 0x%.2X, cnt: %d\r\n", evt->id, evt->state, evt->counter);
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

    /*******************************************************
     * 2. UART1
     *******************************************************/
    Uart1_Init(115200, 128, 128);
    Uart1_Printf("Example 8: Flasher\r\n");

    /*******************************************************
     * 3. LED Blinking with PWM
     *******************************************************/
    Led_Pwm(LED_ID_0, 0,   10, 500);
    Led_Pwm(LED_ID_1, 100, 10, 500);
    Led_Pwm(LED_ID_2, 200, 10, 500);
    Led_Pwm(LED_ID_3, 300, 10, 500);

    /*******************************************************
     * 4. State changed callbacks
     *******************************************************/
    Led_SetChangedCallback(LED_ID_0, LED_Callback);
    Led_SetChangedCallback(LED_ID_1, LED_Callback);
    Led_SetChangedCallback(LED_ID_2, LED_Callback);
    Led_SetChangedCallback(LED_ID_3, LED_Callback);


    /************************************************
     * 5. Create task (If the task uses printf(), or
     * complex methematic operations, the stack size
     * should be larg enough, e.e., 256 or 512 bytes
     ************************************************/
    xTaskCreate(Blink_Flash_TaskCode, "Blink", 512, NULL, tskIDLE_PRIORITY, NULL);

    /*********************************
     * 6. START THE SYSTEM
     *********************************/
    System_Start();                                 // Starts the system.

    return 0;
}
