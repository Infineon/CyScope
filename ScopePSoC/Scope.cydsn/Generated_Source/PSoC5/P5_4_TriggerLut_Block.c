/*******************************************************************************
* File Name: P5_4_TriggerLut_Block.c  
* Version 1.90
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "P5_4_TriggerLut_Block.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 P5_4_TriggerLut_Block__PORT == 15 && ((P5_4_TriggerLut_Block__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: P5_4_TriggerLut_Block_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void P5_4_TriggerLut_Block_Write(uint8 value) 
{
    uint8 staticBits = (P5_4_TriggerLut_Block_DR & (uint8)(~P5_4_TriggerLut_Block_MASK));
    P5_4_TriggerLut_Block_DR = staticBits | ((uint8)(value << P5_4_TriggerLut_Block_SHIFT) & P5_4_TriggerLut_Block_MASK);
}


/*******************************************************************************
* Function Name: P5_4_TriggerLut_Block_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to this drive mode.
*
* Return: 
*  None
*
*******************************************************************************/
void P5_4_TriggerLut_Block_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(P5_4_TriggerLut_Block_0, mode);
}


/*******************************************************************************
* Function Name: P5_4_TriggerLut_Block_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro P5_4_TriggerLut_Block_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 P5_4_TriggerLut_Block_Read(void) 
{
    return (P5_4_TriggerLut_Block_PS & P5_4_TriggerLut_Block_MASK) >> P5_4_TriggerLut_Block_SHIFT;
}


/*******************************************************************************
* Function Name: P5_4_TriggerLut_Block_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 P5_4_TriggerLut_Block_ReadDataReg(void) 
{
    return (P5_4_TriggerLut_Block_DR & P5_4_TriggerLut_Block_MASK) >> P5_4_TriggerLut_Block_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(P5_4_TriggerLut_Block_INTSTAT) 

    /*******************************************************************************
    * Function Name: P5_4_TriggerLut_Block_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 P5_4_TriggerLut_Block_ClearInterrupt(void) 
    {
        return (P5_4_TriggerLut_Block_INTSTAT & P5_4_TriggerLut_Block_MASK) >> P5_4_TriggerLut_Block_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
