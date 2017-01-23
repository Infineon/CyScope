/*******************************************************************************
* File Name: PRS.c
* Version 2.40
*
* Description:
*  This file provides the source code to the API for the PRS component
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

uint8 PRS_initVar = 0u;

PRS_BACKUP_STRUCT PRS_backup =
{
    0x00u, /* enableState; */

    #if (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK)
        /* Save dff register for time mult */
        #if (PRS_TIME_MULTIPLEXING_ENABLE)
            PRS_INIT_STATE, /* dffStatus; */
        #endif  /* End PRS_TIME_MULTIPLEXING_ENABLE */

        /* Save A0 and A1 registers are none-retention */
        #if(PRS_PRS_SIZE <= 32u)
            PRS_DEFAULT_SEED, /* seed */

        #else
            PRS_DEFAULT_SEED_UPPER, /* seedUpper; */
            PRS_DEFAULT_SEED_LOWER, /* seedLower; */

        #endif  /* End (PRS_PRS_SIZE <= 32u) */

    #endif  /* End (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK) */
};


/*******************************************************************************
* Function Name: PRS_Init
********************************************************************************
*
* Summary:
*  Initializes seed and polynomial registers with initial values.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void PRS_Init(void) 
{
    /* Writes seed value and ponynom value provided for customizer */
    #if (PRS_PRS_SIZE <= 32u)
        PRS_WritePolynomial(PRS_DEFAULT_POLYNOM);
        PRS_WriteSeed(PRS_DEFAULT_SEED);
        #if (PRS_RUN_MODE == PRS__CLOCKED)
            PRS_ResetSeedInit(PRS_DEFAULT_SEED);
        #endif  /* End (PRS_RUN_MODE == PRS__CLOCKED) */
            PRS_Enable();
    #else
        PRS_WritePolynomialUpper(PRS_DEFAULT_POLYNOM_UPPER);
        PRS_WritePolynomialLower(PRS_DEFAULT_POLYNOM_LOWER);
        PRS_WriteSeedUpper(PRS_DEFAULT_SEED_UPPER);
        PRS_WriteSeedLower(PRS_DEFAULT_SEED_LOWER);
        #if (PRS_RUN_MODE == PRS__CLOCKED)
            PRS_ResetSeedInitUpper(PRS_DEFAULT_SEED_UPPER);
            PRS_ResetSeedInitLower(PRS_DEFAULT_SEED_LOWER);
        #endif  /* End (PRS_RUN_MODE == PRS__CLOCKED) */
            PRS_Enable();
    #endif  /* End (PRS_PRS_SIZE <= 32u) */
}


/*******************************************************************************
* Function Name: PRS_Enable
********************************************************************************
*
* Summary:
*  Starts PRS computation on rising edge of input clock.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void PRS_Enable(void) 
{
        PRS_CONTROL_REG |= PRS_CTRL_ENABLE;
}


/*******************************************************************************
* Function Name: PRS_Start
********************************************************************************
*
* Summary:
*  Initializes seed and polynomial registers with initial values. Computation
*  of PRS starts on rising edge of input clock.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  PRS_initVar: global variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and set
*  to 1 the first time PRS_Start() is called. This allows
*  enable/disable component without re-initialization in all subsequent calls
*  to the PRS_Start() routine.
*
*******************************************************************************/
void PRS_Start(void) 
{
    /* Writes seed value and ponynom value provided from customizer */
    if (PRS_initVar == 0u)
    {
        PRS_Init();
        PRS_initVar = 1u;
    }

    PRS_Enable();
}


/*******************************************************************************
* Function Name: PRS_Stop
********************************************************************************
*
* Summary:
*  Stops PRS computation.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void PRS_Stop(void) 
{
    PRS_CONTROL_REG &= ((uint8)~PRS_CTRL_ENABLE);
}


#if (PRS_RUN_MODE == PRS__APISINGLESTEP)
    /*******************************************************************************
    * FUNCTION NAME: PRS_Step
    ********************************************************************************
    *
    * Summary:
    *  Increments the PRS by one when API single step mode is used.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  void
    *
    *******************************************************************************/
    void PRS_Step(void) 
    {
        #if (PRS_TIME_MULTIPLEXING_ENABLE)
            /* Makes 4 pulse, 4x for Time Mult */
            PRS_EXECUTE_STEP;
            PRS_EXECUTE_STEP;
            PRS_EXECUTE_STEP;
            PRS_EXECUTE_STEP;

        #else
            /* One pulse without Time mult required */
            PRS_EXECUTE_STEP;

        #endif  /* End PRS_TIME_MULTIPLEXING_ENABLE */
    }
#endif  /* End (PRS_RUN_MODE == PRS__APISINGLESTEP) */


