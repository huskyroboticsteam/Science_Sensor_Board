/***************************************************************************//**
* \file Sensor_UART_SPI_UART_INT.c
* \version 4.0
*
* \brief
*  This file provides the source code to the Interrupt Service Routine for
*  the SCB Component in SPI and UART modes.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Sensor_UART_PVT.h"
#include "Sensor_UART_SPI_UART_PVT.h"
#include "cyapicallbacks.h"

#if (Sensor_UART_SCB_IRQ_INTERNAL)
/*******************************************************************************
* Function Name: Sensor_UART_SPI_UART_ISR
****************************************************************************//**
*
*  Handles the Interrupt Service Routine for the SCB SPI or UART modes.
*
*******************************************************************************/
CY_ISR(Sensor_UART_SPI_UART_ISR)
{
#if (Sensor_UART_INTERNAL_RX_SW_BUFFER_CONST)
    uint32 locHead;
#endif /* (Sensor_UART_INTERNAL_RX_SW_BUFFER_CONST) */

#if (Sensor_UART_INTERNAL_TX_SW_BUFFER_CONST)
    uint32 locTail;
#endif /* (Sensor_UART_INTERNAL_TX_SW_BUFFER_CONST) */

#ifdef Sensor_UART_SPI_UART_ISR_ENTRY_CALLBACK
    Sensor_UART_SPI_UART_ISR_EntryCallback();
#endif /* Sensor_UART_SPI_UART_ISR_ENTRY_CALLBACK */

    if (NULL != Sensor_UART_customIntrHandler)
    {
        Sensor_UART_customIntrHandler();
    }

    #if(Sensor_UART_CHECK_SPI_WAKE_ENABLE)
    {
        /* Clear SPI wakeup source */
        Sensor_UART_ClearSpiExtClkInterruptSource(Sensor_UART_INTR_SPI_EC_WAKE_UP);
    }
    #endif

    #if (Sensor_UART_CHECK_RX_SW_BUFFER)
    {
        if (Sensor_UART_CHECK_INTR_RX_MASKED(Sensor_UART_INTR_RX_NOT_EMPTY))
        {
            do
            {
                /* Move local head index */
                locHead = (Sensor_UART_rxBufferHead + 1u);

                /* Adjust local head index */
                if (Sensor_UART_INTERNAL_RX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                if (locHead == Sensor_UART_rxBufferTail)
                {
                    #if (Sensor_UART_CHECK_UART_RTS_CONTROL_FLOW)
                    {
                        /* There is no space in the software buffer - disable the
                        * RX Not Empty interrupt source. The data elements are
                        * still being received into the RX FIFO until the RTS signal
                        * stops the transmitter. After the data element is read from the
                        * buffer, the RX Not Empty interrupt source is enabled to
                        * move the next data element in the software buffer.
                        */
                        Sensor_UART_INTR_RX_MASK_REG &= ~Sensor_UART_INTR_RX_NOT_EMPTY;
                        break;
                    }
                    #else
                    {
                        /* Overflow: through away received data element */
                        (void) Sensor_UART_RX_FIFO_RD_REG;
                        Sensor_UART_rxBufferOverflow = (uint8) Sensor_UART_INTR_RX_OVERFLOW;
                    }
                    #endif
                }
                else
                {
                    /* Store received data */
                    Sensor_UART_PutWordInRxBuffer(locHead, Sensor_UART_RX_FIFO_RD_REG);

                    /* Move head index */
                    Sensor_UART_rxBufferHead = locHead;
                }
            }
            while(0u != Sensor_UART_GET_RX_FIFO_ENTRIES);

            Sensor_UART_ClearRxInterruptSource(Sensor_UART_INTR_RX_NOT_EMPTY);
        }
    }
    #endif


    #if (Sensor_UART_CHECK_TX_SW_BUFFER)
    {
        if (Sensor_UART_CHECK_INTR_TX_MASKED(Sensor_UART_INTR_TX_NOT_FULL))
        {
            do
            {
                /* Check for room in TX software buffer */
                if (Sensor_UART_txBufferHead != Sensor_UART_txBufferTail)
                {
                    /* Move local tail index */
                    locTail = (Sensor_UART_txBufferTail + 1u);

                    /* Adjust local tail index */
                    if (Sensor_UART_TX_BUFFER_SIZE == locTail)
                    {
                        locTail = 0u;
                    }

                    /* Put data into TX FIFO */
                    Sensor_UART_TX_FIFO_WR_REG = Sensor_UART_GetWordFromTxBuffer(locTail);

                    /* Move tail index */
                    Sensor_UART_txBufferTail = locTail;
                }
                else
                {
                    /* TX software buffer is empty: complete transfer */
                    Sensor_UART_DISABLE_INTR_TX(Sensor_UART_INTR_TX_NOT_FULL);
                    break;
                }
            }
            while (Sensor_UART_SPI_UART_FIFO_SIZE != Sensor_UART_GET_TX_FIFO_ENTRIES);

            Sensor_UART_ClearTxInterruptSource(Sensor_UART_INTR_TX_NOT_FULL);
        }
    }
    #endif

#ifdef Sensor_UART_SPI_UART_ISR_EXIT_CALLBACK
    Sensor_UART_SPI_UART_ISR_ExitCallback();
#endif /* Sensor_UART_SPI_UART_ISR_EXIT_CALLBACK */

}

#endif /* (Sensor_UART_SCB_IRQ_INTERNAL) */


/* [] END OF FILE */
