/*******************************************************************************
* File Name: P5_2_TRIGGER_MUX_SELECT.c  
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
#include "P5_2_TRIGGER_MUX_SELECT.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 P5_2_TRIGGER_MUX_SELECT__PORT == 15 && ((P5_2_TRIGGER_MUX_SELECT__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: P5_2_TRIGGER_MUX_SELECT_Write
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
void P5_2_TRIGGER_MUX_SELECT_Write(uint8 value) 
{
    uint8 staticBits = (P5_2_TRIGGER_MUX_SELECT_DR & (uint8)(~P5_2_TRIGGER_MUX_SELECT_MASK));
    P5_2_TRIGGER_MUX_SELECT_DR = staticBits | ((uint8)(value << P5_2_TRIGGER_MUX_SELECT_SHIFT) & P5_2_TRIGGER_MUX_SELECT_MASK);
}


/*******************************************************************************
* Function Name: P5_2_TRIGGER_MUX_SELECT_SetDriveMode
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
void P5_2_TRIGGER_MUX_SELECT_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(P5_2_TRIGGER_MUX_SELECT_0, mode);
}


/*******************************************************************************
* Function Name: P5_2_TRIGGER_MUX_SELECT_Read
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
*  Macro P5_2_TRIGGER_MUX_SELECT_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 P5_2_TRIGGER_MUX_SELECT_Read(void) 
{
    return (P5_2_TRIGGER_MUX_SELECT_PS & P5_2_TRIGGER_MUX_SELECT_MASK) >> P5_2_TRIGGER_MUX_SELECT_SHIFT;
}


/*******************************************************************************
* Function Name: P5_2_TRIGGER_MUX_SELECT_ReadDataReg
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
uint8 P5_2_TRIGGER_MUX_SELECT_ReadDataReg(void) 
{
    return (P5_2_TRIGGER_MUX_SELECT_DR & P5_2_TRIGGER_MUX_SELECT_MASK) >> P5_2_TRIGGER_MUX_SELECT_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(P5_2_TRIGGER_MUX_SELECT_INTSTAT) 

    /*******************************************************************************
    * Function Name: P5_2_TRIGGER_MUX_SELECT_ClearInterrupt
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
    uint8 P5_2_TRIGGER_MUX_SELECT_ClearInterrupt(void) 
    {
        return (P5_2_TRIGGER_MUX_SELECT_INTSTAT & P5_2_TRIGGER_MUX_SELECT_MASK) >> P5_2_TRIGGER_MUX_SELECT_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
