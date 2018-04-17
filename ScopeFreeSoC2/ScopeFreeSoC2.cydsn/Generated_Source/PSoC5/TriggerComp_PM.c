/*******************************************************************************
* File Name: TriggerComp.c
* Version 2.0
*
* Description:
*  This file provides the power management source code APIs for the
*  Comparator.
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

#include "TriggerComp.h"

static TriggerComp_backupStruct TriggerComp_backup;


/*******************************************************************************
* Function Name: TriggerComp_SaveConfig
********************************************************************************
*
* Summary:
*  Save the current user configuration
*
* Parameters:
*  void:
*
* Return:
*  void
*
*******************************************************************************/
void TriggerComp_SaveConfig(void) 
{
    /* Empty since all are system reset for retention flops */
}


/*******************************************************************************
* Function Name: TriggerComp_RestoreConfig
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
********************************************************************************/
void TriggerComp_RestoreConfig(void) 
{
    /* Empty since all are system reset for retention flops */    
}


/*******************************************************************************
* Function Name: TriggerComp_Sleep
********************************************************************************
*
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
*  TriggerComp_backup.enableState:  Is modified depending on the enable 
*   state of the block before entering sleep mode.
*
*******************************************************************************/
void TriggerComp_Sleep(void) 
{
    /* Save Comp's enable state */    
    if(TriggerComp_ACT_PWR_EN == (TriggerComp_PWRMGR & TriggerComp_ACT_PWR_EN))
    {
        /* Comp is enabled */
        TriggerComp_backup.enableState = 1u;
    }
    else
    {
        /* Comp is disabled */
        TriggerComp_backup.enableState = 0u;
    }    
    
    TriggerComp_Stop();
    TriggerComp_SaveConfig();
}


/*******************************************************************************
* Function Name: TriggerComp_Wakeup
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
*  TriggerComp_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void TriggerComp_Wakeup(void) 
{
    TriggerComp_RestoreConfig();
    
    if(TriggerComp_backup.enableState == 1u)
    {
        /* Enable Comp's operation */
        TriggerComp_Enable();

    } /* Do nothing if Comp was disabled before */ 
}


/* [] END OF FILE */
