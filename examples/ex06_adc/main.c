/************************************************************
 * Example 6. ADCs                                          *
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
 *   2) Change value of the TARGET_EXAMPLE to 6.
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
 * RTOS Task used to read all channels of ADCs.
 ************************************************************/
void ADC_TaskCode( void * pvParameters ) {
    for( ;; ) {
        Uart1_Printf("TSK: %3d %3d %3d %3d\r\n", Adc_Get(0), Adc_Get(1), Adc_Get(2), Adc_Get(3));
        Led_Set(LED_ID_0); vTaskDelayMs(50);
        Led_Clr(LED_ID_0); vTaskDelayMs(950);
    }
}

/************************************************************
 * Callback function (OnChanedCallback), called when value is changed.
 ************************************************************/
void ADC_Changed( void *event ) {

    adc_event_t *evt = (adc_event_t *)event;

    if( evt->direction > 0 ) {
        Beep_Play(1200, 50);    // High frequency beep
        Led_Set(LED_ID_1);

    }
    else {
        Beep_Play(500, 50);     // Low frequency beep
        Led_Clr(LED_ID_1);
    }

    // Note:
    //   Be careful to use the Uart1_Printf() with out critical section protection.
    Uart1_Printf("CBK: %d %4d %3d (%c)\r\n", evt->id, evt->value, evt->delta, evt->direction>0?'+':'-');
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
    Uart1_Printf("Example 6: ADCs\r\n");

    /*********************************
     * 3. Changed Callbacks
     *********************************/
    Adc_SetChangedCallback(ADC_ID_0, ADC_Changed);  // Add callback function to channel 0
    Adc_SetChangedCallback(ADC_ID_1, ADC_Changed);  // Add callback function to channel 1
    Adc_SetChangedCallback(ADC_ID_2, ADC_Changed);  // Add callback function to channel 2
    Adc_SetChangedCallback(ADC_ID_3, ADC_Changed);  // Add callback function to channel 3

    /*********************************
     * 4. Changed Interval and Threahold
     *    ADC1 connected to LDR.
     *********************************/
    Adc_SetChangedInterval(ADC_ID_1, 5);    // 5mS
    Adc_SetChangedThreshold(ADC_ID_1, 20);  // Threahold value


    /************************************************
     * 5. Create task (If the task uses printf(), or
     * complex methematic operations, the stack size
     * should be larg enough, e.e., 256 or 512 bytes
     ************************************************/
    xTaskCreate(ADC_TaskCode, "ADC", 512, NULL, tskIDLE_PRIORITY, NULL);

    /*********************************
     * 6. START THE SYSTEM
     *********************************/
    System_Start();                                 // Starts the system.

    return 0;
}
