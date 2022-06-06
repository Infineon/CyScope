/*******************************************************************************
* File Name: ADC_Clock_Pin.c  
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
#include "ADC_Clock_Pin.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 ADC_Clock_Pin__PORT == 15 && ((ADC_Clock_Pin__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: ADC_Clock_Pin_Write
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
void ADC_Clock_Pin_Write(uint8 value) 
{
    uint8 staticBits = (ADC_Clock_Pin_DR & (uint8)(~ADC_Clock_Pin_MASK));
    ADC_Clock_Pin_DR = staticBits | ((uint8)(value << ADC_Clock_Pin_SHIFT) & ADC_Clock_Pin_MASK);
}


/*******************************************************************************
* Function Name: ADC_Clock_Pin_SetDriveMode
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
void ADC_Clock_Pin_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(ADC_Clock_Pin_0, mode);
}


/*******************************************************************************
* Function Name: ADC_Clock_Pin_Read
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
*  Macro ADC_Clock_Pin_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 ADC_Clock_Pin_Read(void) 
{
    return (ADC_Clock_Pin_PS & ADC_Clock_Pin_MASK) >> ADC_Clock_Pin_SHIFT;
}


/*******************************************************************************
* Function Name: ADC_Clock_Pin_ReadDataReg
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
uint8 ADC_Clock_Pin_ReadDataReg(void) 
{
    return (ADC_Clock_Pin_DR & ADC_Clock_Pin_MASK) >> ADC_Clock_Pin_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(ADC_Clock_Pin_INTSTAT) 

    /*******************************************************************************
    * Function Name: ADC_Clock_Pin_ClearInterrupt
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
    uint8 ADC_Clock_Pin_ClearInterrupt(void) 
    {
        return (ADC_Clock_Pin_INTSTAT & ADC_Clock_Pin_MASK) >> ADC_Clock_Pin_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
