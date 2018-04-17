/*******************************************************************************
* File Name: KEES_Mod.c  
* Version 1.0
*
* Description:
*  This file provides the source code to the API for the ADC 
*  User Module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2011, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "KEES_Mod.h"

uint8 KEES_Mod_initVar = 0u;

/*******************************************************************************   
* Function Name: KEES_Mod_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  KEES_Mod_Start().
*
* Parameters:
*  void
*
* Return:
*  void
*
* Reentrant:
*  No
* 
*******************************************************************************/
void KEES_Mod_Init(void)
{
    /* Set DelSig mode */
    KEES_Mod_CR0_REG = 
        (KEES_Mod_MODE_1ST_MOD << KEES_Mod_MODE_SHIFT);
	
    // set the input range
	KEES_Mod_CR1_REG = 
        (1 << KEES_Mod_GAIN_SHIFT) | 
        (KEES_Mod_DRIVE_400UA << KEES_Mod_DRIVE_SHIFT) | 
        (KEES_Mod_COMP_3P6PF << KEES_Mod_COMP_SHIFT);
    
    // set bias control and compensetaion
	KEES_Mod_CR2_REG = 
        (KEES_Mod_BIAS_CTL_2X << KEES_Mod_BIAS_CTL_SHIFT);
    
    // enable the clock source and enable dynamic control for modulator reset
	KEES_Mod_CLK_REG |= 
        (KEES_Mod_CLK_EN_ENABLE << KEES_Mod_CLK_EN_SHIFT) | 
        (KEES_Mod_DYN_CNTL_EN_ENABLE << KEES_Mod_DYN_CNTL_EN_SHIFT);
}

/*******************************************************************************   
* Function Name: KEES_Mod_Enable
********************************************************************************
*
* Summary:
*  Enables the ADC block operation
*
* Parameters:
*  void
*l
* Return:
*  void
*
* Reentrant:
*  No
* 
*******************************************************************************/
void KEES_Mod_Enable(void)
{
    // enable the internal pump clock for the SC/CT block
	KEES_Mod_PUMP_CR1_REG |= (KEES_Mod_PUMP_INTERNAL << KEES_Mod_PUMP_SHIFT);
	
	 /* Enable power to the Amp in Active mode*/
    KEES_Mod_PM_ACT_CFG_REG |= KEES_Mod_ACT_PWR_EN;
    
    /* Enable power to the Amp in Alt Active mode*/
    KEES_Mod_PM_STBY_CFG_REG |= KEES_Mod_STBY_PWR_EN;
	
	#if (CYDEV_VARIABLE_VDDA == 1u)
        if(CyScPumpEnabled == 1u)
        {
            KEES_Mod_BST_REG &= (uint8)(~KEES_Mod_MX_BST_CLK_MASK);
            KEES_Mod_BST_REG |= (uint8)(KEES_Mod_BST_CLK_EN_ENABLE << KEES_Mod_BST_CLK_EN_SHIFT) | CyScBoostClk__INDEX;
            KEES_Mod_SC_MISC_REG |= (uint8)(KEES_Mod_SC_PUMP_FORCE << KEES_Mod_SC_PUMP_FORCE_SHIFT);
            CyScBoostClk_Start();
        }
        else
        {
            KEES_Mod_BST_REG &= (uint8)(~KEES_Mod_MX_BST_CLK_MASK);
            KEES_Mod_SC_MISC_REG &= (uint8)(~KEES_Mod_SC_PUMP_FORCE_MASK);
        }
    #endif
}

/*******************************************************************************
* Function Name: KEES_Mod_Start
********************************************************************************
*
* Summary:
*  The start function initializes the PGA with the default values, and sets
*  the power to the given level.  A power level of 0, is the same as executing
*  the stop function.
*
* Parameters:  
*  void
*
* Return: 
*  void 
*
* Reentrant: 
*  No
*
*******************************************************************************/
void KEES_Mod_Start(void) 
{   

    if(KEES_Mod_initVar == 0u)
    {
        KEES_Mod_Init();
        KEES_Mod_initVar = 1u;
    }
    
    
    KEES_Mod_Enable();

	
}

/* [] END OF FILE */