/*******************************************************************************
* File Name: Trigger_Comp.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the Comparator component
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Trigger_Comp.h"

uint8 Trigger_Comp_initVar = 0u;

/* Internal functions definitoin */
static void Trigger_Comp_trimAdjust(uint8 nibble) ;

/* static Trigger_Comp_backupStruct  Trigger_Comp_backup; */
#if (CY_PSOC5A)
    static Trigger_Comp_LOWPOWER_BACKUP_STRUCT  Trigger_Comp_lowPowerBackup;
#endif /* CY_PSOC5A */

/* variable to decide whether or not to restore the control register in 
   Enable() API for PSoC5A only */
#if (CY_PSOC5A)
    static uint8 Trigger_Comp_restoreReg = 0u;
#endif /* CY_PSOC5A */


/*******************************************************************************
* Function Name: Trigger_Comp_Init
********************************************************************************
*
* Summary:
*  Initialize to the schematic state
* 
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Trigger_Comp_Init(void) 
{
    /* Set default speed/power */
    Trigger_Comp_SetSpeed(Trigger_Comp_DEFAULT_SPEED);

    /* Set default Hysteresis */
    #if ( Trigger_Comp_DEFAULT_HYSTERESIS == 0u )
        Trigger_Comp_CR |= Trigger_Comp_HYST_OFF;
    #else
        Trigger_Comp_CR &= (uint8)(~Trigger_Comp_HYST_OFF);
    #endif /* Trigger_Comp_DEFAULT_HYSTERESIS == 0u */
    /* Power down override feature is not supported for PSoC5A. */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Set default Power Down Override */
        #if ( Trigger_Comp_DEFAULT_PWRDWN_OVRD == 0u )
            Trigger_Comp_CR &= (uint8)(~Trigger_Comp_PWRDWN_OVRD);
        #else 
            Trigger_Comp_CR |= Trigger_Comp_PWRDWN_OVRD;
        #endif /* Trigger_Comp_DEFAULT_PWRDWN_OVRD == 0u */
    #endif /* CY_PSOC3 || CY_PSOC5LP */
    
    /* Set mux always on logic */
    Trigger_Comp_CR |= Trigger_Comp_MX_AO;

    /* Set default sync */
    Trigger_Comp_CLK &= (uint8)(~Trigger_Comp_SYNCCLK_MASK);
    #if ( Trigger_Comp_DEFAULT_BYPASS_SYNC == 0u )
        Trigger_Comp_CLK |= Trigger_Comp_SYNC_CLK_EN;
    #else
        Trigger_Comp_CLK |= Trigger_Comp_BYPASS_SYNC;
    #endif /* Trigger_Comp_DEFAULT_BYPASS_SYNC == 0u */
}


/*******************************************************************************
* Function Name: Trigger_Comp_Enable
********************************************************************************
*
* Summary:
*  Enable the Comparator
* 
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Trigger_Comp_Enable(void) 
{
    Trigger_Comp_PWRMGR |= Trigger_Comp_ACT_PWR_EN;
    Trigger_Comp_STBY_PWRMGR |= Trigger_Comp_STBY_PWR_EN;
     
     /* This is to restore the value of register CR which is saved 
    in prior to the modification in stop() API */
    #if (CY_PSOC5A)
        if(Trigger_Comp_restoreReg == 1u)
        {
            Trigger_Comp_CR = Trigger_Comp_lowPowerBackup.compCRReg;

            /* Clear the flag */
            Trigger_Comp_restoreReg = 0u;
        }
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: Trigger_Comp_Start
********************************************************************************
*
* Summary:
*  The start function initializes the Analog Comparator with the default values.
*
* Parameters:
*  void
*
* Return:
*  void 
*
* Global variables:
*  Trigger_Comp_initVar: Is modified when this function is called for the 
*   first time. Is used to ensure that initialization happens only once.
*  
*******************************************************************************/
void Trigger_Comp_Start(void) 
{

    if ( Trigger_Comp_initVar == 0u )
    {
        Trigger_Comp_Init();
        
        Trigger_Comp_initVar = 1u;
    }   

    /* Enable power to comparator */
    Trigger_Comp_Enable();    
}


