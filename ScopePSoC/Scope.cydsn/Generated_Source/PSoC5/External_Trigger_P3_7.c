/*******************************************************************************
* File Name: External_Trigger_P3_7.c  
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
#include "External_Trigger_P3_7.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 External_Trigger_P3_7__PORT == 15 && ((External_Trigger_P3_7__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: External_Trigger_P3_7_Write
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
void External_Trigger_P3_7_Write(uint8 value) 
{
    uint8 staticBits = (External_Trigger_P3_7_DR & (uint8)(~External_Trigger_P3_7_MASK));
    External_Trigger_P3_7_DR = staticBits | ((uint8)(value << External_Trigger_P3_7_SHIFT) & External_Trigger_P3_7_MASK);
}


/*******************************************************************************
* Function Name: External_Trigger_P3_7_SetDriveMode
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
void External_Trigger_P3_7_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(External_Trigger_P3_7_0, mode);
}


/*******************************************************************************
* Function Name: External_Trigger_P3_7_Read
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
*  Macro External_Trigger_P3_7_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 External_Trigger_P3_7_Read(void) 
{
    return (External_Trigger_P3_7_PS & External_Trigger_P3_7_MASK) >> External_Trigger_P3_7_SHIFT;
}


/*******************************************************************************
* Function Name: External_Trigger_P3_7_ReadDataReg
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
uint8 External_Trigger_P3_7_ReadDataReg(void) 
{
    return (External_Trigger_P3_7_DR & External_Trigger_P3_7_MASK) >> External_Trigger_P3_7_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(External_Trigger_P3_7_INTSTAT) 

    /*******************************************************************************
    * Function Name: External_Trigger_P3_7_ClearInterrupt
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
    uint8 External_Trigger_P3_7_ClearInterrupt(void) 
    {
        return (External_Trigger_P3_7_INTSTAT & External_Trigger_P3_7_MASK) >> External_Trigger_P3_7_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