#if (PRS_RUN_MODE == PRS__CLOCKED)
    #if (PRS_PRS_SIZE <= 32u) /* 8-32 bits PRS */
        /*******************************************************************************
        * FUNCTION NAME: PRS_ResetSeedInit
        ********************************************************************************
        *
        * Summary:
        *  Increments the PRS by one when API single step mode is used.
        *
        * Parameters:
        *  void
        *
        * Return:
        *  void
        *
        *******************************************************************************/
        void PRS_ResetSeedInit(uint16 seed)
                                            
        {
            uint8 enableInterrupts;

            /* Mask the Seed to cut unused bits */
            seed &= PRS_MASK;

            /* Change AuxControl reg, need to be safety */
            enableInterrupts = CyEnterCriticalSection();

            #if (PRS_TIME_MULTIPLEXING_ENABLE)
                /* Set FIFOs to single register */
                PRS_AUX_CONTROL_A_REG |= PRS_AUXCTRL_FIFO_SINGLE_REG;

                #if(PRS_PRS_SIZE > 16u)       /* 17-32 bits PRS */
                    PRS_AUX_CONTROL_B_REG |= PRS_AUXCTRL_FIFO_SINGLE_REG;
                #endif  /* End (PRS_PRS_SIZE <= 8u) */

                /* AuxControl reg settings are done */
                CyExitCriticalSection(enableInterrupts);

                /* Write Seed COPY */
                #if (PRS_PRS_SIZE <= 16u)          /* 16 bits PRS */
                    PRS_SEED_COPY_A__A1_REG = HI8(seed);
                    PRS_SEED_COPY_A__A0_REG = LO8(seed);

                #elif (PRS_PRS_SIZE <= 24u)        /* 24 bits PRS */
                    PRS_SEED_COPY_B__A1_REG = LO8(HI16(seed));
                    PRS_SEED_COPY_B__A0_REG = HI8(seed);
                    PRS_SEED_COPY_A__A0_REG = LO8(seed);

                #else                                           /* 32 bits PRS */
                    PRS_SEED_COPY_B__A1_REG = HI8(HI16(seed));
                    PRS_SEED_COPY_A__A1_REG = LO8(HI16(seed));
                    PRS_SEED_COPY_B__A0_REG = HI8(seed);
                    PRS_SEED_COPY_A__A0_REG = LO8(seed);
                #endif  /* End (PRS_PRS_SIZE <= 32u) */

            #else
                /* Set FIFOs to single register */
                #if (PRS_PRS_SIZE <= 8u)      /* 8 bits PRS */
                    PRS_AUX_CONTROL_A_REG |= PRS_AUXCTRL_FIFO_SINGLE_REG;

                #elif (PRS_PRS_SIZE <= 16u)      /* 16 bits PRS */
                    PRS_AUX_CONTROL_A_REG  |= PRS_AUXCTRL_FIFO_SINGLE_REG;
                    PRS_AUX_CONTROL_B_REG  |= PRS_AUXCTRL_FIFO_SINGLE_REG;

                #elif (PRS_PRS_SIZE <= 24u)      /* 24-39 bits PRS */
                    PRS_AUX_CONTROL_A_REG  |= PRS_AUXCTRL_FIFO_SINGLE_REG;
                    PRS_AUX_CONTROL_B_REG  |= PRS_AUXCTRL_FIFO_SINGLE_REG;
                    PRS_AUX_CONTROL_C_REG  |= PRS_AUXCTRL_FIFO_SINGLE_REG;

                #elif (PRS_PRS_SIZE <= 32u)      /* 40-55 bits PRS */
                    PRS_AUX_CONTROL_A_REG  |= PRS_AUXCTRL_FIFO_SINGLE_REG;
                    PRS_AUX_CONTROL_B_REG  |= PRS_AUXCTRL_FIFO_SINGLE_REG;
                    PRS_AUX_CONTROL_C_REG  |= PRS_AUXCTRL_FIFO_SINGLE_REG;
                    PRS_AUX_CONTROL_D_REG  |= PRS_AUXCTRL_FIFO_SINGLE_REG;

                #endif  /* End (PRS_PRS_SIZE <= 8u) */

                /* AuxControl reg setting are done */
                CyExitCriticalSection(enableInterrupts);

                /* Write Seed COPY */
                CY_SET_REG16(PRS_SEED_COPY_PTR, seed);

            #endif  /* End (PRS_TIME_MULTIPLEXING_ENABLE) */
        }

    #else

        /*******************************************************************************
        * FUNCTION NAME: PRS_ResetSeedInitUpper
        ********************************************************************************
        *
        * Summary:
        *  Increments the PRS by one when API single step mode is used.
        *
        * Parameters:
        *  void
        *
        * Return:
        *  void
        *
        *******************************************************************************/
        void PRS_ResetSeedInitUpper(uint32 seed) 
        {
			uint8 enableInterrupts;

			/* Mask the Seed Upper half to cut unused bits */
            seed &= PRS_MASK;

			/* Change AuxControl reg, need to be safety */
            enableInterrupts = CyEnterCriticalSection();

            /* Set FIFOs to single register */
            PRS_AUX_CONTROL_A_REG |= PRS_AUXCTRL_FIFO_SINGLE_REG;
            PRS_AUX_CONTROL_B_REG |= PRS_AUXCTRL_FIFO_SINGLE_REG;
            PRS_AUX_CONTROL_C_REG |= PRS_AUXCTRL_FIFO_SINGLE_REG;

			#if (PRS_PRS_SIZE > 48u)               /* 49-64 bits PRS */
                PRS_AUX_CONTROL_D_REG |= PRS_AUXCTRL_FIFO_SINGLE_REG;
            #endif  /* End (PRS_PRS_SIZE <= 8u) */

            /* AuxControl reg settings are done */
            CyExitCriticalSection(enableInterrupts);

            /* Write Seed Upper COPY */
            #if (PRS_PRS_SIZE <= 40u)          /* 40 bits PRS */
                PRS_SEED_UPPER_COPY_C__A1_REG = LO8(seed);

            #elif (PRS_PRS_SIZE <= 48u)        /* 48 bits PRS */
                PRS_SEED_UPPER_COPY_C__A1_REG = HI8(seed);
                PRS_SEED_UPPER_COPY_B__A1_REG = LO8(seed);

            #elif (PRS_PRS_SIZE <= 56u)        /* 56 bits PRS */
                PRS_SEED_UPPER_COPY_D__A1_REG = LO8(HI16(seed));
                PRS_SEED_UPPER_COPY_C__A1_REG = HI8(seed);
                PRS_SEED_UPPER_COPY_B__A1_REG = HI8(seed);

            #else                                           /* 64 bits PRS */
                PRS_SEED_UPPER_COPY_D__A1_REG = HI8(HI16(seed));
                PRS_SEED_UPPER_COPY_C__A1_REG = LO8(HI16(seed));
                PRS_SEED_UPPER_COPY_B__A1_REG = HI8(seed);
                PRS_SEED_UPPER_COPY_A__A1_REG = LO8(seed);

            #endif  /* End (PRS_PRS_SIZE <= 32u) */
        }


        /*******************************************************************************
        * FUNCTION NAME: PRS_ResetSeedInitLower
        ********************************************************************************
        *
        * Summary:
        *  Increments the PRS by one when API single step mode is used.
        *
        * Parameters:
        *  void
        *
        * Return:
        *  void
        *
        *******************************************************************************/
        void PRS_ResetSeedInitLower(uint32 seed) 
        {
            /* Write Seed Lower COPY */
            #if (PRS_PRS_SIZE <= 40u)          /* 40 bits PRS */
                PRS_SEED_LOWER_COPY_B__A1_REG = HI8(HI16(seed));
                PRS_SEED_LOWER_COPY_C__A0_REG = LO8(HI16(seed));
                PRS_SEED_LOWER_COPY_B__A0_REG = HI8(seed);
                PRS_SEED_LOWER_COPY_A__A0_REG = LO8(seed);

            #elif (PRS_PRS_SIZE <= 48u)        /* 48 bits PRS */
                PRS_SEED_LOWER_COPY_A__A1_REG = HI8(HI16(seed));
                PRS_SEED_LOWER_COPY_C__A0_REG = LO8(HI16(seed));
                PRS_SEED_LOWER_COPY_B__A0_REG = HI8(seed);
                PRS_SEED_LOWER_COPY_A__A0_REG = LO8(seed);

            #else                                           /* 64 bits PRS */
                PRS_SEED_LOWER_COPY_D__A0_REG = HI8(HI16(seed));
                PRS_SEED_LOWER_COPY_C__A0_REG = LO8(HI16(seed));
                PRS_SEED_LOWER_COPY_B__A0_REG = HI8(seed);
                PRS_SEED_LOWER_COPY_A__A0_REG = LO8(seed);

            #endif  /* End (PRS_PRS_SIZE <= 32u) */
        }

    #endif  /* End (PRS_PRS_SIZE <= 32u) */

