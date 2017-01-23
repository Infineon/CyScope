/*******************************************************************************
* File Name: Vtrigger_PM.c  
* Version 1.90
*
* Description:
*  This file provides the power management source code to API for the
*  VDAC8.  
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Vtrigger.h"

static Vtrigger_backupStruct Vtrigger_backup;


/*******************************************************************************
* Function Name: Vtrigger_SaveConfig
********************************************************************************
* Summary:
*  Save the current user configuration
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void Vtrigger_SaveConfig(void) 
{
    if (!((Vtrigger_CR1 & Vtrigger_SRC_MASK) == Vtrigger_SRC_UDB))
    {
        Vtrigger_backup.data_value = Vtrigger_Data;
    }
}


/*******************************************************************************
* Function Name: Vtrigger_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
*******************************************************************************/
void Vtrigger_RestoreConfig(void) 
{
    if (!((Vtrigger_CR1 & Vtrigger_SRC_MASK) == Vtrigger_SRC_UDB))
    {
        if((Vtrigger_Strobe & Vtrigger_STRB_MASK) == Vtrigger_STRB_EN)
        {
            Vtrigger_Strobe &= (uint8)(~Vtrigger_STRB_MASK);
            Vtrigger_Data = Vtrigger_backup.data_value;
            Vtrigger_Strobe |= Vtrigger_STRB_EN;
        }
        else
        {
            Vtrigger_Data = Vtrigger_backup.data_value;
        }
    }
}


/*******************************************************************************
* Function Name: Vtrigger_Sleep
********************************************************************************
* Summary:
*  Stop and Save the user configuration
*
* Parameters:  
*  void:  
*
* Return: 
*  void
*
* Global variables:
*  Vtrigger_backup.enableState:  Is modified depending on the enable 
*  state  of the block before entering sleep mode.
*
*******************************************************************************/
void Vtrigger_Sleep(void) 
{
    /* Save VDAC8's enable state */    
    if(Vtrigger_ACT_PWR_EN == (Vtrigger_PWRMGR & Vtrigger_ACT_PWR_EN))
    {
        /* VDAC8 is enabled */
        Vtrigger_backup.enableState = 1u;
    }
    else
    {
        /* VDAC8 is disabled */
        Vtrigger_backup.enableState = 0u;
    }
    
    Vtrigger_Stop();
    Vtrigger_SaveConfig();
}


/*******************************************************************************
* Function Name: Vtrigger_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Vtrigger_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Vtrigger_Wakeup(void) 
{
    Vtrigger_RestoreConfig();
    
    if(Vtrigger_backup.enableState == 1u)
    {
        /* Enable VDAC8's operation */
        Vtrigger_Enable();

        /* Restore the data register */
        Vtrigger_SetValue(Vtrigger_Data);
    } /* Do nothing if VDAC8 was disabled before */    
}


/* [] END OF FILE */
