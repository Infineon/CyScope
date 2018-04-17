/*******************************************************************************
* File Name: PWM_Sample_Buffer_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWM_Sample_Buffer.h"

static PWM_Sample_Buffer_backupStruct PWM_Sample_Buffer_backup;


/*******************************************************************************
* Function Name: PWM_Sample_Buffer_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_Sample_Buffer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_Sample_Buffer_SaveConfig(void) 
{

    #if(!PWM_Sample_Buffer_UsingFixedFunction)
        #if(!PWM_Sample_Buffer_PWMModeIsCenterAligned)
            PWM_Sample_Buffer_backup.PWMPeriod = PWM_Sample_Buffer_ReadPeriod();
        #endif /* (!PWM_Sample_Buffer_PWMModeIsCenterAligned) */
        PWM_Sample_Buffer_backup.PWMUdb = PWM_Sample_Buffer_ReadCounter();
        #if (PWM_Sample_Buffer_UseStatus)
            PWM_Sample_Buffer_backup.InterruptMaskValue = PWM_Sample_Buffer_STATUS_MASK;
        #endif /* (PWM_Sample_Buffer_UseStatus) */

        #if(PWM_Sample_Buffer_DeadBandMode == PWM_Sample_Buffer__B_PWM__DBM_256_CLOCKS || \
            PWM_Sample_Buffer_DeadBandMode == PWM_Sample_Buffer__B_PWM__DBM_2_4_CLOCKS)
            PWM_Sample_Buffer_backup.PWMdeadBandValue = PWM_Sample_Buffer_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_Sample_Buffer_KillModeMinTime)
             PWM_Sample_Buffer_backup.PWMKillCounterPeriod = PWM_Sample_Buffer_ReadKillTime();
        #endif /* (PWM_Sample_Buffer_KillModeMinTime) */

        #if(PWM_Sample_Buffer_UseControl)
            PWM_Sample_Buffer_backup.PWMControlRegister = PWM_Sample_Buffer_ReadControlRegister();
        #endif /* (PWM_Sample_Buffer_UseControl) */
    #endif  /* (!PWM_Sample_Buffer_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_Sample_Buffer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_Sample_Buffer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_Sample_Buffer_RestoreConfig(void) 
{
        #if(!PWM_Sample_Buffer_UsingFixedFunction)
            #if(!PWM_Sample_Buffer_PWMModeIsCenterAligned)
                PWM_Sample_Buffer_WritePeriod(PWM_Sample_Buffer_backup.PWMPeriod);
            #endif /* (!PWM_Sample_Buffer_PWMModeIsCenterAligned) */

            PWM_Sample_Buffer_WriteCounter(PWM_Sample_Buffer_backup.PWMUdb);

            #if (PWM_Sample_Buffer_UseStatus)
                PWM_Sample_Buffer_STATUS_MASK = PWM_Sample_Buffer_backup.InterruptMaskValue;
            #endif /* (PWM_Sample_Buffer_UseStatus) */

            #if(PWM_Sample_Buffer_DeadBandMode == PWM_Sample_Buffer__B_PWM__DBM_256_CLOCKS || \
                PWM_Sample_Buffer_DeadBandMode == PWM_Sample_Buffer__B_PWM__DBM_2_4_CLOCKS)
                PWM_Sample_Buffer_WriteDeadTime(PWM_Sample_Buffer_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_Sample_Buffer_KillModeMinTime)
                PWM_Sample_Buffer_WriteKillTime(PWM_Sample_Buffer_backup.PWMKillCounterPeriod);
            #endif /* (PWM_Sample_Buffer_KillModeMinTime) */

            #if(PWM_Sample_Buffer_UseControl)
                PWM_Sample_Buffer_WriteControlRegister(PWM_Sample_Buffer_backup.PWMControlRegister);
            #endif /* (PWM_Sample_Buffer_UseControl) */
        #endif  /* (!PWM_Sample_Buffer_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_Sample_Buffer_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_Sample_Buffer_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_Sample_Buffer_Sleep(void) 
{
    #if(PWM_Sample_Buffer_UseControl)
        if(PWM_Sample_Buffer_CTRL_ENABLE == (PWM_Sample_Buffer_CONTROL & PWM_Sample_Buffer_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_Sample_Buffer_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_Sample_Buffer_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_Sample_Buffer_UseControl) */

    /* Stop component */
    PWM_Sample_Buffer_Stop();

    /* Save registers configuration */
    PWM_Sample_Buffer_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_Sample_Buffer_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_Sample_Buffer_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_Sample_Buffer_Wakeup(void) 
{
     /* Restore registers values */
    PWM_Sample_Buffer_RestoreConfig();

    if(PWM_Sample_Buffer_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_Sample_Buffer_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
