/***************************************************************************//**
* \file Sensor_UART_UART.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  UART mode.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Sensor_UART_PVT.h"
#include "Sensor_UART_SPI_UART_PVT.h"
#include "cyapicallbacks.h"

#if (Sensor_UART_UART_WAKE_ENABLE_CONST && Sensor_UART_UART_RX_WAKEUP_IRQ)
    /**
    * \addtogroup group_globals
    * \{
    */
    /** This global variable determines whether to enable Skip Start
    * functionality when Sensor_UART_Sleep() function is called:
    * 0 – disable, other values – enable. Default value is 1.
    * It is only available when Enable wakeup from Deep Sleep Mode is enabled.
    */
    uint8 Sensor_UART_skipStart = 1u;
    /** \} globals */
#endif /* (Sensor_UART_UART_WAKE_ENABLE_CONST && Sensor_UART_UART_RX_WAKEUP_IRQ) */

#if(Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    const Sensor_UART_UART_INIT_STRUCT Sensor_UART_configUart =
    {
        Sensor_UART_UART_SUB_MODE,
        Sensor_UART_UART_DIRECTION,
        Sensor_UART_UART_DATA_BITS_NUM,
        Sensor_UART_UART_PARITY_TYPE,
        Sensor_UART_UART_STOP_BITS_NUM,
        Sensor_UART_UART_OVS_FACTOR,
        Sensor_UART_UART_IRDA_LOW_POWER,
        Sensor_UART_UART_MEDIAN_FILTER_ENABLE,
        Sensor_UART_UART_RETRY_ON_NACK,
        Sensor_UART_UART_IRDA_POLARITY,
        Sensor_UART_UART_DROP_ON_PARITY_ERR,
        Sensor_UART_UART_DROP_ON_FRAME_ERR,
        Sensor_UART_UART_WAKE_ENABLE,
        0u,
        NULL,
        0u,
        NULL,
        Sensor_UART_UART_MP_MODE_ENABLE,
        Sensor_UART_UART_MP_ACCEPT_ADDRESS,
        Sensor_UART_UART_MP_RX_ADDRESS,
        Sensor_UART_UART_MP_RX_ADDRESS_MASK,
        (uint32) Sensor_UART_SCB_IRQ_INTERNAL,
        Sensor_UART_UART_INTR_RX_MASK,
        Sensor_UART_UART_RX_TRIGGER_LEVEL,
        Sensor_UART_UART_INTR_TX_MASK,
        Sensor_UART_UART_TX_TRIGGER_LEVEL,
        (uint8) Sensor_UART_UART_BYTE_MODE_ENABLE,
        (uint8) Sensor_UART_UART_CTS_ENABLE,
        (uint8) Sensor_UART_UART_CTS_POLARITY,
        (uint8) Sensor_UART_UART_RTS_POLARITY,
        (uint8) Sensor_UART_UART_RTS_FIFO_LEVEL,
        (uint8) Sensor_UART_UART_RX_BREAK_WIDTH
    };


    /*******************************************************************************
    * Function Name: Sensor_UART_UartInit
    ****************************************************************************//**
    *
    *  Configures the Sensor_UART for UART operation.
    *
    *  This function is intended specifically to be used when the Sensor_UART
    *  configuration is set to “Unconfigured Sensor_UART” in the customizer.
    *  After initializing the Sensor_UART in UART mode using this function,
    *  the component can be enabled using the Sensor_UART_Start() or
    * Sensor_UART_Enable() function.
    *  This function uses a pointer to a structure that provides the configuration
    *  settings. This structure contains the same information that would otherwise
    *  be provided by the customizer settings.
    *
    *  \param config: pointer to a structure that contains the following list of
    *   fields. These fields match the selections available in the customizer.
    *   Refer to the customizer for further description of the settings.
    *
    *******************************************************************************/
    void Sensor_UART_UartInit(const Sensor_UART_UART_INIT_STRUCT *config)
    {
        uint32 pinsConfig;

        if (NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Get direction to configure UART pins: TX, RX or TX+RX */
            pinsConfig  = config->direction;

        #if !(Sensor_UART_CY_SCBIP_V0 || Sensor_UART_CY_SCBIP_V1)
            /* Add RTS and CTS pins to configure */
            pinsConfig |= (0u != config->rtsRxFifoLevel) ? (Sensor_UART_UART_RTS_PIN_ENABLE) : (0u);
            pinsConfig |= (0u != config->enableCts)      ? (Sensor_UART_UART_CTS_PIN_ENABLE) : (0u);
        #endif /* !(Sensor_UART_CY_SCBIP_V0 || Sensor_UART_CY_SCBIP_V1) */

            /* Configure pins */
            Sensor_UART_SetPins(Sensor_UART_SCB_MODE_UART, config->mode, pinsConfig);

            /* Store internal configuration */
            Sensor_UART_scbMode       = (uint8) Sensor_UART_SCB_MODE_UART;
            Sensor_UART_scbEnableWake = (uint8) config->enableWake;
            Sensor_UART_scbEnableIntr = (uint8) config->enableInterrupt;

            /* Set RX direction internal variables */
            Sensor_UART_rxBuffer      =         config->rxBuffer;
            Sensor_UART_rxDataBits    = (uint8) config->dataBits;
            Sensor_UART_rxBufferSize  =         config->rxBufferSize;

            /* Set TX direction internal variables */
            Sensor_UART_txBuffer      =         config->txBuffer;
            Sensor_UART_txDataBits    = (uint8) config->dataBits;
            Sensor_UART_txBufferSize  =         config->txBufferSize;

            /* Configure UART interface */
            if(Sensor_UART_UART_MODE_IRDA == config->mode)
            {
                /* OVS settings: IrDA */
                Sensor_UART_CTRL_REG  = ((0u != config->enableIrdaLowPower) ?
                                                (Sensor_UART_UART_GET_CTRL_OVS_IRDA_LP(config->oversample)) :
                                                (Sensor_UART_CTRL_OVS_IRDA_OVS16));
            }
            else
            {
                /* OVS settings: UART and SmartCard */
                Sensor_UART_CTRL_REG  = Sensor_UART_GET_CTRL_OVS(config->oversample);
            }

            Sensor_UART_CTRL_REG     |= Sensor_UART_GET_CTRL_BYTE_MODE  (config->enableByteMode)      |
                                             Sensor_UART_GET_CTRL_ADDR_ACCEPT(config->multiprocAcceptAddr) |
                                             Sensor_UART_CTRL_UART;

            /* Configure sub-mode: UART, SmartCard or IrDA */
            Sensor_UART_UART_CTRL_REG = Sensor_UART_GET_UART_CTRL_MODE(config->mode);

            /* Configure RX direction */
            Sensor_UART_UART_RX_CTRL_REG = Sensor_UART_GET_UART_RX_CTRL_MODE(config->stopBits)              |
                                        Sensor_UART_GET_UART_RX_CTRL_POLARITY(config->enableInvertedRx)          |
                                        Sensor_UART_GET_UART_RX_CTRL_MP_MODE(config->enableMultiproc)            |
                                        Sensor_UART_GET_UART_RX_CTRL_DROP_ON_PARITY_ERR(config->dropOnParityErr) |
                                        Sensor_UART_GET_UART_RX_CTRL_DROP_ON_FRAME_ERR(config->dropOnFrameErr)   |
                                        Sensor_UART_GET_UART_RX_CTRL_BREAK_WIDTH(config->breakWidth);

            if(Sensor_UART_UART_PARITY_NONE != config->parity)
            {
               Sensor_UART_UART_RX_CTRL_REG |= Sensor_UART_GET_UART_RX_CTRL_PARITY(config->parity) |
                                                    Sensor_UART_UART_RX_CTRL_PARITY_ENABLED;
            }

            Sensor_UART_RX_CTRL_REG      = Sensor_UART_GET_RX_CTRL_DATA_WIDTH(config->dataBits)       |
                                                Sensor_UART_GET_RX_CTRL_MEDIAN(config->enableMedianFilter) |
                                                Sensor_UART_GET_UART_RX_CTRL_ENABLED(config->direction);

            Sensor_UART_RX_FIFO_CTRL_REG = Sensor_UART_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(config->rxTriggerLevel);

            /* Configure MP address */
            Sensor_UART_RX_MATCH_REG     = Sensor_UART_GET_RX_MATCH_ADDR(config->multiprocAddr) |
                                                Sensor_UART_GET_RX_MATCH_MASK(config->multiprocAddrMask);

            /* Configure RX direction */
            Sensor_UART_UART_TX_CTRL_REG = Sensor_UART_GET_UART_TX_CTRL_MODE(config->stopBits) |
                                                Sensor_UART_GET_UART_TX_CTRL_RETRY_NACK(config->enableRetryNack);

            if(Sensor_UART_UART_PARITY_NONE != config->parity)
            {
               Sensor_UART_UART_TX_CTRL_REG |= Sensor_UART_GET_UART_TX_CTRL_PARITY(config->parity) |
                                                    Sensor_UART_UART_TX_CTRL_PARITY_ENABLED;
            }

            Sensor_UART_TX_CTRL_REG      = Sensor_UART_GET_TX_CTRL_DATA_WIDTH(config->dataBits)    |
                                                Sensor_UART_GET_UART_TX_CTRL_ENABLED(config->direction);

            Sensor_UART_TX_FIFO_CTRL_REG = Sensor_UART_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(config->txTriggerLevel);

        #if !(Sensor_UART_CY_SCBIP_V0 || Sensor_UART_CY_SCBIP_V1)
            Sensor_UART_UART_FLOW_CTRL_REG = Sensor_UART_GET_UART_FLOW_CTRL_CTS_ENABLE(config->enableCts) | \
                                            Sensor_UART_GET_UART_FLOW_CTRL_CTS_POLARITY (config->ctsPolarity)  | \
                                            Sensor_UART_GET_UART_FLOW_CTRL_RTS_POLARITY (config->rtsPolarity)  | \
                                            Sensor_UART_GET_UART_FLOW_CTRL_TRIGGER_LEVEL(config->rtsRxFifoLevel);
        #endif /* !(Sensor_UART_CY_SCBIP_V0 || Sensor_UART_CY_SCBIP_V1) */

            /* Configure interrupt with UART handler but do not enable it */
            CyIntDisable    (Sensor_UART_ISR_NUMBER);
            CyIntSetPriority(Sensor_UART_ISR_NUMBER, Sensor_UART_ISR_PRIORITY);
            (void) CyIntSetVector(Sensor_UART_ISR_NUMBER, &Sensor_UART_SPI_UART_ISR);

            /* Configure WAKE interrupt */
        #if(Sensor_UART_UART_RX_WAKEUP_IRQ)
            CyIntDisable    (Sensor_UART_RX_WAKE_ISR_NUMBER);
            CyIntSetPriority(Sensor_UART_RX_WAKE_ISR_NUMBER, Sensor_UART_RX_WAKE_ISR_PRIORITY);
            (void) CyIntSetVector(Sensor_UART_RX_WAKE_ISR_NUMBER, &Sensor_UART_UART_WAKEUP_ISR);
        #endif /* (Sensor_UART_UART_RX_WAKEUP_IRQ) */

            /* Configure interrupt sources */
            Sensor_UART_INTR_I2C_EC_MASK_REG = Sensor_UART_NO_INTR_SOURCES;
            Sensor_UART_INTR_SPI_EC_MASK_REG = Sensor_UART_NO_INTR_SOURCES;
            Sensor_UART_INTR_SLAVE_MASK_REG  = Sensor_UART_NO_INTR_SOURCES;
            Sensor_UART_INTR_MASTER_MASK_REG = Sensor_UART_NO_INTR_SOURCES;
            Sensor_UART_INTR_RX_MASK_REG     = config->rxInterruptMask;
            Sensor_UART_INTR_TX_MASK_REG     = config->txInterruptMask;

            /* Configure TX interrupt sources to restore. */
            Sensor_UART_IntrTxMask = LO16(Sensor_UART_INTR_TX_MASK_REG);

            /* Clear RX buffer indexes */
            Sensor_UART_rxBufferHead     = 0u;
            Sensor_UART_rxBufferTail     = 0u;
            Sensor_UART_rxBufferOverflow = 0u;

            /* Clear TX buffer indexes */
            Sensor_UART_txBufferHead = 0u;
            Sensor_UART_txBufferTail = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: Sensor_UART_UartInit
    ****************************************************************************//**
    *
    *  Configures the SCB for the UART operation.
    *
    *******************************************************************************/
    void Sensor_UART_UartInit(void)
    {
        /* Configure UART interface */
        Sensor_UART_CTRL_REG = Sensor_UART_UART_DEFAULT_CTRL;

        /* Configure sub-mode: UART, SmartCard or IrDA */
        Sensor_UART_UART_CTRL_REG = Sensor_UART_UART_DEFAULT_UART_CTRL;

        /* Configure RX direction */
        Sensor_UART_UART_RX_CTRL_REG = Sensor_UART_UART_DEFAULT_UART_RX_CTRL;
        Sensor_UART_RX_CTRL_REG      = Sensor_UART_UART_DEFAULT_RX_CTRL;
        Sensor_UART_RX_FIFO_CTRL_REG = Sensor_UART_UART_DEFAULT_RX_FIFO_CTRL;
        Sensor_UART_RX_MATCH_REG     = Sensor_UART_UART_DEFAULT_RX_MATCH_REG;

        /* Configure TX direction */
        Sensor_UART_UART_TX_CTRL_REG = Sensor_UART_UART_DEFAULT_UART_TX_CTRL;
        Sensor_UART_TX_CTRL_REG      = Sensor_UART_UART_DEFAULT_TX_CTRL;
        Sensor_UART_TX_FIFO_CTRL_REG = Sensor_UART_UART_DEFAULT_TX_FIFO_CTRL;

    #if !(Sensor_UART_CY_SCBIP_V0 || Sensor_UART_CY_SCBIP_V1)
        Sensor_UART_UART_FLOW_CTRL_REG = Sensor_UART_UART_DEFAULT_FLOW_CTRL;
    #endif /* !(Sensor_UART_CY_SCBIP_V0 || Sensor_UART_CY_SCBIP_V1) */

        /* Configure interrupt with UART handler but do not enable it */
    #if(Sensor_UART_SCB_IRQ_INTERNAL)
        CyIntDisable    (Sensor_UART_ISR_NUMBER);
        CyIntSetPriority(Sensor_UART_ISR_NUMBER, Sensor_UART_ISR_PRIORITY);
        (void) CyIntSetVector(Sensor_UART_ISR_NUMBER, &Sensor_UART_SPI_UART_ISR);
    #endif /* (Sensor_UART_SCB_IRQ_INTERNAL) */

        /* Configure WAKE interrupt */
    #if(Sensor_UART_UART_RX_WAKEUP_IRQ)
        CyIntDisable    (Sensor_UART_RX_WAKE_ISR_NUMBER);
        CyIntSetPriority(Sensor_UART_RX_WAKE_ISR_NUMBER, Sensor_UART_RX_WAKE_ISR_PRIORITY);
        (void) CyIntSetVector(Sensor_UART_RX_WAKE_ISR_NUMBER, &Sensor_UART_UART_WAKEUP_ISR);
    #endif /* (Sensor_UART_UART_RX_WAKEUP_IRQ) */

        /* Configure interrupt sources */
        Sensor_UART_INTR_I2C_EC_MASK_REG = Sensor_UART_UART_DEFAULT_INTR_I2C_EC_MASK;
        Sensor_UART_INTR_SPI_EC_MASK_REG = Sensor_UART_UART_DEFAULT_INTR_SPI_EC_MASK;
        Sensor_UART_INTR_SLAVE_MASK_REG  = Sensor_UART_UART_DEFAULT_INTR_SLAVE_MASK;
        Sensor_UART_INTR_MASTER_MASK_REG = Sensor_UART_UART_DEFAULT_INTR_MASTER_MASK;
        Sensor_UART_INTR_RX_MASK_REG     = Sensor_UART_UART_DEFAULT_INTR_RX_MASK;
        Sensor_UART_INTR_TX_MASK_REG     = Sensor_UART_UART_DEFAULT_INTR_TX_MASK;

        /* Configure TX interrupt sources to restore. */
        Sensor_UART_IntrTxMask = LO16(Sensor_UART_INTR_TX_MASK_REG);

    #if(Sensor_UART_INTERNAL_RX_SW_BUFFER_CONST)
        Sensor_UART_rxBufferHead     = 0u;
        Sensor_UART_rxBufferTail     = 0u;
        Sensor_UART_rxBufferOverflow = 0u;
    #endif /* (Sensor_UART_INTERNAL_RX_SW_BUFFER_CONST) */

    #if(Sensor_UART_INTERNAL_TX_SW_BUFFER_CONST)
        Sensor_UART_txBufferHead = 0u;
        Sensor_UART_txBufferTail = 0u;
    #endif /* (Sensor_UART_INTERNAL_TX_SW_BUFFER_CONST) */
    }
#endif /* (Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: Sensor_UART_UartPostEnable
****************************************************************************//**
*
*  Restores HSIOM settings for the UART output pins (TX and/or RTS) to be
*  controlled by the SCB UART.
*
*******************************************************************************/
void Sensor_UART_UartPostEnable(void)
{
#if (Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG)
    #if (Sensor_UART_TX_SDA_MISO_PIN)
        if (Sensor_UART_CHECK_TX_SDA_MISO_PIN_USED)
        {
            /* Set SCB UART to drive the output pin */
            Sensor_UART_SET_HSIOM_SEL(Sensor_UART_TX_SDA_MISO_HSIOM_REG, Sensor_UART_TX_SDA_MISO_HSIOM_MASK,
                                           Sensor_UART_TX_SDA_MISO_HSIOM_POS, Sensor_UART_TX_SDA_MISO_HSIOM_SEL_UART);
        }
    #endif /* (Sensor_UART_TX_SDA_MISO_PIN_PIN) */

    #if !(Sensor_UART_CY_SCBIP_V0 || Sensor_UART_CY_SCBIP_V1)
        #if (Sensor_UART_RTS_SS0_PIN)
            if (Sensor_UART_CHECK_RTS_SS0_PIN_USED)
            {
                /* Set SCB UART to drive the output pin */
                Sensor_UART_SET_HSIOM_SEL(Sensor_UART_RTS_SS0_HSIOM_REG, Sensor_UART_RTS_SS0_HSIOM_MASK,
                                               Sensor_UART_RTS_SS0_HSIOM_POS, Sensor_UART_RTS_SS0_HSIOM_SEL_UART);
            }
        #endif /* (Sensor_UART_RTS_SS0_PIN) */
    #endif /* !(Sensor_UART_CY_SCBIP_V0 || Sensor_UART_CY_SCBIP_V1) */

#else
    #if (Sensor_UART_UART_TX_PIN)
         /* Set SCB UART to drive the output pin */
        Sensor_UART_SET_HSIOM_SEL(Sensor_UART_TX_HSIOM_REG, Sensor_UART_TX_HSIOM_MASK,
                                       Sensor_UART_TX_HSIOM_POS, Sensor_UART_TX_HSIOM_SEL_UART);
    #endif /* (Sensor_UART_UART_TX_PIN) */

    #if (Sensor_UART_UART_RTS_PIN)
        /* Set SCB UART to drive the output pin */
        Sensor_UART_SET_HSIOM_SEL(Sensor_UART_RTS_HSIOM_REG, Sensor_UART_RTS_HSIOM_MASK,
                                       Sensor_UART_RTS_HSIOM_POS, Sensor_UART_RTS_HSIOM_SEL_UART);
    #endif /* (Sensor_UART_UART_RTS_PIN) */
#endif /* (Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Restore TX interrupt sources. */
    Sensor_UART_SetTxInterruptMode(Sensor_UART_IntrTxMask);
}


/*******************************************************************************
* Function Name: Sensor_UART_UartStop
****************************************************************************//**
*
*  Changes the HSIOM settings for the UART output pins (TX and/or RTS) to keep
*  them inactive after the block is disabled. The output pins are controlled by
*  the GPIO data register. Also, the function disables the skip start feature
*  to not cause it to trigger after the component is enabled.
*
*******************************************************************************/
void Sensor_UART_UartStop(void)
{
#if(Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG)
    #if (Sensor_UART_TX_SDA_MISO_PIN)
        if (Sensor_UART_CHECK_TX_SDA_MISO_PIN_USED)
        {
            /* Set GPIO to drive output pin */
            Sensor_UART_SET_HSIOM_SEL(Sensor_UART_TX_SDA_MISO_HSIOM_REG, Sensor_UART_TX_SDA_MISO_HSIOM_MASK,
                                           Sensor_UART_TX_SDA_MISO_HSIOM_POS, Sensor_UART_TX_SDA_MISO_HSIOM_SEL_GPIO);
        }
    #endif /* (Sensor_UART_TX_SDA_MISO_PIN_PIN) */

    #if !(Sensor_UART_CY_SCBIP_V0 || Sensor_UART_CY_SCBIP_V1)
        #if (Sensor_UART_RTS_SS0_PIN)
            if (Sensor_UART_CHECK_RTS_SS0_PIN_USED)
            {
                /* Set output pin state after block is disabled */
                Sensor_UART_uart_rts_spi_ss0_Write(Sensor_UART_GET_UART_RTS_INACTIVE);

                /* Set GPIO to drive output pin */
                Sensor_UART_SET_HSIOM_SEL(Sensor_UART_RTS_SS0_HSIOM_REG, Sensor_UART_RTS_SS0_HSIOM_MASK,
                                               Sensor_UART_RTS_SS0_HSIOM_POS, Sensor_UART_RTS_SS0_HSIOM_SEL_GPIO);
            }
        #endif /* (Sensor_UART_RTS_SS0_PIN) */
    #endif /* !(Sensor_UART_CY_SCBIP_V0 || Sensor_UART_CY_SCBIP_V1) */

#else
    #if (Sensor_UART_UART_TX_PIN)
        /* Set GPIO to drive output pin */
        Sensor_UART_SET_HSIOM_SEL(Sensor_UART_TX_HSIOM_REG, Sensor_UART_TX_HSIOM_MASK,
                                       Sensor_UART_TX_HSIOM_POS, Sensor_UART_TX_HSIOM_SEL_GPIO);
    #endif /* (Sensor_UART_UART_TX_PIN) */

    #if (Sensor_UART_UART_RTS_PIN)
        /* Set output pin state after block is disabled */
        Sensor_UART_rts_Write(Sensor_UART_GET_UART_RTS_INACTIVE);

        /* Set GPIO to drive output pin */
        Sensor_UART_SET_HSIOM_SEL(Sensor_UART_RTS_HSIOM_REG, Sensor_UART_RTS_HSIOM_MASK,
                                       Sensor_UART_RTS_HSIOM_POS, Sensor_UART_RTS_HSIOM_SEL_GPIO);
    #endif /* (Sensor_UART_UART_RTS_PIN) */

#endif /* (Sensor_UART_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (Sensor_UART_UART_WAKE_ENABLE_CONST)
    /* Disable skip start feature used for wakeup */
    Sensor_UART_UART_RX_CTRL_REG &= (uint32) ~Sensor_UART_UART_RX_CTRL_SKIP_START;
#endif /* (Sensor_UART_UART_WAKE_ENABLE_CONST) */

    /* Store TX interrupt sources (exclude level triggered). */
    Sensor_UART_IntrTxMask = LO16(Sensor_UART_GetTxInterruptMode() & Sensor_UART_INTR_UART_TX_RESTORE);
}


/*******************************************************************************
* Function Name: Sensor_UART_UartSetRxAddress
****************************************************************************//**
*
*  Sets the hardware detectable receiver address for the UART in the
*  Multiprocessor mode.
*
*  \param address: Address for hardware address detection.
*
*******************************************************************************/
void Sensor_UART_UartSetRxAddress(uint32 address)
{
     uint32 matchReg;

    matchReg = Sensor_UART_RX_MATCH_REG;

    matchReg &= ((uint32) ~Sensor_UART_RX_MATCH_ADDR_MASK); /* Clear address bits */
    matchReg |= ((uint32)  (address & Sensor_UART_RX_MATCH_ADDR_MASK)); /* Set address  */

    Sensor_UART_RX_MATCH_REG = matchReg;
}


/*******************************************************************************
* Function Name: Sensor_UART_UartSetRxAddressMask
****************************************************************************//**
*
*  Sets the hardware address mask for the UART in the Multiprocessor mode.
*
*  \param addressMask: Address mask.
*   - Bit value 0 – excludes bit from address comparison.
*   - Bit value 1 – the bit needs to match with the corresponding bit
*     of the address.
*
*******************************************************************************/
void Sensor_UART_UartSetRxAddressMask(uint32 addressMask)
{
    uint32 matchReg;

    matchReg = Sensor_UART_RX_MATCH_REG;

    matchReg &= ((uint32) ~Sensor_UART_RX_MATCH_MASK_MASK); /* Clear address mask bits */
    matchReg |= ((uint32) (addressMask << Sensor_UART_RX_MATCH_MASK_POS));

    Sensor_UART_RX_MATCH_REG = matchReg;
}


#if(Sensor_UART_UART_RX_DIRECTION)
    /*******************************************************************************
    * Function Name: Sensor_UART_UartGetChar
    ****************************************************************************//**
    *
    *  Retrieves next data element from receive buffer.
    *  This function is designed for ASCII characters and returns a char where
    *  1 to 255 are valid characters and 0 indicates an error occurred or no data
    *  is present.
    *  - RX software buffer is disabled: Returns data element retrieved from RX
    *    FIFO.
    *  - RX software buffer is enabled: Returns data element from the software
    *    receive buffer.
    *
    *  \return
    *   Next data element from the receive buffer. ASCII character values from
    *   1 to 255 are valid. A returned zero signifies an error condition or no
    *   data available.
    *
    *  \sideeffect
    *   The errors bits may not correspond with reading characters due to
    *   RX FIFO and software buffer usage.
    *   RX software buffer is enabled: The internal software buffer overflow
    *   is not treated as an error condition.
    *   Check Sensor_UART_rxBufferOverflow to capture that error condition.
    *
    *******************************************************************************/
    uint32 Sensor_UART_UartGetChar(void)
    {
        uint32 rxData = 0u;

        /* Reads data only if there is data to read */
        if (0u != Sensor_UART_SpiUartGetRxBufferSize())
        {
            rxData = Sensor_UART_SpiUartReadRxData();
        }

        if (Sensor_UART_CHECK_INTR_RX(Sensor_UART_INTR_RX_ERR))
        {
            rxData = 0u; /* Error occurred: returns zero */
            Sensor_UART_ClearRxInterruptSource(Sensor_UART_INTR_RX_ERR);
        }

        return (rxData);
    }


    /*******************************************************************************
    * Function Name: Sensor_UART_UartGetByte
    ****************************************************************************//**
    *
    *  Retrieves the next data element from the receive buffer, returns the
    *  received byte and error condition.
    *   - The RX software buffer is disabled: returns the data element retrieved
    *     from the RX FIFO. Undefined data will be returned if the RX FIFO is
    *     empty.
    *   - The RX software buffer is enabled: returns data element from the
    *     software receive buffer.
    *
    *  \return
    *   Bits 7-0 contain the next data element from the receive buffer and
    *   other bits contain the error condition.
    *   - Sensor_UART_UART_RX_OVERFLOW - Attempt to write to a full
    *     receiver FIFO.
    *   - Sensor_UART_UART_RX_UNDERFLOW    Attempt to read from an empty
    *     receiver FIFO.
    *   - Sensor_UART_UART_RX_FRAME_ERROR - UART framing error detected.
    *   - Sensor_UART_UART_RX_PARITY_ERROR - UART parity error detected.
    *
    *  \sideeffect
    *   The errors bits may not correspond with reading characters due to
    *   RX FIFO and software buffer usage.
    *   RX software buffer is enabled: The internal software buffer overflow
    *   is not treated as an error condition.
    *   Check Sensor_UART_rxBufferOverflow to capture that error condition.
    *
    *******************************************************************************/
    uint32 Sensor_UART_UartGetByte(void)
    {
        uint32 rxData;
        uint32 tmpStatus;

        #if (Sensor_UART_CHECK_RX_SW_BUFFER)
        {
            Sensor_UART_DisableInt();
        }
        #endif

        if (0u != Sensor_UART_SpiUartGetRxBufferSize())
        {
            /* Enables interrupt to receive more bytes: at least one byte is in
            * buffer.
            */
            #if (Sensor_UART_CHECK_RX_SW_BUFFER)
            {
                Sensor_UART_EnableInt();
            }
            #endif

            /* Get received byte */
            rxData = Sensor_UART_SpiUartReadRxData();
        }
        else
        {
            /* Reads a byte directly from RX FIFO: underflow is raised in the
            * case of empty. Otherwise the first received byte will be read.
            */
            rxData = Sensor_UART_RX_FIFO_RD_REG;


            /* Enables interrupt to receive more bytes. */
            #if (Sensor_UART_CHECK_RX_SW_BUFFER)
            {

                /* The byte has been read from RX FIFO. Clear RX interrupt to
                * not involve interrupt handler when RX FIFO is empty.
                */
                Sensor_UART_ClearRxInterruptSource(Sensor_UART_INTR_RX_NOT_EMPTY);

                Sensor_UART_EnableInt();
            }
            #endif
        }

        /* Get and clear RX error mask */
        tmpStatus = (Sensor_UART_GetRxInterruptSource() & Sensor_UART_INTR_RX_ERR);
        Sensor_UART_ClearRxInterruptSource(Sensor_UART_INTR_RX_ERR);

        /* Puts together data and error status:
        * MP mode and accept address: 9th bit is set to notify mark.
        */
        rxData |= ((uint32) (tmpStatus << 8u));

        return (rxData);
    }


    #if !(Sensor_UART_CY_SCBIP_V0 || Sensor_UART_CY_SCBIP_V1)
        /*******************************************************************************
        * Function Name: Sensor_UART_UartSetRtsPolarity
        ****************************************************************************//**
        *
        *  Sets active polarity of RTS output signal.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *  \param polarity: Active polarity of RTS output signal.
        *   - Sensor_UART_UART_RTS_ACTIVE_LOW  - RTS signal is active low.
        *   - Sensor_UART_UART_RTS_ACTIVE_HIGH - RTS signal is active high.
        *
        *******************************************************************************/
        void Sensor_UART_UartSetRtsPolarity(uint32 polarity)
        {
            if(0u != polarity)
            {
                Sensor_UART_UART_FLOW_CTRL_REG |= (uint32)  Sensor_UART_UART_FLOW_CTRL_RTS_POLARITY;
            }
            else
            {
                Sensor_UART_UART_FLOW_CTRL_REG &= (uint32) ~Sensor_UART_UART_FLOW_CTRL_RTS_POLARITY;
            }
        }


        /*******************************************************************************
        * Function Name: Sensor_UART_UartSetRtsFifoLevel
        ****************************************************************************//**
        *
        *  Sets level in the RX FIFO for RTS signal activation.
        *  While the RX FIFO has fewer entries than the RX FIFO level the RTS signal
        *  remains active, otherwise the RTS signal becomes inactive.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *  \param level: Level in the RX FIFO for RTS signal activation.
        *   The range of valid level values is between 0 and RX FIFO depth - 1.
        *   Setting level value to 0 disables RTS signal activation.
        *
        *******************************************************************************/
        void Sensor_UART_UartSetRtsFifoLevel(uint32 level)
        {
            uint32 uartFlowCtrl;

            uartFlowCtrl = Sensor_UART_UART_FLOW_CTRL_REG;

            uartFlowCtrl &= ((uint32) ~Sensor_UART_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
            uartFlowCtrl |= ((uint32) (Sensor_UART_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK & level));

            Sensor_UART_UART_FLOW_CTRL_REG = uartFlowCtrl;
        }
    #endif /* !(Sensor_UART_CY_SCBIP_V0 || Sensor_UART_CY_SCBIP_V1) */

#endif /* (Sensor_UART_UART_RX_DIRECTION) */


#if(Sensor_UART_UART_TX_DIRECTION)
    /*******************************************************************************
    * Function Name: Sensor_UART_UartPutString
    ****************************************************************************//**
    *
    *  Places a NULL terminated string in the transmit buffer to be sent at the
    *  next available bus time.
    *  This function is blocking and waits until there is a space available to put
    *  requested data in transmit buffer.
    *
    *  \param string: pointer to the null terminated string array to be placed in the
    *   transmit buffer.
    *
    *******************************************************************************/
    void Sensor_UART_UartPutString(const char8 string[])
    {
        uint32 bufIndex;

        bufIndex = 0u;

        /* Blocks the control flow until all data has been sent */
        while(string[bufIndex] != ((char8) 0))
        {
            Sensor_UART_UartPutChar((uint32) string[bufIndex]);
            bufIndex++;
        }
    }


    /*******************************************************************************
    * Function Name: Sensor_UART_UartPutCRLF
    ****************************************************************************//**
    *
    *  Places byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) in the transmit buffer.
    *  This function is blocking and waits until there is a space available to put
    *  all requested data in transmit buffer.
    *
    *  \param txDataByte: the data to be transmitted.
    *
    *******************************************************************************/
    void Sensor_UART_UartPutCRLF(uint32 txDataByte)
    {
        Sensor_UART_UartPutChar(txDataByte);  /* Blocks control flow until all data has been sent */
        Sensor_UART_UartPutChar(0x0Du);       /* Blocks control flow until all data has been sent */
        Sensor_UART_UartPutChar(0x0Au);       /* Blocks control flow until all data has been sent */
    }


    #if !(Sensor_UART_CY_SCBIP_V0 || Sensor_UART_CY_SCBIP_V1)
        /*******************************************************************************
        * Function Name: Sensor_UARTSCB_UartEnableCts
        ****************************************************************************//**
        *
        *  Enables usage of CTS input signal by the UART transmitter.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *******************************************************************************/
        void Sensor_UART_UartEnableCts(void)
        {
            Sensor_UART_UART_FLOW_CTRL_REG |= (uint32)  Sensor_UART_UART_FLOW_CTRL_CTS_ENABLE;
        }


        /*******************************************************************************
        * Function Name: Sensor_UART_UartDisableCts
        ****************************************************************************//**
        *
        *  Disables usage of CTS input signal by the UART transmitter.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *******************************************************************************/
        void Sensor_UART_UartDisableCts(void)
        {
            Sensor_UART_UART_FLOW_CTRL_REG &= (uint32) ~Sensor_UART_UART_FLOW_CTRL_CTS_ENABLE;
        }


        /*******************************************************************************
        * Function Name: Sensor_UART_UartSetCtsPolarity
        ****************************************************************************//**
        *
        *  Sets active polarity of CTS input signal.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        * \param
        * polarity: Active polarity of CTS output signal.
        *   - Sensor_UART_UART_CTS_ACTIVE_LOW  - CTS signal is active low.
        *   - Sensor_UART_UART_CTS_ACTIVE_HIGH - CTS signal is active high.
        *
        *******************************************************************************/
        void Sensor_UART_UartSetCtsPolarity(uint32 polarity)
        {
            if (0u != polarity)
            {
                Sensor_UART_UART_FLOW_CTRL_REG |= (uint32)  Sensor_UART_UART_FLOW_CTRL_CTS_POLARITY;
            }
            else
            {
                Sensor_UART_UART_FLOW_CTRL_REG &= (uint32) ~Sensor_UART_UART_FLOW_CTRL_CTS_POLARITY;
            }
        }
    #endif /* !(Sensor_UART_CY_SCBIP_V0 || Sensor_UART_CY_SCBIP_V1) */


    /*******************************************************************************
    * Function Name: Sensor_UART_UartSendBreakBlocking
    ****************************************************************************//**
    *
    * Sends a break condition (logic low) of specified width on UART TX line.
    * Blocks until break is completed. Only call this function when UART TX FIFO
    * and shifter are empty.
    *
    * \param breakWidth
    * Width of break condition. Valid range is 4 to 16 bits.
    *
    * \note
    * Before sending break all UART TX interrupt sources are disabled. The state
    * of UART TX interrupt sources is restored before function returns.
    *
    * \sideeffect
    * If this function is called while there is data in the TX FIFO or shifter that
    * data will be shifted out in packets the size of breakWidth.
    *
    *******************************************************************************/
    void Sensor_UART_UartSendBreakBlocking(uint32 breakWidth)
    {
        uint32 txCtrlReg;
        uint32 txIntrReg;

        /* Disable all UART TX interrupt source and clear UART TX Done history */
        txIntrReg = Sensor_UART_GetTxInterruptMode();
        Sensor_UART_SetTxInterruptMode(0u);
        Sensor_UART_ClearTxInterruptSource(Sensor_UART_INTR_TX_UART_DONE);

        /* Store TX CTRL configuration */
        txCtrlReg = Sensor_UART_TX_CTRL_REG;

        /* Set break width */
        Sensor_UART_TX_CTRL_REG = (Sensor_UART_TX_CTRL_REG & (uint32) ~Sensor_UART_TX_CTRL_DATA_WIDTH_MASK) |
                                        Sensor_UART_GET_TX_CTRL_DATA_WIDTH(breakWidth);

        /* Generate break */
        Sensor_UART_TX_FIFO_WR_REG = 0u;

        /* Wait for break completion */
        while (0u == (Sensor_UART_GetTxInterruptSource() & Sensor_UART_INTR_TX_UART_DONE))
        {
        }

        /* Clear all UART TX interrupt sources to  */
        Sensor_UART_ClearTxInterruptSource(Sensor_UART_INTR_TX_ALL);

        /* Restore TX interrupt sources and data width */
        Sensor_UART_TX_CTRL_REG = txCtrlReg;
        Sensor_UART_SetTxInterruptMode(txIntrReg);
    }
#endif /* (Sensor_UART_UART_TX_DIRECTION) */


#if (Sensor_UART_UART_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: Sensor_UART_UartSaveConfig
    ****************************************************************************//**
    *
    *  Clears and enables an interrupt on a falling edge of the Rx input. The GPIO
    *  interrupt does not track in the active mode, therefore requires to be
    *  cleared by this API.
    *
    *******************************************************************************/
    void Sensor_UART_UartSaveConfig(void)
    {
    #if (Sensor_UART_UART_RX_WAKEUP_IRQ)
        /* Set SKIP_START if requested (set by default). */
        if (0u != Sensor_UART_skipStart)
        {
            Sensor_UART_UART_RX_CTRL_REG |= (uint32)  Sensor_UART_UART_RX_CTRL_SKIP_START;
        }
        else
        {
            Sensor_UART_UART_RX_CTRL_REG &= (uint32) ~Sensor_UART_UART_RX_CTRL_SKIP_START;
        }

        /* Clear RX GPIO interrupt status and pending interrupt in NVIC because
        * falling edge on RX line occurs while UART communication in active mode.
        * Enable interrupt: next interrupt trigger should wakeup device.
        */
        Sensor_UART_CLEAR_UART_RX_WAKE_INTR;
        Sensor_UART_RxWakeClearPendingInt();
        Sensor_UART_RxWakeEnableInt();
    #endif /* (Sensor_UART_UART_RX_WAKEUP_IRQ) */
    }


    /*******************************************************************************
    * Function Name: Sensor_UART_UartRestoreConfig
    ****************************************************************************//**
    *
    *  Disables the RX GPIO interrupt. Until this function is called the interrupt
    *  remains active and triggers on every falling edge of the UART RX line.
    *
    *******************************************************************************/
    void Sensor_UART_UartRestoreConfig(void)
    {
    #if (Sensor_UART_UART_RX_WAKEUP_IRQ)
        /* Disable interrupt: no more triggers in active mode */
        Sensor_UART_RxWakeDisableInt();
    #endif /* (Sensor_UART_UART_RX_WAKEUP_IRQ) */
    }


    #if (Sensor_UART_UART_RX_WAKEUP_IRQ)
        /*******************************************************************************
        * Function Name: Sensor_UART_UART_WAKEUP_ISR
        ****************************************************************************//**
        *
        *  Handles the Interrupt Service Routine for the SCB UART mode GPIO wakeup
        *  event. This event is configured to trigger on a falling edge of the RX line.
        *
        *******************************************************************************/
        CY_ISR(Sensor_UART_UART_WAKEUP_ISR)
        {
        #ifdef Sensor_UART_UART_WAKEUP_ISR_ENTRY_CALLBACK
            Sensor_UART_UART_WAKEUP_ISR_EntryCallback();
        #endif /* Sensor_UART_UART_WAKEUP_ISR_ENTRY_CALLBACK */

            Sensor_UART_CLEAR_UART_RX_WAKE_INTR;

        #ifdef Sensor_UART_UART_WAKEUP_ISR_EXIT_CALLBACK
            Sensor_UART_UART_WAKEUP_ISR_ExitCallback();
        #endif /* Sensor_UART_UART_WAKEUP_ISR_EXIT_CALLBACK */
        }
    #endif /* (Sensor_UART_UART_RX_WAKEUP_IRQ) */
#endif /* (Sensor_UART_UART_RX_WAKEUP_IRQ) */


/* [] END OF FILE */
