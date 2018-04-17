/*******************************************************************************
* File Name: UART_PM.c
* Version 2.30
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "UART.h"


/***************************************
* Local data allocation
***************************************/

static UART_BACKUP_STRUCT  UART_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: UART_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_SaveConfig(void)
{
    #if (CY_UDB_V0)

        #if(UART_CONTROL_REG_REMOVED == 0u)
            UART_backup.cr = UART_CONTROL_REG;
        #endif /* End UART_CONTROL_REG_REMOVED */

        #if( (UART_RX_ENABLED) || (UART_HD_ENABLED) )
            UART_backup.rx_period = UART_RXBITCTR_PERIOD_REG;
            UART_backup.rx_mask = UART_RXSTATUS_MASK_REG;
            #if (UART_RXHW_ADDRESS_ENABLED)
                UART_backup.rx_addr1 = UART_RXADDRESS1_REG;
                UART_backup.rx_addr2 = UART_RXADDRESS2_REG;
            #endif /* End UART_RXHW_ADDRESS_ENABLED */
        #endif /* End UART_RX_ENABLED | UART_HD_ENABLED*/

        #if(UART_TX_ENABLED)
            #if(UART_TXCLKGEN_DP)
                UART_backup.tx_clk_ctr = UART_TXBITCLKGEN_CTR_REG;
                UART_backup.tx_clk_compl = UART_TXBITCLKTX_COMPLETE_REG;
            #else
                UART_backup.tx_period = UART_TXBITCTR_PERIOD_REG;
            #endif /*End UART_TXCLKGEN_DP */
            UART_backup.tx_mask = UART_TXSTATUS_MASK_REG;
        #endif /*End UART_TX_ENABLED */


    #else /* CY_UDB_V1 */

        #if(UART_CONTROL_REG_REMOVED == 0u)
            UART_backup.cr = UART_CONTROL_REG;
        #endif /* End UART_CONTROL_REG_REMOVED */

    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: UART_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_RestoreConfig(void)
{

    #if (CY_UDB_V0)

        #if(UART_CONTROL_REG_REMOVED == 0u)
            UART_CONTROL_REG = UART_backup.cr;
        #endif /* End UART_CONTROL_REG_REMOVED */

        #if( (UART_RX_ENABLED) || (UART_HD_ENABLED) )
            UART_RXBITCTR_PERIOD_REG = UART_backup.rx_period;
            UART_RXSTATUS_MASK_REG = UART_backup.rx_mask;
            #if (UART_RXHW_ADDRESS_ENABLED)
                UART_RXADDRESS1_REG = UART_backup.rx_addr1;
                UART_RXADDRESS2_REG = UART_backup.rx_addr2;
            #endif /* End UART_RXHW_ADDRESS_ENABLED */
        #endif  /* End (UART_RX_ENABLED) || (UART_HD_ENABLED) */

        #if(UART_TX_ENABLED)
            #if(UART_TXCLKGEN_DP)
                UART_TXBITCLKGEN_CTR_REG = UART_backup.tx_clk_ctr;
                UART_TXBITCLKTX_COMPLETE_REG = UART_backup.tx_clk_compl;
            #else
                UART_TXBITCTR_PERIOD_REG = UART_backup.tx_period;
            #endif /*End UART_TXCLKGEN_DP */
            UART_TXSTATUS_MASK_REG = UART_backup.tx_mask;
        #endif /*End UART_TX_ENABLED */

    #else /* CY_UDB_V1 */

        #if(UART_CONTROL_REG_REMOVED == 0u)
            UART_CONTROL_REG = UART_backup.cr;
        #endif /* End UART_CONTROL_REG_REMOVED */

    #endif  /* End CY_UDB_V0 */
}


/*******************************************************************************
* Function Name: UART_Sleep
********************************************************************************
*
* Summary:
*  Stops and saves the user configuration. Should be called
*  just prior to entering sleep.
*
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_Sleep(void)
{

    #if(UART_RX_ENABLED || UART_HD_ENABLED)
        if((UART_RXSTATUS_ACTL_REG  & UART_INT_ENABLE) != 0u)
        {
            UART_backup.enableState = 1u;
        }
        else
        {
            UART_backup.enableState = 0u;
        }
    #else
        if((UART_TXSTATUS_ACTL_REG  & UART_INT_ENABLE) !=0u)
        {
            UART_backup.enableState = 1u;
        }
        else
        {
            UART_backup.enableState = 0u;
        }
    #endif /* End UART_RX_ENABLED || UART_HD_ENABLED*/

    UART_Stop();
    UART_SaveConfig();
}


/*******************************************************************************
* Function Name: UART_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called
*  just after awaking from sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_Wakeup(void)
{
    UART_RestoreConfig();
    #if( (UART_RX_ENABLED) || (UART_HD_ENABLED) )
        UART_ClearRxBuffer();
    #endif /* End (UART_RX_ENABLED) || (UART_HD_ENABLED) */
    #if(UART_TX_ENABLED || UART_HD_ENABLED)
        UART_ClearTxBuffer();
    #endif /* End UART_TX_ENABLED || UART_HD_ENABLED */

    if(UART_backup.enableState != 0u)
    {
        UART_Enable();
    }
}


/* [] END OF FILE */
