/*******************************************************************************
* File Name: TriggerStatus.c  
* Version 1.80
*
* Description:
*  This file contains API to enable firmware to read the value of a Status 
*  Register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "TriggerStatus.h"

#if !defined(TriggerStatus_sts_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: TriggerStatus_Read
********************************************************************************
*
* Summary:
*  Reads the current value assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The current value in the Status Register.
*
*******************************************************************************/
uint8 TriggerStatus_Read(void) 
{ 
    return TriggerStatus_Status;
}


/*******************************************************************************
* Function Name: TriggerStatus_InterruptEnable
********************************************************************************
*
* Summary:
*  Enables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void TriggerStatus_InterruptEnable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    TriggerStatus_Status_Aux_Ctrl |= TriggerStatus_STATUS_INTR_ENBL;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: TriggerStatus_InterruptDisable
********************************************************************************
*
* Summary:
*  Disables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void TriggerStatus_InterruptDisable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    TriggerStatus_Status_Aux_Ctrl &= (uint8)(~TriggerStatus_STATUS_INTR_ENBL);
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: TriggerStatus_WriteMask
********************************************************************************
*
* Summary:
*  Writes the current mask value assigned to the Status Register.
*
* Parameters:
*  mask:  Value to write into the mask register.
*
* Return:
*  None.
*
*******************************************************************************/
void TriggerStatus_WriteMask(uint8 mask) 
{
    #if(TriggerStatus_INPUTS < 8u)
    	mask &= (uint8)((((uint8)1u) << TriggerStatus_INPUTS) - 1u);
	#endif /* End TriggerStatus_INPUTS < 8u */
    TriggerStatus_Status_Mask = mask;
}


/*******************************************************************************
* Function Name: TriggerStatus_ReadMask
********************************************************************************
*
* Summary:
*  Reads the current interrupt mask assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The value of the interrupt mask of the Status Register.
*
*******************************************************************************/
uint8 TriggerStatus_ReadMask(void) 
{
    return TriggerStatus_Status_Mask;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
