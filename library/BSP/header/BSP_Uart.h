/************************************************************
 * File:    BSP_Uart.h                                      *
 * Author:  Asst.Prof.Dr.Santi Nuratch                      *
 *          Embedded Computing and Control Laboratory       *
 *          ECC-Lab, INC, LMUTT, Thailand                   *
 * Update:  16 February 2020                                *
 ************************************************************/

#ifndef __BSP_UART_H__

    #define __BSP_UART_H__

    #include <BSP_Config.h>
    #include <BSP_Queue.h>


    /*******************************************************
     * UART IDs
     *******************************************************/
    #define UART_ID_1   1
    #define UART_ID_2   2


    /*******************************************************
     * U1 TX&RX ISRs CONTROL
     *******************************************************/
    #define UART1_TX_ISR_ENABLE()       (IEC0bits.U1TXIE = 1)
    #define UART1_TX_ISR_DISABLE()      (IEC0bits.U1TXIE = 0)
    #define UART1_RX_ISR_ENABLE()       (IEC0bits.U1RXIE = 1)
    #define UART1_RX_ISR_DISABLE()      (IEC0bits.U1RXIE = 0)



    /*******************************************************
     * U2 TX&RX ISRs CONTROL
     *******************************************************/
    #define UART2_TX_ISR_ENABLE()       (IEC1bits.U2TXIE = 1)
    #define UART2_TX_ISR_DISABLE()      (IEC1bits.U2TXIE = 0)
    #define UART2_RX_ISR_ENABLE()       (IEC1bits.U2RXIE = 1)
    #define UART2_RX_ISR_DISABLE()      (IEC1bits.U2RXIE = 0)


    /*******************************************************
     * UART1 TX&RX BUFFER LENGTH
     *******************************************************/
    //#define U1TX_BUFF_LENGTH    256
    //#define U1RX_BUFF_LENGTH    128

    /*******************************************************
     * UART1 TX&RX BUFFER LENGTH
     *******************************************************/
    //#define U2TX_BUFF_LENGTH    256
    //#define U2RX_BUFF_LENGTH    128




    /*******************************************************
     * UARTx BUFFER LENGTH for Uartx_Printf
     *******************************************************/
    #define PRINTF_BUFF_LENGTH  256 //!! Dont touch this value



    /*******************************************************
     * UART EVENT TYPES
     *******************************************************/
    #define EVT_UART_RX_ISR     0
    #define EVT_UART_TX_ISR     1
    #define EVT_UART_RX_STD     2
    #define EVT_UART_TX_STD     3




    /*******************************************************
     * UART OBJECT STRUCTURE
     *******************************************************/
    typedef struct {

        int         id;             // Id of the UART (UART_ID_1 or UART_ID_2).
        char        std_rxd;        // Rx data of queue buffer.
        char        isr_rxd;        // Rx data of Rx ISR.
        char        isr_txd;        // Tx data of Tx ISR.
        bool        txemp;          // Tx buffer empty frag.
        Queue       *rxqueue;       // Rx queue object.
        Queue       *txqueue;       // Tx queue object.

        callback_t  rxd_isr_cbk;    // Rx ISR callback performed inside the Rx ISR.
        callback_t  txd_isr_cbk;    // Tx ISR callback performed inside the Tx ISR.

        callback_t  rxd_std_cbk;    // Rx standard callback performed outside the Rx ISR.
        callback_t  txd_std_cbk;    // Tx standard callback performed outside the Tx ISR.

    }uart_t;


    /*******************************************************
     * UART EVENT OBJECT STRUCTURE
     *******************************************************/
    typedef struct {
        int     type;       // Uart event type.
        int     id;         // Id of the UART (UART_ID_1 or UART_ID_2).
        char    byte;       // Received byte data.
        char    *string;    // Received string data (line).
        uart_t  *sender;    // Uart object.
    }uart_event_t;


    /*******************************************************
     * Uart1_Init
     * Initializes Uart1.
     * Parameter:
     * - baurate: Baudrate of the Uart1.
     * - rxBuffLength: RX buffer size in bytes.
     * - txBuffLength: TX buffer size in bytes.
     *******************************************************/
    void Uart1_Init(uint32_t baurate, uint16_t rxBuffLength, uint16_t txBuffLength);


    /*******************************************************
     * Uart2_Init
     * Initializes Uart2.
     * Parameter:
     * - baurate: Baudrate of the Uart2.
     * - rxBuffLength: RX buffer size in bytes.
     * - txBuffLength: TX buffer size in bytes.
     *******************************************************/
    void Uart2_Init(uint32_t baurate, uint16_t rxBuffLength, uint16_t txBuffLength);


    /*******************************************************
     * Uart_Init
     * Initializes the Uart specified by the id.
     * Parameter:
     * - id: Uart id (UART_ID_1 or UART_ID_2).
     * - baurate: Baudrate of the Uart1.
     * - rxBuffLength: RX buffer size in bytes.
     * - txBuffLength: TX buffer size in bytes.
     *******************************************************/
    void Uart_Init(int id, uint32_t baurate, uint16_t rxBuffLength, uint16_t txBuffLength);

    /*******************************************************
     * Uart1_Put
     * Put a byte data into the Uart1 TX register.
     * It waits until the previous byte is sent before puts the new one into the TX register.
     * Parameter:
     * - data: A byte data.
     *******************************************************/
    void Uart1_Put(char data);

    /*******************************************************
     * Uart2_Put
     * Put a byte data into the Uart2 register.
     * It waits until the previous byte is sent before puts the new one into the TX register.
     * Parameter:
     * - data: A byte data.
     *******************************************************/
    void Uart2_Put(char data);

    /*******************************************************
     * Uart1_Get
     * Get a byte of data from Uart1 RX register.
     * It waits until a byte is received and writes the received byte to the output variable (*data).
     * Parameter:
     * - data: Output byte data.
     *******************************************************/
    void Uart1_Get(char *data);

    /*******************************************************
     * Uart2_Get
     * Get a byte of data from Uart2 RX register.
     * It waits until a byte is received and writes the received byte to the output variable (*data).
     * Parameter:
     * - data: Output byte data.
     *******************************************************/
    void Uart2_Get(char *data);



    /*******************************************************
     * Uart1_Write
     * Write a string (array of characters) to the Uart1.
     * It waits until all bytes are sent.
     * Parameter:
     * - string: Array of characters.
     *******************************************************/
    void Uart1_Write(const char *string);

    /*******************************************************
     * Uart2_Write
     * Write a string (array of characters) to the Uart2.
     * It waits until all bytes are sent.
     * Parameter:
     * - string: Array of characters.
     *******************************************************/
    void Uart2_Write(const char *string);


    /*******************************************************
     * Uart1_PutAsync
     * Asynchronously put a byte data into the Uart1 TX register.
     * It puts the byte data into TX queue and returns immediately.
     * Returns zero if the TX queue is full.
     * Parameter:
     * - data: A byte data.
     *******************************************************/
    uint16_t Uart1_PutAsync(char data);


    /*******************************************************
     * Uart2_PutAsync
     * Asynchronously put a byte data into the Uart2 TX register.
     * It puts the byte data into TX queue and returns immediately.
     * Returns zero if the TX queue is full.
     * Parameter:
     * - data: A byte data.
     *******************************************************/
    uint16_t Uart2_PutAsync(char data);


    /*******************************************************
     * Uart1_WriteAsync
     * Asynchronously write a string (array of characters) to the Uart1.
     * It writes all bytes into TX queue and returns immediately.
     * Parameter:
     * - string: Array of characters.
     *******************************************************/
    uint16_t Uart1_WriteAsync(const char *string);

    /*******************************************************
     * Uart2_WriteAsync
     * Asynchronously write a string (array of characters) to the Uart2.
     * It writes all bytes into TX queue and returns immediately.
     * Parameter:
     * - string: Array of characters.
     *******************************************************/
    uint16_t Uart2_WriteAsync(const char *string);


    /*******************************************************
     * Uart1_Printf
     * Asynchronously prints a formatted string to Uart1.
     * Parameter:
     * - format: The formatted string.
     * - ...: Additional parameters used to create the string.
     *******************************************************/
    void Uart1_Printf(const char *format, ...);

    /*******************************************************
     * Uart2_Printf
     *  Asynchronously prints a formatted string to Uart2.
     * Parameter:
     * - format: The formatted string.
     * - ...: Additional parameters used to create the string.
     *******************************************************/
    void Uart2_Printf(const char *format, ...);


    /*******************************************************
     * Uart_Printf
     * Asynchronously prints a formatted string to the Uart specified by the id.
     * Parameter:
     * - id: Uart id (UART_ID_1 or UART_ID_2)
     * - format: The formatted string.
     * - ...: Additional parameters used to create the string.
     *******************************************************/
    void Uart_Printf(int id, const char *format, ...);



    //
    // ISR CALLBACKS
    //


    /*******************************************************
     * Uart1_SetRxIsrCallback
     * Set ISR callback function the Uart1 byte received.
     * Parameter:
     * - callback: Callback function of the Uart1 byte received.
     *******************************************************/
    void Uart1_SetRxIsrCallback(callback_t callback);

    /*******************************************************
     * Uart2_SetRxIsrCallback
     * Set ISR callback function the Uart2 byte received.
     * Parameter:
     * - callback: Callback function of the Uart2 byte received.
     *******************************************************/
    void Uart2_SetRxIsrCallback(callback_t callback);

    /*******************************************************
     * Uart1_SetTxIsrCallback
     * Set ISR callback function the Uart1 byte transmitted.
     * Parameter:
     * - callback: Callback function of the Uart1 byte transmitted.
     *******************************************************/
    void Uart1_SetTxIsrCallback(callback_t callback);

    /*******************************************************
     * Uart2_SetTxIsrCallback
     * Set ISR callback function the Uart2 byte transmitted.
     * Parameter:
     * - callback: Callback function of the Uart2 byte transmitted.
     *******************************************************/
    void Uart2_SetTxIsrCallback(callback_t callback);


    //
    // STD CALLBACKS
    //

    /*******************************************************
     * Uart1_SetRxCallback
     * Set callback function the Uart1 byte received.
     * Parameter:
     * - callback: Callback function of the Uart1 byte received.
     *******************************************************/
    void Uart1_SetRxCallback(callback_t callback);

    /*******************************************************
     * Uart2_SetRxCallback
     * Set callback function the Uart2 byte received.
     * Parameter:
     * - callback: Callback function of the Uart2 byte received.
     *******************************************************/
    void Uart2_SetRxCallback(callback_t callback);

    /*******************************************************
     * Uart1_SetTxCallback
     * Set callback function the Uart1 byte transmitted.
     * Parameter:
     * - callback: Callback function of the Uart1 byte transmitted.
     *******************************************************/
    void Uart1_SetTxCallback(callback_t callback);

    /*******************************************************
     * Uart2_SetTxCallback
     * Set callback function the Uart2 byte transmitted.
     * Parameter:
     * - callback: Callback function of the Uart2 byte transmitted.
     *******************************************************/
    void Uart2_SetTxCallback(callback_t callback);



    /*******************************************************
     * Uart1_Executor
     * Performs the Rx callback function of Uart1.
     * This function is called by the BSP_Executor.
     *******************************************************/
    inline void Uart1_Executor(void);

    /*******************************************************
     * Uart2_Executor
     * Performs the Rx callback function of Uart2.
     * This function is called by the BSP_Executor.
     *******************************************************/
    inline void Uart2_Executor(void);



#endif // __BSP_UART_H__
