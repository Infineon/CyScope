/*******************************************************************************
* File Name: P5_3_TriggerLut_S0.c  
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
#include "P5_3_TriggerLut_S0.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 P5_3_TriggerLut_S0__PORT == 15 && ((P5_3_TriggerLut_S0__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: P5_3_TriggerLut_S0_Write
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
void P5_3_TriggerLut_S0_Write(uint8 value) 
{
    uint8 staticBits = (P5_3_TriggerLut_S0_DR & (uint8)(~P5_3_TriggerLut_S0_MASK));
    P5_3_TriggerLut_S0_DR = staticBits | ((uint8)(value << P5_3_TriggerLut_S0_SHIFT) & P5_3_TriggerLut_S0_MASK);
}


/*******************************************************************************
* Function Name: P5_3_TriggerLut_S0_SetDriveMode
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
void P5_3_TriggerLut_S0_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(P5_3_TriggerLut_S0_0, mode);
}


/*******************************************************************************
* Function Name: P5_3_TriggerLut_S0_Read
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
*  Macro P5_3_TriggerLut_S0_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 P5_3_TriggerLut_S0_Read(void) 
{
    return (P5_3_TriggerLut_S0_PS & P5_3_TriggerLut_S0_MASK) >> P5_3_TriggerLut_S0_SHIFT;
}


/*******************************************************************************
* Function Name: P5_3_TriggerLut_S0_ReadDataReg
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
uint8 P5_3_TriggerLut_S0_ReadDataReg(void) 
{
    return (P5_3_TriggerLut_S0_DR & P5_3_TriggerLut_S0_MASK) >> P5_3_TriggerLut_S0_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(P5_3_TriggerLut_S0_INTSTAT) 

    /*******************************************************************************
    * Function Name: P5_3_TriggerLut_S0_ClearInterrupt
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
    uint8 P5_3_TriggerLut_S0_ClearInterrupt(void) 
    {
        return (P5_3_TriggerLut_S0_INTSTAT & P5_3_TriggerLut_S0_MASK) >> P5_3_TriggerLut_S0_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
