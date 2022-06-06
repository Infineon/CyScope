/*******************************************************************************
* File Name: ADC_2_PM.c
* Version 3.10
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_2.h"


/***************************************
* Local data allocation
***************************************/

static ADC_2_BACKUP_STRUCT  ADC_2_backup =
{
    ADC_2_DISABLED
};


/*******************************************************************************
* Function Name: ADC_2_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_2_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_2_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void ADC_2_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: ADC_2_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred routine to prepare the component for sleep.
*  The ADC_2_Sleep() routine saves the current component state,
*  then it calls the ADC_Stop() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_2_backup - The structure field 'enableState' is modified
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void ADC_2_Sleep(void)
{
    if((ADC_2_PWRMGR_SAR_REG  & ADC_2_ACT_PWR_SAR_EN) != 0u)
    {
        if((ADC_2_SAR_CSR0_REG & ADC_2_SAR_SOF_START_CONV) != 0u)
        {
            ADC_2_backup.enableState = ADC_2_ENABLED | ADC_2_STARTED;
        }
        else
        {
            ADC_2_backup.enableState = ADC_2_ENABLED;
        }
        ADC_2_Stop();
    }
    else
    {
        ADC_2_backup.enableState = ADC_2_DISABLED;
    }
}


/*******************************************************************************
* Function Name: ADC_2_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred routine to restore the component to the state when
*  ADC_2_Sleep() was called. If the component was enabled before the
*  ADC_2_Sleep() function was called, the
*  ADC_2_Wakeup() function also re-enables the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ADC_2_backup - The structure field 'enableState' is used to
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void ADC_2_Wakeup(void)
{
    if(ADC_2_backup.enableState != ADC_2_DISABLED)
    {
        ADC_2_Enable();
        #if(ADC_2_DEFAULT_CONV_MODE != ADC_2__HARDWARE_TRIGGER)
            if((ADC_2_backup.enableState & ADC_2_STARTED) != 0u)
            {
                ADC_2_StartConvert();
            }
        #endif /* End ADC_2_DEFAULT_CONV_MODE != ADC_2__HARDWARE_TRIGGER */
    }
}


/* [] END OF FILE */