#endif  /* End (PRS_RUN_MODE == PRS__CLOCKED) */


#if(PRS_PRS_SIZE <= 32u) /* 8-32 bits PRS */
    /*******************************************************************************
    * Function Name: PRS_Read
    ********************************************************************************
    *
    * Summary:
    *  Reads PRS value.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Returns PRS value.
    *
    * Side Effects:
    *  The seed value is cut according to mask = 2^(Resolution) - 1.
    *  For example if PRS Resolution is 14 bits the mask value is:
    *  mask = 2^(14) - 1 = 0x3FFFu.
    *  The seed value = 0xFFFFu is cut:
    *  seed & mask = 0xFFFFu & 0x3FFFu = 0x3FFFu.
    *
    *******************************************************************************/
    uint16 PRS_Read(void) 
    {
        /* Read PRS */
        #if (PRS_TIME_MULTIPLEXING_ENABLE)

            uint16 seed;

            #if (PRS_PRS_SIZE <= 16u)          /* 16 bits PRS */
                seed = ((uint16) PRS_SEED_A__A1_REG) << 8u;
                seed |= PRS_SEED_A__A0_REG;

            #elif (PRS_PRS_SIZE <= 24u)        /* 24 bits PRS */
                seed = ((uint32) (PRS_SEED_B__A1_REG)) << 16u;
                seed |= ((uint32) (PRS_SEED_B__A0_REG)) << 8u;
                seed |= PRS_SEED_A__A0_REG;

            #else                                           /* 32 bits PRS */
                seed = ((uint32) PRS_SEED_B__A1_REG) << 24u;
                seed |= ((uint32) PRS_SEED_A__A1_REG) << 16u;
                seed |= ((uint32) PRS_SEED_B__A0_REG) << 8u;
                seed |= PRS_SEED_A__A0_REG;

            #endif  /* End (PRS_PRS_SIZE <= 8u) */

            return (seed  & PRS_MASK);

        #else

            return (CY_GET_REG16(PRS_SEED_PTR) & PRS_MASK);

        #endif  /* End (PRS_TIME_MULTIPLEXING_ENABLE) */
    }


    /*******************************************************************************
    * Function Name: PRS_WriteSeed
    ********************************************************************************
    *
    * Summary:
    *  Writes seed value.
    *
    * Parameters:
    *  seed:  Seed value.
    *
    * Return:
    *  void
    *
    * Side Effects:
    *  The seed value is cut according to mask = 2^(Resolution) - 1.
    *  For example if PRS Resolution is 14 bits the mask value is:
    *  mask = 2^(14) - 1 = 0x3FFFu.
    *  The seed value = 0xFFFFu is cut:
    *  seed & mask = 0xFFFFu & 0x3FFFu = 0x3FFFu.
    *
    *******************************************************************************/
    void PRS_WriteSeed(uint16 seed) 
    {
        /* Masks the Seed to cut unused bits */
        seed &= PRS_MASK;

        /* Writes Seed */
        #if (PRS_TIME_MULTIPLEXING_ENABLE)

            #if (PRS_PRS_SIZE <= 16u)          /* 16 bits PRS */
                PRS_SEED_A__A1_REG = HI8(seed);
                PRS_SEED_A__A0_REG = LO8(seed);

            #elif (PRS_PRS_SIZE <= 24u)        /* 24 bits PRS */
                PRS_SEED_B__A1_REG = LO8(HI16(seed));
                PRS_SEED_B__A0_REG = HI8(seed);
                PRS_SEED_A__A0_REG = LO8(seed);

            #else                                           /* 32 bits PRS */
                PRS_SEED_B__A1_REG = HI8(HI16(seed));
                PRS_SEED_A__A1_REG = LO8(HI16(seed));
                PRS_SEED_B__A0_REG = HI8(seed);
                PRS_SEED_A__A0_REG = LO8(seed);
            #endif  /* End (PRS_PRS_SIZE <= 32u) */

            /* Resets triggers */
            #if (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK)
                if((PRS_sleepState & PRS_NORMAL_SEQUENCE) != 0u)
                {
                    PRS_EXECUTE_DFF_RESET;
                }
            #else
                PRS_EXECUTE_DFF_RESET;
            #endif  /* (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK) */

        #else

            CY_SET_REG16(PRS_SEED_PTR, seed);

        #endif  /* End (PRS_TIME_MULTIPLEXING_ENABLE) */
    }


    /*******************************************************************************
    * Function Name: PRS_ReadPolynomial
    ********************************************************************************
    *
    * Summary:
    *  Reads PRS polynomial value.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Returns PRS polynomial value.
    *
    *******************************************************************************/
    uint16 PRS_ReadPolynomial(void)
                                
    {
        /* Reads polynomial */
        #if (PRS_TIME_MULTIPLEXING_ENABLE)

            uint16 polynomial;

            #if (PRS_PRS_SIZE <= 16u)          /* 16 bits PRS */
                polynomial = ((uint16) PRS_POLYNOM_A__D1_REG) << 8u;
                polynomial |= (PRS_POLYNOM_A__D0_REG);

            #elif (PRS_PRS_SIZE <= 24u)        /* 24 bits PRS */
                polynomial = ((uint32) PRS_POLYNOM_B__D1_REG) << 16u;
                polynomial |= ((uint32) PRS_POLYNOM_B__D0_REG) << 8u;
                polynomial |= PRS_POLYNOM_A__D0_REG;

            #else                                           /* 32 bits PRS */
                polynomial = ((uint32) PRS_POLYNOM_B__D1_REG) << 24u;
                polynomial |= ((uint32) PRS_POLYNOM_A__D1_REG) << 16u;
                polynomial |= ((uint32) PRS_POLYNOM_B__D0_REG) << 8u;
                polynomial |= PRS_POLYNOM_A__D0_REG;

            #endif  /* End (PRS_PRS_SIZE <= 32u) */

            return polynomial;

        #else

            return CY_GET_REG16(PRS_POLYNOM_PTR);

        #endif  /* End (PRS_TIME_MULTIPLEXING_ENABLE) */
    }


    /*******************************************************************************
    * Function Name: PRS_WritePolynomial
    ********************************************************************************
    *
    * Summary:
    *  Writes PRS polynomial value.
    *
    * Parameters:
    *  polynomial:  PRS polynomial.
    *
    * Return:
    *  void
    *
    * Side Effects:
    *  The polynomial value is cut according to mask = 2^(Resolution) - 1.
    *  For example if PRS Resolution is 14 bits the mask value is:
    *  mask = 2^(14) - 1 = 0x3FFFu.
    *  The polynomial value = 0xFFFFu is cut:
    *  polynomial & mask = 0xFFFFu & 0x3FFFu = 0x3FFFu.
    *
    *******************************************************************************/
    void PRS_WritePolynomial(uint16 polynomial)
                                          
    {
        /* Mask polynomial to cut unused bits */
        polynomial &= PRS_MASK;

        /* Write polynomial */
        #if (PRS_TIME_MULTIPLEXING_ENABLE)

            #if (PRS_PRS_SIZE <= 16u)          /* 16 bits PRS */

                PRS_POLYNOM_A__D1_REG = HI8(polynomial);
                PRS_POLYNOM_A__D0_REG = LO8(polynomial);

            #elif (PRS_PRS_SIZE <= 24u)        /* 24 bits PRS */
                PRS_POLYNOM_B__D1_REG = LO8(HI16(polynomial));
                PRS_POLYNOM_B__D0_REG = HI8(polynomial);
                PRS_POLYNOM_A__D0_REG = LO8(polynomial);

            #else                                           /* 32 bits PRS */
                PRS_POLYNOM_B__D1_REG = HI8(HI16(polynomial));
                PRS_POLYNOM_A__D1_REG = LO8(HI16(polynomial));
                PRS_POLYNOM_B__D0_REG = HI8(polynomial);
                PRS_POLYNOM_A__D0_REG = LO8(polynomial);

            #endif  /* End (PRS_PRS_SIZE <= 32u) */

            /* Resets triggers */
            #if (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK)
                if((PRS_sleepState & PRS_NORMAL_SEQUENCE) != 0u)
                {
                    PRS_EXECUTE_DFF_RESET;
                }
            #else
                PRS_EXECUTE_DFF_RESET;
            #endif  /* (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK) */

        #else

            CY_SET_REG16(PRS_POLYNOM_PTR, polynomial);

        #endif  /* End (PRS_TIME_MULTIPLEXING_ENABLE) */
    }

