/*******************************************************************************
* File Name: PRS_PM.c
* Version 2.40
*
* Description:
*  This file provides Sleep APIs for PRS component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "PRS.h"

#if ((PRS_TIME_MULTIPLEXING_ENABLE) && (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK))
    uint8 PRS_sleepState = PRS_NORMAL_SEQUENCE;    
#endif  /* End ((PRS_TIME_MULTIPLEXING_ENABLE) && 
          (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK)) */


/*******************************************************************************
* Function Name: PRS_SaveConfig
********************************************************************************
*
* Summary:
*  Saves seed and polynomial registers.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global Variables:
*  PRS_backup - modified when non-retention registers are saved.
*
*******************************************************************************/
void PRS_SaveConfig(void) 
{    
    #if (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK)
        /* Save dff register for time mult */
        #if (PRS_TIME_MULTIPLEXING_ENABLE)
            PRS_backup.dffStatus = PRS_STATUS;
            /* Clear normal dff sequence set */
            PRS_sleepState &= ((uint8)~PRS_NORMAL_SEQUENCE);
        #endif  /* End PRS_TIME_MULTIPLEXING_ENABLE */
        
        /* Save A0 and A1 registers */
        #if (PRS_PRS_SIZE <= 32u)
            PRS_backup.seed = PRS_Read();
            
        #else
            PRS_backup.seedUpper = PRS_ReadUpper();
            PRS_backup.seedLower = PRS_ReadLower();
            
        #endif     /* End (PRS_PRS_SIZE <= 32u) */
        
    #endif  /* End (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK) */
}


/*******************************************************************************
* Function Name: PRS_Sleep
********************************************************************************
*
* Summary:
*  Stops PRS computation and saves PRS configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global Variables:
*  PRS_backup - modified when non-retention registers are saved.
*
*******************************************************************************/
void PRS_Sleep(void) 
{
    /* Store PRS enable state */
    if(PRS_IS_PRS_ENABLE(PRS_CONTROL_REG))
    {
        PRS_backup.enableState = 1u;
        PRS_Stop();
    }
    else
    {
        PRS_backup.enableState = 0u;
    }
    
    PRS_SaveConfig();
}


/*******************************************************************************
* Function Name: PRS_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores seed and polynomial registers.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
#if (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK)
    #if (PRS_TIME_MULTIPLEXING_ENABLE)
        void PRS_RestoreConfig(void) 
        {   
            /* Restore A0 and A1 registers */
            #if (PRS_PRS_SIZE <= 32u)
                PRS_WriteSeed(PRS_backup.seed);
            #else
                PRS_WriteSeedUpper(PRS_backup.seedUpper);
                PRS_WriteSeedLower(PRS_backup.seedLower);
            #endif  /* End (PRS_PRS_SIZE <= 32u) */
            
            #if (PRS_RUN_MODE == PRS__CLOCKED)
                #if (PRS_PRS_SIZE <= 32u)
                    PRS_ResetSeedInit(PRS_DEFAULT_SEED);                        
                #else
                    PRS_ResetSeedInitUpper(PRS_DEFAULT_SEED_UPPER);
                    PRS_ResetSeedInitLower(PRS_DEFAULT_SEED_LOWER); 
                #endif  /* End (PRS_PRS_SIZE <= 32u) */ 
            #endif  /* End (PRS_RUN_MODE == PRS__CLOCKED) */
            
            /* Restore dff state for time mult: use async set/reest */
            /* Set CI, SI, SO, STATE0, STATE1 */
            PRS_CONTROL_REG = PRS_backup.dffStatus;
            
            /* Make pulse, to set trigger to defined state */
            PRS_EXECUTE_DFF_SET;
            
            /* Set normal dff sequence set */
            PRS_sleepState |= PRS_NORMAL_SEQUENCE;
        }
        
    #else
        void PRS_RestoreConfig(void) 
        {   
            /* Restore A0 and A1 registers */
            #if (PRS_PRS_SIZE <= 32u)
                PRS_WriteSeed(PRS_backup.seed);
            #else
                PRS_WriteSeedUpper(PRS_backup.seedUpper);
                PRS_WriteSeedLower(PRS_backup.seedLower);
            #endif  /* End (PRS_PRS_SIZE <= 32u) */
            
            #if (PRS_RUN_MODE == PRS__CLOCKED)
                #if (PRS_PRS_SIZE <= 32u)
                    PRS_ResetSeedInit(PRS_DEFAULT_SEED);                        
                #else
                    PRS_ResetSeedInitUpper(PRS_DEFAULT_SEED_UPPER);
                    PRS_ResetSeedInitLower(PRS_DEFAULT_SEED_LOWER); 
                #endif  /* End (PRS_PRS_SIZE <= 32u) */ 
            #endif  /* End (PRS_RUN_MODE == PRS__CLOCKED) */
        }
        
    #endif  /* End (PRS_TIME_MULTIPLEXING_ENABLE) */
    
#else
    void PRS_RestoreConfig(void) 
    {
        PRS_Init();
    }
    
#endif  /* End (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK) */


/*******************************************************************************
* Function Name: PRS_Wakeup
********************************************************************************
*
* Summary:
*  Restores PRS configuration and starts PRS computation. 
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
#if ((PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK) && (PRS_TIME_MULTIPLEXING_ENABLE))
    void PRS_Wakeup(void) 
#else
    void PRS_Wakeup(void) 
#endif  /* End ((PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK) && 
                (PRS_TIME_MULTIPLEXING_ENABLE)) */
{
    PRS_RestoreConfig();
    
    /* Restore PRS enable state */
    if (PRS_backup.enableState != 0u)
    {
        PRS_Enable();
    }
}

/* [] END OF FILE */
