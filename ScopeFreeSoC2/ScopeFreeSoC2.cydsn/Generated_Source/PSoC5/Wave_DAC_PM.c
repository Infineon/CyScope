/*******************************************************************************
* File Name: Wave_DAC_PM.c  
* Version 2.10
*
* Description:
*  This file provides the power manager source code to the API for 
*  the WaveDAC8 component.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Wave_DAC.h"

static Wave_DAC_BACKUP_STRUCT  Wave_DAC_backup;


/*******************************************************************************
* Function Name: Wave_DAC_Sleep
********************************************************************************
*
* Summary:
*  Stops the component and saves its configuration. Should be called 
*  just prior to entering sleep.
*  
* Parameters:  
*  None
*
* Return: 
*  None
*
* Global variables:
*  Wave_DAC_backup:  The structure field 'enableState' is modified 
*  depending on the enable state of the block before entering to sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Wave_DAC_Sleep(void) 
{
	/* Save DAC8's enable state */

	Wave_DAC_backup.enableState = (Wave_DAC_VDAC8_ACT_PWR_EN == 
		(Wave_DAC_VDAC8_PWRMGR_REG & Wave_DAC_VDAC8_ACT_PWR_EN)) ? 1u : 0u ;
	
	Wave_DAC_Stop();
	Wave_DAC_SaveConfig();
}


/*******************************************************************************
* Function Name: Wave_DAC_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component configuration. Should be called
*  just after awaking from sleep.
*  
* Parameters:  
*  None
*
* Return: 
*  void
*
* Global variables:
*  Wave_DAC_backup:  The structure field 'enableState' is used to 
*  restore the enable state of block after wakeup from sleep mode.
*
* Reentrant:
*  No
*
*******************************************************************************/
void Wave_DAC_Wakeup(void) 
{
	Wave_DAC_RestoreConfig();

	if(Wave_DAC_backup.enableState == 1u)
	{
		Wave_DAC_Enable();
	}
}


/* [] END OF FILE */