#else   /* 33-64 bits PRS */

    /*******************************************************************************
    *  Function Name: PRS_ReadUpper
    ********************************************************************************
    *
    * Summary:
    *  Reads upper half of PRS value. Only generated for 33-64-bit PRS.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Returns upper half of PRS value.
    *
    * Side Effects:
    *  The upper half of seed value is cut according to
    *  mask = 2^(Resolution - 32) - 1.
    *  For example if PRS Resolution is 35 bits the mask value is:
    *  2^(35 - 32) - 1 = 2^(3) - 1 = 0x0000 0007u.
    *  The upper half of seed value = 0x0000 00FFu is cut:
    *  upper half of seed & mask = 0x0000 00FFu & 0x0000 0007u = 0x0000 0007u.
    *
    *******************************************************************************/
    uint32 PRS_ReadUpper(void) 
    {
        uint32 seed;

        /* Read PRS Upper */
        #if (PRS_PRS_SIZE <= 40u)          /* 40 bits PRS */
            seed = PRS_SEED_UPPER_C__A1_REG;

        #elif (PRS_PRS_SIZE <= 48u)        /* 48 bits PRS */
            seed = ((uint32) PRS_SEED_UPPER_C__A1_REG) << 8u;
            seed |= PRS_SEED_UPPER_B__A1_REG;

        #elif (PRS_PRS_SIZE <= 56u)        /* 56 bits PRS */
            seed = ((uint32) PRS_SEED_UPPER_D__A1_REG) << 16u;
            seed |= ((uint32) PRS_SEED_UPPER_C__A1_REG) << 8u;
            seed |= PRS_SEED_UPPER_B__A1_REG;

        #else                                           /* 64 bits PRS */
            seed = ((uint32) PRS_SEED_UPPER_D__A1_REG) << 24u;
            seed |= ((uint32) PRS_SEED_UPPER_C__A1_REG) << 16u;
            seed |= ((uint32) PRS_SEED_UPPER_B__A1_REG) << 8u;
            seed |= PRS_SEED_UPPER_A__A1_REG;

        #endif  /* End (PRS_PRS_SIZE <= 32u) */

        return (seed & PRS_MASK);
    }


    /*******************************************************************************
    *  Function Name: PRS_ReadLower
    ********************************************************************************
    *
    * Summary:
    *  Reads lower half of PRS value. Only generated for 33-64-bit PRS.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Returns lower half of PRS value.
    *
    *******************************************************************************/
    uint32 PRS_ReadLower(void) 
    {
        uint32 seed;

        /* Read PRS Lower */
        #if (PRS_PRS_SIZE <= 40u)          /* 40 bits PRS */
            seed = ((uint32) PRS_SEED_LOWER_B__A1_REG) << 24u;
            seed |= ((uint32) PRS_SEED_LOWER_C__A0_REG) << 16u;
            seed |= ((uint32) PRS_SEED_LOWER_B__A0_REG) << 8u;
            seed |= PRS_SEED_LOWER_A__A0_REG;

        #elif (PRS_PRS_SIZE <= 48u)        /* 48 bits PRS */
            seed = ((uint32) PRS_SEED_LOWER_A__A1_REG) << 24u;
            seed |= ((uint32) PRS_SEED_LOWER_C__A0_REG) << 16u;
            seed |= ((uint32) PRS_SEED_LOWER_B__A0_REG) << 8u;
            seed |= PRS_SEED_LOWER_A__A0_REG;

        #else                                           /* 64 bits PRS */
            seed = ((uint32) PRS_SEED_LOWER_D__A0_REG) << 24u;
            seed |= ((uint32) PRS_SEED_LOWER_C__A0_REG) << 16u;
            seed |= ((uint32) PRS_SEED_LOWER_B__A0_REG) << 8u;
            seed |= PRS_SEED_LOWER_A__A0_REG;

        #endif  /* End (PRS_PRS_SIZE <= 32u) */

        return seed;
    }


    /*******************************************************************************
    * Function Name: PRS_WriteSeedUpper
    ********************************************************************************
    *
    * Summary:
    *  Writes upper half of seed value. Only generated for 33-64-bit PRS.
    *
    * Parameters:
    *  seed:  Upper half of seed value.
    *
    * Return:
    *  void
    *
    * Side Effects:
    *  The upper half of seed value is cut according to
    *  mask = 2^(Resolution - 32) - 1.
    *  For example if PRS Resolution is 35 bits the mask value is:
    *  2^(35 - 32) - 1 = 2^(3) - 1 = 0x0000 0007u.
    *  The upper half of seed value = 0x0000 00FFu is cut:
    *  upper half of seed & mask = 0x0000 00FFu & 0x0000 0007u = 0x0000 0007u.
    *
    *******************************************************************************/
    void PRS_WriteSeedUpper(uint32 seed) 
    {
        /* Mask the Seed Upper half to cut unused bits */
        seed &= PRS_MASK;

        /* Write Seed Upper */
        #if (PRS_PRS_SIZE <= 40u)          /* 40 bits PRS */
            PRS_SEED_UPPER_C__A1_REG = LO8(seed);

        #elif (PRS_PRS_SIZE <= 48u)        /* 48 bits PRS */
            PRS_SEED_UPPER_C__A1_REG = HI8(seed);
            PRS_SEED_UPPER_B__A1_REG = LO8(seed);

        #elif (PRS_PRS_SIZE <= 56u)        /* 56 bits PRS */
            PRS_SEED_UPPER_D__A1_REG = LO8(HI16(seed));
            PRS_SEED_UPPER_C__A1_REG = HI8(seed);
            PRS_SEED_UPPER_B__A1_REG = HI8(seed);

        #else                                           /* 64 bits PRS */
            PRS_SEED_UPPER_D__A1_REG = HI8(HI16(seed));
            PRS_SEED_UPPER_C__A1_REG = LO8(HI16(seed));
            PRS_SEED_UPPER_B__A1_REG = HI8(seed);
            PRS_SEED_UPPER_A__A1_REG = LO8(seed);

        #endif  /* End (PRS_PRS_SIZE <= 32u) */

        /* Resets triggers */
        #if (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK)
            if((PRS_sleepState & PRS_NORMAL_SEQUENCE) != 0u)
            {
                PRS_EXECUTE_DFF_RESET;
            }
        #else
            PRS_EXECUTE_DFF_RESET;
        #endif  /* (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK) */
    }


    /*******************************************************************************
    * Function Name: PRS_WriteSeedLower
    ********************************************************************************
    *
    * Summary:
    *  Writes lower half of seed value. Only generated for 33-64-bit PRS.
    *
    * Parameters:
    *  seed:  Lower half of seed value.
    *
    * Return:
    *  void
    *
    *******************************************************************************/
    void PRS_WriteSeedLower(uint32 seed) 
    {
        /* Write Seed Lower */
        #if (PRS_PRS_SIZE <= 40u)          /* 40 bits PRS */
            PRS_SEED_LOWER_B__A1_REG = HI8(HI16(seed));
            PRS_SEED_LOWER_C__A0_REG = LO8(HI16(seed));
            PRS_SEED_LOWER_B__A0_REG = HI8(seed);
            PRS_SEED_LOWER_A__A0_REG = LO8(seed);

        #elif (PRS_PRS_SIZE <= 48u)        /* 48 bits PRS */
            PRS_SEED_LOWER_A__A1_REG = HI8(HI16(seed));
            PRS_SEED_LOWER_C__A0_REG = LO8(HI16(seed));
            PRS_SEED_LOWER_B__A0_REG = HI8(seed);
            PRS_SEED_LOWER_A__A0_REG = LO8(seed);

        #else                                           /* 64 bits PRS */
            PRS_SEED_LOWER_D__A0_REG = HI8(HI16(seed));
            PRS_SEED_LOWER_C__A0_REG = LO8(HI16(seed));
            PRS_SEED_LOWER_B__A0_REG = HI8(seed);
            PRS_SEED_LOWER_A__A0_REG = LO8(seed);

        #endif  /* End (PRS_PRS_SIZE <= 32u) */

        /* Resets triggers */
        #if (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK)
            if((PRS_sleepState & PRS_NORMAL_SEQUENCE) != 0u)
            {
                PRS_EXECUTE_DFF_RESET;
            }
        #else
            PRS_EXECUTE_DFF_RESET;
        #endif  /* (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK) */
    }


    /*******************************************************************************
    * Function Name: PRS_ReadPolynomialUpper
    ********************************************************************************
    *
    * Summary:
    *  Reads upper half of PRS polynomial value. Only generated for 33-64-bit PRS.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Returns upper half of PRS polynomial value.
    *
    *******************************************************************************/
    uint32 PRS_ReadPolynomialUpper(void) 
    {
        uint32 polynomial;

        /* Read Polynomial Upper */
        #if (PRS_PRS_SIZE <= 40u)          /* 40 bits PRS */
            polynomial = PRS_POLYNOM_UPPER_C__D1_REG;

        #elif (PRS_PRS_SIZE <= 48u)        /* 48 bits PRS */
            polynomial = ((uint32) PRS_POLYNOM_UPPER_C__D1_REG) << 8u;
            polynomial |= PRS_POLYNOM_UPPER_B__D1_REG;

        #elif (PRS_PRS_SIZE <= 56u)        /* 56 bits PRS */
            polynomial = ((uint32) PRS_POLYNOM_UPPER_D__D1_REG) << 16u;
            polynomial |= ((uint32) PRS_POLYNOM_UPPER_C__D1_REG) << 8u;
            polynomial |= PRS_POLYNOM_UPPER_B__D1_REG;

        #else                                           /* 64 bits PRS */
            polynomial = ((uint32) PRS_POLYNOM_UPPER_D__D1_REG) << 24u;
            polynomial |= ((uint32) PRS_POLYNOM_UPPER_C__D1_REG) << 16u;
            polynomial |= ((uint32) PRS_POLYNOM_UPPER_B__D1_REG) << 8u;
            polynomial |= PRS_POLYNOM_UPPER_A__D1_REG;

        #endif  /* End (PRS_PRS_SIZE <= 32u) */

        return polynomial;
    }


    /*******************************************************************************
    * Function Name: PRS_ReadPolynomialLower
    ********************************************************************************
    *
    * Summary:
    *  Reads lower half of PRS polynomial value. Only generated for 33-64-bit PRS.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Returns lower half of PRS polynomial value.
    *
    *******************************************************************************/
    uint32 PRS_ReadPolynomialLower(void) 
    {
        uint32 polynomial;

        /* Read Polynomial Lower */
        #if (PRS_PRS_SIZE <= 40u)          /* 40 bits PRS */
            polynomial = ( (uint32) PRS_POLYNOM_LOWER_B__D1_REG) << 24u;
            polynomial |= ( (uint32) PRS_POLYNOM_LOWER_C__D0_REG) << 16u;
            polynomial |= ( (uint32) PRS_POLYNOM_LOWER_B__D0_REG) << 8u;
            polynomial |= PRS_POLYNOM_LOWER_A__D0_REG;

        #elif (PRS_PRS_SIZE <= 48u)        /* 48 bits PRS */
            polynomial = ((uint32) PRS_POLYNOM_LOWER_A__D1_REG) << 24u;
            polynomial |= ((uint32) PRS_POLYNOM_LOWER_C__D0_REG) << 16u;
            polynomial |= ((uint32) PRS_POLYNOM_LOWER_B__D0_REG) << 8u;
            polynomial |= PRS_POLYNOM_LOWER_A__D0_REG;

        #else                                           /* 64 bits PRS */
            polynomial = ((uint32) PRS_POLYNOM_LOWER_D__D0_REG) << 24u;
            polynomial |= ((uint32) PRS_POLYNOM_LOWER_C__D0_REG) << 16u;
            polynomial |= ((uint32) PRS_POLYNOM_LOWER_B__D0_REG) << 8u;
            polynomial |= PRS_POLYNOM_LOWER_A__D0_REG;

        #endif  /* End (PRS_PRS_SIZE <= 32u) */

        return polynomial;
    }


    /*******************************************************************************
    * Function Name: PRS_WritePolynomialUpper
    ********************************************************************************
    *
    * Summary:
    *  Writes upper half of PRS polynomial value. Only generated for 33-64-bit PRS.
    *
    * Parameters:
    *  polynomial:  Upper half PRS polynomial value.
    *
    * Return:
    *  void
    *
    * Side Effects:
    *  The upper half of polynomial value is cut according to
    *  mask = 2^(Resolution - 32) - 1.
    *  For example if PRS Resolution is 35 bits the mask value is:
    *  2^(35 - 32) - 1 = 2^(3) - 1 = 0x0000 0007u.
    *  The upper half of polynomial value = 0x0000 00FFu is cut:
    *  upper half of polynomial & mask = 0x0000 00FFu & 0x0000 0007u = 0x0000 0007u.
    *
    *******************************************************************************/
    void PRS_WritePolynomialUpper(uint32 polynomial)
                                                
    {
        /* Mask the polynomial upper half to cut unused bits */
        polynomial &= PRS_MASK;

        /* Write Polynomial Upper */
        #if (PRS_PRS_SIZE <= 40u)          /* 40 bits PRS */
            PRS_POLYNOM_UPPER_C__D1_REG = LO8(polynomial);

        #elif (PRS_PRS_SIZE <= 48u)        /* 48 bits PRS */
            PRS_POLYNOM_UPPER_C__D1_REG = HI8(polynomial);
            PRS_POLYNOM_UPPER_B__D1_REG = LO8(polynomial);

        #elif (PRS_PRS_SIZE <= 56u)        /* 56 bits PRS */
            PRS_POLYNOM_UPPER_D__D1_REG = LO8(HI16(polynomial));
            PRS_POLYNOM_UPPER_C__D1_REG = HI8(polynomial);
            PRS_POLYNOM_UPPER_B__D1_REG = LO8(polynomial);

        #else                                           /* 64 bits PRS */
            PRS_POLYNOM_UPPER_D__D1_REG = HI8(HI16(polynomial));
            PRS_POLYNOM_UPPER_C__D1_REG = LO8(HI16(polynomial));
            PRS_POLYNOM_UPPER_B__D1_REG = HI8(polynomial);
            PRS_POLYNOM_UPPER_A__D1_REG = LO8(polynomial);

        #endif  /* End (PRS_PRS_SIZE <= 32u) */

        /* Resets triggers */
        #if (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK)
            if((PRS_sleepState & PRS_NORMAL_SEQUENCE) != 0u)
            {
                PRS_EXECUTE_DFF_RESET;
            }
        #else
            PRS_EXECUTE_DFF_RESET;
        #endif  /* (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK) */
    }


    /*******************************************************************************
    * Function Name: PRS_WritePolynomialLower
    ********************************************************************************
    *
    * Summary:
    *  Writes lower half of PRS polynomial value. Only generated for 33-64-bit PRS.
    *
    * Parameters:
    *  polynomial:  Lower half of PRS polynomial value.
    *
    * Return:
    *  void
    *
    *******************************************************************************/
    void PRS_WritePolynomialLower(uint32 polynomial)
                                                
    {
        /* Write Polynomial Lower */
        #if (PRS_PRS_SIZE <= 40u)          /* 40 bits PRS */
            PRS_POLYNOM_LOWER_B__D1_REG = HI8(HI16(polynomial));
            PRS_POLYNOM_LOWER_C__D0_REG = LO8(HI16(polynomial));
            PRS_POLYNOM_LOWER_B__D0_REG = HI8(polynomial);
            PRS_POLYNOM_LOWER_A__D0_REG = LO8(polynomial);

        #elif (PRS_PRS_SIZE <= 48u)        /* 48 bits PRS */
            PRS_POLYNOM_LOWER_A__D1_REG = HI8(HI16(polynomial));
            PRS_POLYNOM_LOWER_C__D0_REG = LO8(HI16(polynomial));
            PRS_POLYNOM_LOWER_B__D0_REG = HI8(polynomial);
            PRS_POLYNOM_LOWER_A__D0_REG = LO8(polynomial);

        #else                                           /* 64 bits PRS */
            PRS_POLYNOM_LOWER_D__D0_REG = HI8(HI16(polynomial));
            PRS_POLYNOM_LOWER_C__D0_REG = LO8(HI16(polynomial));
            PRS_POLYNOM_LOWER_B__D0_REG = HI8(polynomial);
            PRS_POLYNOM_LOWER_A__D0_REG = LO8(polynomial);

        #endif  /* End (PRS_PRS_SIZE <= 32u) */

        /* Resets triggers */
        #if (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK)
            if((PRS_sleepState & PRS_NORMAL_SEQUENCE) != 0u)
            {
                PRS_EXECUTE_DFF_RESET;
            }
        #else
            PRS_EXECUTE_DFF_RESET;
        #endif  /* (PRS_WAKEUP_BEHAVIOUR == PRS__RESUMEWORK) */
    }
#endif  /* End (PRS_PRS_SIZE <= 32u) */


/* [] END OF FILE */
