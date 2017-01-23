/*******************************************************************************
* File Name: P6_3_LED.c  
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
#include "P6_3_LED.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 P6_3_LED__PORT == 15 && ((P6_3_LED__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: P6_3_LED_Write
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
void P6_3_LED_Write(uint8 value) 
{
    uint8 staticBits = (P6_3_LED_DR & (uint8)(~P6_3_LED_MASK));
    P6_3_LED_DR = staticBits | ((uint8)(value << P6_3_LED_SHIFT) & P6_3_LED_MASK);
}


/*******************************************************************************
* Function Name: P6_3_LED_SetDriveMode
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
void P6_3_LED_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(P6_3_LED_0, mode);
}


/*******************************************************************************
* Function Name: P6_3_LED_Read
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
*  Macro P6_3_LED_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 P6_3_LED_Read(void) 
{
    return (P6_3_LED_PS & P6_3_LED_MASK) >> P6_3_LED_SHIFT;
}


/*******************************************************************************
* Function Name: P6_3_LED_ReadDataReg
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
uint8 P6_3_LED_ReadDataReg(void) 
{
    return (P6_3_LED_DR & P6_3_LED_MASK) >> P6_3_LED_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(P6_3_LED_INTSTAT) 

    /*******************************************************************************
    * Function Name: P6_3_LED_ClearInterrupt
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
    uint8 P6_3_LED_ClearInterrupt(void) 
    {
        return (P6_3_LED_INTSTAT & P6_3_LED_MASK) >> P6_3_LED_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