/*******************************************************************************
* Function Name: Trigger_Comp_Stop
********************************************************************************
*
* Summary:
*  Powers down amplifier to lowest power state.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Trigger_Comp_Stop(void) 
{
    /* Disable power to comparator */
    Trigger_Comp_PWRMGR &= (uint8)(~Trigger_Comp_ACT_PWR_EN);
    Trigger_Comp_STBY_PWRMGR &= (uint8)(~Trigger_Comp_STBY_PWR_EN);
    
    #if (CY_PSOC5A)
        /* Enable the variable */
        Trigger_Comp_restoreReg = 1u;

        /* Save the control register before clearing it */
        Trigger_Comp_lowPowerBackup.compCRReg = Trigger_Comp_CR;
        Trigger_Comp_CR = Trigger_Comp_COMP_REG_CLR;
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: Trigger_Comp_SetSpeed
********************************************************************************
*
* Summary:
*  This function sets the speed of the Analog Comparator. The faster the speed
*  the more power that is used.
*
* Parameters:
*  speed: (uint8) Sets operation mode of Comparator
*
* Return:
*  void
*
*******************************************************************************/
void Trigger_Comp_SetSpeed(uint8 speed) 
{
    /* Clear and Set power level */    
    Trigger_Comp_CR = (Trigger_Comp_CR & (uint8)(~Trigger_Comp_PWR_MODE_MASK)) |
                           (speed & Trigger_Comp_PWR_MODE_MASK);

    /* Set trim value for high speed comparator */
    if(speed == Trigger_Comp_HIGHSPEED)
    {
        /* PSoC5A */
        #if (CY_PSOC5A)
            Trigger_Comp_TR = Trigger_Comp_HS_TRIM_TR0;
        #endif /* CY_PSOC5A */
        
        /* PSoC3, PSoC5LP or later */
        #if (CY_PSOC3 || CY_PSOC5LP) 
            Trigger_Comp_TR0 = Trigger_Comp_HS_TRIM_TR0;
            Trigger_Comp_TR1 = Trigger_Comp_HS_TRIM_TR1;
        #endif /* CY_PSOC3 || CY_PSOC5LP */
    }
    else
    {
    /* PSoC5A */
        #if (CY_PSOC5A)
            Trigger_Comp_TR = Trigger_Comp_LS_TRIM_TR0;
        #endif /* CY_PSOC5A */
        
        /* PSoC3, PSoC5LP or later */
        #if (CY_PSOC3 || CY_PSOC5LP) 
            Trigger_Comp_TR0 = Trigger_Comp_LS_TRIM_TR0;
            Trigger_Comp_TR1 = Trigger_Comp_LS_TRIM_TR1;
        #endif /* CY_PSOC3 || CY_PSOC5LP */
    }

}


/*******************************************************************************
* Function Name: Trigger_Comp_GetCompare
********************************************************************************
*
* Summary:
*  This function returns the comparator output value.
*  This value is not affected by the Polarity parameter.
*  This valuea lways reflects a noninverted state.
*
* Parameters:
*   None
*
* Return:
*  (uint8)  0     - if Pos_Input less than Neg_input
*           non 0 - if Pos_Input greater than Neg_input.
*
*******************************************************************************/
uint8 Trigger_Comp_GetCompare(void) 
{
    return( Trigger_Comp_WRK & Trigger_Comp_CMP_OUT_MASK);
}


/*******************************************************************************
* Function Name: Trigger_Comp_trimAdjust
********************************************************************************
*
* Summary:
*  This function adjusts the value in the low nibble/high nibble of the Analog 
*  Comparator trim register
*
* Parameters:  
*  nibble:
*      0 -- adjusts the value in the low nibble
*      1 -- adjusts the value in the high nibble
*
* Return:
*  None
*
* Theory: 
*  Function assumes comparator block is setup for trim adjust.
*  Intended to be called from Comp_ZeroCal()
* 
* Side Effects:
*  Routine uses a course 1ms delay following each trim adjustment to allow 
*  the comparator output to respond.
*
*******************************************************************************/
static void Trigger_Comp_trimAdjust(uint8 nibble) 
{
    uint8 trimCnt, trimCntMax;
    uint8 cmpState;   

    /* get current state of comparator output */
    cmpState = Trigger_Comp_WRK & Trigger_Comp_CMP_OUT_MASK;
    
    if (nibble == 0u)
    {    
        /* if comparator output is high, negative offset adjust is required */
        if ( cmpState != 0u )
        {
            /* PSoC5A */
            #if (CY_PSOC5A)
                Trigger_Comp_TR |= Trigger_Comp_CMP_TRIM1_DIR;
            #endif /* CY_PSOC5A */
            
            /* PSoC3, PSoC5LP or later */
            #if (CY_PSOC3 || CY_PSOC5LP)
                Trigger_Comp_TR0 |= Trigger_Comp_CMP_TR0_DIR;
            #endif /* CY_PSOC3 || CY_PSOC5LP */
        }
    }
    else
    {
        /* if comparator output is low, positive offset adjust is required */
        if ( cmpState == 0u )
        {
            /* PSoC5A */
            #if (CY_PSOC5A)
                Trigger_Comp_TR |= Trigger_Comp_CMP_TRIM2_DIR; 
            #endif /* CY_PSOC5A */
            
            /* PSoC3, PSoC5LP or later */
            #if (CY_PSOC3 || CY_PSOC5LP)
                Trigger_Comp_TR1 |= Trigger_Comp_CMP_TR1_DIR;
            #endif /* CY_PSOC3 || CY_PSOC5LP */
        }
    }

    /* Increment trim value until compare output changes state */
	
    /* PSoC5A */
	#if (CY_PSOC5A)
	    trimCntMax = 7u;
    #endif
	
	/* PSoC3, PSoC5LP or later */
	#if (CY_PSOC3 || CY_PSOC5LP)
    	if(nibble == 0u)
    	{
    		trimCntMax = 15u;
    	}
    	else
    	{
    		trimCntMax = 7u;
    	}
	#endif
	
    for ( trimCnt = 0u; trimCnt < trimCntMax; trimCnt++ )
	{
        if (nibble == 0u)
        {
            /* PSoC5A */
            #if (CY_PSOC5A)
                Trigger_Comp_TR += 1u;
            #endif /* CY_PSOC5A */
            
            /* PSoC3, PSoC5LP or later */
            #if (CY_PSOC3 || CY_PSOC5LP)
                Trigger_Comp_TR0 += 1u;
            #endif /* CY_PSOC3 || CY_PSOC5LP */
        }
        else
        {
            /* PSoC5A */
            #if (CY_PSOC5A)
                Trigger_Comp_TR += 0x10u;
            #endif /* CY_PSOC5A */
            
            /* PSoC3, PSoC5LP or later */
            #if (CY_PSOC3 || CY_PSOC5LP)
                Trigger_Comp_TR1 += 1u;
            #endif /* CY_PSOC3 || CY_PSOC5LP */
        }
        
        CyDelayUs(10u);
        
        /* Check for change in comparator output */
        if ((Trigger_Comp_WRK & Trigger_Comp_CMP_OUT_MASK) != cmpState)
        {
            break;      /* output changed state, trim phase is complete */
        }        
    }    
}


