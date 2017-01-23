/*******************************************************************************
* File Name: Wave_Control_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Wave_Control.h"

/* Check for removal by optimization */
#if !defined(Wave_Control_Sync_ctrl_reg__REMOVED)

static Wave_Control_BACKUP_STRUCT  Wave_Control_backup = {0u};

    
/*******************************************************************************
* Function Name: Wave_Control_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Wave_Control_SaveConfig(void) 
{
    Wave_Control_backup.controlState = Wave_Control_Control;
}


/*******************************************************************************
* Function Name: Wave_Control_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void Wave_Control_RestoreConfig(void) 
{
     Wave_Control_Control = Wave_Control_backup.controlState;
}


/*******************************************************************************
* Function Name: Wave_Control_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Wave_Control_Sleep(void) 
{
    Wave_Control_SaveConfig();
}


/*******************************************************************************
* Function Name: Wave_Control_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Wave_Control_Wakeup(void)  
{
    Wave_Control_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
