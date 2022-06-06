/*******************************************************************************
* File Name: Digital_Input_Status.c  
* Version 1.90
*
* Description:
*  This file contains API to enable firmware to read the value of a Status 
*  Register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Digital_Input_Status.h"

#if !defined(Digital_Input_Status_sts_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: Digital_Input_Status_Read
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
uint8 Digital_Input_Status_Read(void) 
{ 
    return Digital_Input_Status_Status;
}


/*******************************************************************************
* Function Name: Digital_Input_Status_InterruptEnable
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
void Digital_Input_Status_InterruptEnable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    Digital_Input_Status_Status_Aux_Ctrl |= Digital_Input_Status_STATUS_INTR_ENBL;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Digital_Input_Status_InterruptDisable
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
void Digital_Input_Status_InterruptDisable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    Digital_Input_Status_Status_Aux_Ctrl &= (uint8)(~Digital_Input_Status_STATUS_INTR_ENBL);
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: Digital_Input_Status_WriteMask
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
void Digital_Input_Status_WriteMask(uint8 mask) 
{
    #if(Digital_Input_Status_INPUTS < 8u)
    	mask &= ((uint8)(1u << Digital_Input_Status_INPUTS) - 1u);
	#endif /* End Digital_Input_Status_INPUTS < 8u */
    Digital_Input_Status_Status_Mask = mask;
}


/*******************************************************************************
* Function Name: Digital_Input_Status_ReadMask
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
uint8 Digital_Input_Status_ReadMask(void) 
{
    return Digital_Input_Status_Status_Mask;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
