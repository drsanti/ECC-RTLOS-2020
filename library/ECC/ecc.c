/************************************************************
 * File:    ecc.c (system core)                             *
 * Description:                                             *
 *          Demonstrates how to use the RTOS, BSP and RTL.  *
 *          (Please see closely in the app.h and config.cfg)*
 * Author:  Asst.Prof.Dr.Santi Nuratch                      *
 *          Embedded Computing and Control Laboratory       *
 *          ECC-Lab, INC, LMUTT, Thailand                   *
 ************************************************************
 * Update:  23 February 2020                                *
 ************************************************************/

#include <ecc.h>

#if ECC_SYSTEM_USE_RTOS  >  0

/************************************************************
 * Hardware timer (TIMER1) ISR
 ************************************************************/
void __attribute__((__interrupt__, auto_psv)) _T1Interrupt/*configTICK_INTERRUPT_HANDLER*/( void ) {
	IFS0bits.T1IF = 0;
	if( xTaskIncrementTick() != pdFALSE ) {
		portYIELD();
	}
}


/************************************************************
 * RTOS vApplicationIdleHook.
 ************************************************************/
void vApplicationIdleHook( void ) {
    /*********************************
     * vCoRoutineSchedule
     *********************************/
    vCoRoutineSchedule();
}


/************************************************************
 * RTOS vApplicationTickHook.
 ************************************************************/
void vApplicationTickHook( void ) {

    /*********************************
     * BSP_TickIsrExecutor
     *********************************/
    BSP_TickIsrExecutor();  // Executes the BSP tick.

    /*********************************
     * RTL_TickIsrExecutor
     *********************************/
	#if ECC_SYSTEM_USE_RTL > 0
       RTL_TickIsrExecutor();  // Executes the RTL tick.
	#endif
}


/************************************************************
 * RTOS vApplicationStackOverflowHook.
 ************************************************************/
void vApplicationStackOverflowHook (TaskHandle_t pxTask, char *pcTaskName ) {
    (void)pcTaskName;  (void)pxTask;
	Uart1_Write("\r\nApplication Stack Overflow!\r\n");
    taskDISABLE_INTERRUPTS();
    Led_Write(0x03);
    while (1);
}


/************************************************************
 * CoRoutine executed by FreeRTOS.
 ************************************************************/
void System_CoRoutine( CoRoutineHandle_t xHandle, UBaseType_t uxIndex ) {
    crSTART(xHandle);   	// Start co-routine
    for(;;) {
        BSP_Executor(); 	// Executes the BSP module.
        #if ECC_SYSTEM_USE_RTL > 0
	    	RTL_Executor();	// Executes the RTL module.
		#endif
    }
    crEND();            	// End co-routine
}


/************************************************************
 * Create a CoRoutine for BSP_Executor and RTL_Executor.
 ************************************************************/
void System_StartCoRutine( void ) {
    /************************************************
     * Create a CoRoutine
     ************************************************/
    xCoRoutineCreate( System_CoRoutine, tskIDLE_PRIORITY, 0 );
}
#else


/*******************************************************
 * ISR of Timer1 (System timer)
 *******************************************************/
void __attribute__((interrupt, auto_psv)) _T1Interrupt( void ) {
    IFS0bits.T1IF  = 0;

    /*********************************
     * System_IsrPerformTick
     *********************************/
	System_IsrPerformTick();

    /*********************************
     * BSP_TickIsrExecutor
     *********************************/
	BSP_TickIsrExecutor();

    /*********************************
     * RTL_TickIsrExecutor
     *********************************/
	#if ECC_SYSTEM_USE_RTL > 0
        RTL_TickIsrExecutor();
	#endif
}
#endif
