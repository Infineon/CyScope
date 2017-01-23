/*******************************************************************************
* File Name: Wave_Buf_PM.c
* Version 1.90
*
* Description:
*  This file provides the power management source code to the API for the 
*  OpAmp (Analog Buffer) component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Wave_Buf.h"

static Wave_Buf_BACKUP_STRUCT  Wave_Buf_backup;


/*******************************************************************************  
* Function Name: Wave_Buf_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration registers.
* 
* Parameters:
*  void
* 
* Return:
*  void
*
*******************************************************************************/
void Wave_Buf_SaveConfig(void) 
{
    /* Nothing to save as registers are System reset on retention flops */
}


/*******************************************************************************  
* Function Name: Wave_Buf_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration registers.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Wave_Buf_RestoreConfig(void) 
{
    /* Nothing to restore */
}


/*******************************************************************************   
* Function Name: Wave_Buf_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves its configuration. Should be called 
*  just prior to entering sleep.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Wave_Buf_backup: The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void Wave_Buf_Sleep(void) 
{
    /* Save OpAmp enable state */
    if((Wave_Buf_PM_ACT_CFG_REG & Wave_Buf_ACT_PWR_EN) != 0u)
    {
        /* Component is enabled */
        Wave_Buf_backup.enableState = 1u;
         /* Stops the component */
         Wave_Buf_Stop();
    }
    else
    {
        /* Component is disabled */
        Wave_Buf_backup.enableState = 0u;
    }
    /* Saves the configuration */
    Wave_Buf_SaveConfig();
}


/*******************************************************************************  
* Function Name: Wave_Buf_Wakeup
********************************************************************************
*
* Summary:
*  Enables block's operation and restores its configuration. Should be called
*  just after awaking from sleep.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Wave_Buf_backup: The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void Wave_Buf_Wakeup(void) 
{
    /* Restore the user configuration */
    Wave_Buf_RestoreConfig();

    /* Enables the component operation */
    if(Wave_Buf_backup.enableState == 1u)
    {
        Wave_Buf_Enable();
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