/*******************************************************************************
* Function Name: Trigger_Comp_ZeroCal
********************************************************************************
*
* Summary:
*  This function calibrates the offset of the Analog Comparator.
*
* Parameters:
*  None
*
* Return:
*  (uint16)  value written in trim register when calibration complete.
*
* Theory: 
*  This function is used to optimize the calibration for user specific voltage
*  range.  The comparator trim is adjusted to counter transistor offsets
*   - offset is defined as positive if the output transitions to high before inP
*     is greater than inN
*   - offset is defined as negative if the output transitions to high after inP
*     is greater than inP
*
*  PSoC5A
*  The Analog Comparator provides 1 byte for offset trim.  The byte contains two
*  4 bit trim fields - one is a course trim and the other allows smaller
*  offset adjustments only for slow modes.
*  - low nibble - fine trim
*  - high nibble - course trim
*  PSoC3, PSoC5LP or later
*  The Analog Comparator provides 2 bytes for offset trim.  The bytes contain two
*  5 bit trim fields - one is a course trim and the other allows smaller
*  offset adjustments only for slow modes.
*  - TR0 - fine trim
*  - TR1 - course trim
*
*  Trim algorithm is a two phase process
*  The first phase performs course offset adjustment
*  The second phase serves one of two purposes depending on the outcome of the
*  first phase
*  - if the first trim value was maxed out without a comparator output 
*    transition, more offset will be added by adjusting the second trim value.
*  - if the first trim phase resulted in a comparator output transition, the
*    second trim value will serve as fine trim (in the opposite direction)to
*    ensure the offset is < 1 mV.
*
* Trim Process:   
*  1) User applies a voltage to the negative input.  Voltage should be in the
*     comparator operating range or an average of the operating voltage range.
*  2) Clear registers associated with analog routing to the positive input.
*  3) Disable Hysteresis
*  4) Set the calibration bit to short the negative and positive inputs to
*     the users calibration voltage.
*  5) Clear the TR register  ( TR = 0x00 )
*  ** LOW MODES
*  6) Check if compare output is high, if so, set the MSb of course trim field 
*     to a 1.
*  7) Increment the course trim field until the compare output changes
*  8) Check if compare output is low, if so, set the MSb of fine trim field
*     to a 1.
*  9) Increment the fine trim field until the compare output changes
*  ** FAST MODE - skip the steps 8,9
*
* Side Effects:
*  Routine clears analog routing associated with the comparator positive input.  
*  This may affect routing of signals from other components that are connected
*  to the positive input of the comparator.
*
*******************************************************************************/
uint16 Trigger_Comp_ZeroCal(void) 
{
    uint8 tmpSW0;
    uint8 tmpSW2;
    uint8 tmpSW3;
    uint8 tmpCR;

    /* Save a copy of routing registers associated with inP */
    tmpSW0 = Trigger_Comp_SW0;
    tmpSW2 = Trigger_Comp_SW2;
    tmpSW3 = Trigger_Comp_SW3;

     /* Clear routing for inP, retain routing for inN */
    Trigger_Comp_SW0 = 0x00u;
    Trigger_Comp_SW2 = 0x00u;
    Trigger_Comp_SW3 = tmpSW3 & (uint8)(~Trigger_Comp_CMP_SW3_INPCTL_MASK);

    /* Preserve original configuration
     * - turn off Hysteresis
     * - set calibration bit - shorts inN to inP
    */
    tmpCR = Trigger_Comp_CR;
    Trigger_Comp_CR |= (Trigger_Comp_CAL_ON | Trigger_Comp_HYST_OFF);
    
    /* Write default low values to trim register - no offset adjust */
    /* PSoC5A */
    #if (CY_PSOC5A)
        Trigger_Comp_TR = Trigger_Comp_DEFAULT_CMP_TRIM;
    #endif /* CY_PSOC5A */
    
    /* PSoC3, PSoC5LP or later */
    #if (CY_PSOC3 || CY_PSOC5LP)
        Trigger_Comp_TR0 = Trigger_Comp_DEFAULT_CMP_TRIM;
        Trigger_Comp_TR1 = Trigger_Comp_DEFAULT_CMP_TRIM;
    #endif /* CY_PSOC3 || CY_PSOC5LP */
	
	/* Two phase trim - slow modes, one phase trim - for fast */ 
    if ( (Trigger_Comp_CR & Trigger_Comp_PWR_MODE_MASK) == Trigger_Comp_PWR_MODE_FAST)
    {
        Trigger_Comp_trimAdjust(0u);
    }
    else /* default to trim for fast modes */
    {
        Trigger_Comp_trimAdjust(1u);
		Trigger_Comp_trimAdjust(0u);
    }
   
    /* Restore Config Register */
    Trigger_Comp_CR = tmpCR;
    
    /* Restore routing registers for inP */
    Trigger_Comp_SW0 = tmpSW0;
    Trigger_Comp_SW2 = tmpSW2;
    Trigger_Comp_SW3 = tmpSW3;
    
    /* PSoC5A */
    #if (CY_PSOC5A)
        return (uint16) Trigger_Comp_TR;
    #endif /* CY_PSOC5A */
    
    /* PSoC3, PSoC5LP or later */
    #if (CY_PSOC3 || CY_PSOC5LP)
        return ((uint16)((uint16)Trigger_Comp_TR1 << 8u) | (Trigger_Comp_TR0));        
    #endif /* CY_PSOC3 || CY_PSOC5LP */
}


/*******************************************************************************
* Function Name: Trigger_Comp_LoadTrim
********************************************************************************
*
* Summary:
*  This function stores a value in the Analog Comparator trim register.
*
* Parameters:  
*  uint8 trimVal - trim value.  This value is the same format as the value 
*  returned by the _ZeroCal routine.
*
* Return:
*  None
*
*******************************************************************************/
void Trigger_Comp_LoadTrim(uint16 trimVal) 
{
    /* Stores value in the Analog Comparator trim register */
    /* PSoC5A */
    #if (CY_PSOC5A)
        Trigger_Comp_TR = (uint8) trimVal;
    #endif /* CY_PSOC5A */
    
    /* PSoC3, PSoC5LP or later */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Stores value in the Analog Comparator trim register for P-type load */
        Trigger_Comp_TR0 = (uint8) trimVal;
        
        /* Stores value in the Analog Comparator trim register for N-type load */
        Trigger_Comp_TR1 = (uint8) (trimVal >> 8); 
    #endif /* CY_PSOC3 || CY_PSOC5LP */
}


#if (CY_PSOC3 || CY_PSOC5LP)

    /*******************************************************************************
    * Function Name: Trigger_Comp_PwrDwnOverrideEnable
    ********************************************************************************
    *
    * Summary:
    *  This is the power down over-ride feature. This function ignores sleep 
    *  parameter and allows the component to stay active during sleep mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Trigger_Comp_PwrDwnOverrideEnable(void) 
    {
        /* Set the pd_override bit in CMP_CR register */
        Trigger_Comp_CR |= Trigger_Comp_PWRDWN_OVRD;
    }


    /*******************************************************************************
    * Function Name: Trigger_Comp_PwrDwnOverrideDisable
    ********************************************************************************
    *
    * Summary:
    *  This is the power down over-ride feature. This allows the component to stay
    *  inactive during sleep.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void Trigger_Comp_PwrDwnOverrideDisable(void) 
    {
        /* Reset the pd_override bit in CMP_CR register */
        Trigger_Comp_CR &= (uint8)(~Trigger_Comp_PWRDWN_OVRD);
    }
#endif /* (CY_PSOC3 || CY_PSOC5LP) */


/* [] END OF FILE */
