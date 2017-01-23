/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef KEES_Mod_HEADER
#define KEES_Mod_HEADER

/***************************************
*        Function Prototypes 
***************************************/
#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"  /* needed for the CyScPumpEnabled definition in init function */


void KEES_Mod_Start(void);

void KEES_Mod_Init(void);
void KEES_Mod_Enable(void);

/***************************************
*              Registers        
***************************************/
// CFG
#define KEES_Mod_CR0_REG                  (* (reg8 *) KEES_Mod_ScBlock__CR0 )
#define KEES_Mod_CR0_PTR                  (  (reg8 *) KEES_Mod_ScBlock__CR0 )

#define KEES_Mod_CR1_REG                  (* (reg8 *) KEES_Mod_ScBlock__CR1 )
#define KEES_Mod_CR1_PTR                  (  (reg8 *) KEES_Mod_ScBlock__CR1 )

#define KEES_Mod_CR2_REG                  (* (reg8 *) KEES_Mod_ScBlock__CR2 )
#define KEES_Mod_CR2_PTR                  (  (reg8 *) KEES_Mod_ScBlock__CR2 )

#define KEES_Mod_PUMP_CR1_REG             (* (reg8 *) CYDEV_ANAIF_CFG_PUMP_CR1)
#define KEES_Mod_PUMP_CR1_PTR             (  (reg8 *) CYDEV_ANAIF_CFG_PUMP_CR1)

// RT
#define KEES_Mod_SW0_REG                  (* (reg8 *) KEES_Mod_ScBlock__SW0 )
#define KEES_Mod_SW0_PTR                  (  (reg8 *) KEES_Mod_ScBlock__SW0 )

#define KEES_Mod_SW2_REG                  (* (reg8 *) KEES_Mod_ScBlock__SW2 )
#define KEES_Mod_SW2_PTR                  (  (reg8 *) KEES_Mod_ScBlock__SW2 )

#define KEES_Mod_SW3_REG                  (* (reg8 *) KEES_Mod_ScBlock__SW3 )
#define KEES_Mod_SW3_PTR                  (  (reg8 *) KEES_Mod_ScBlock__SW3 )

#define KEES_Mod_SW4_REG                  (* (reg8 *) KEES_Mod_ScBlock__SW4 )
#define KEES_Mod_SW4_PTR                  (  (reg8 *) KEES_Mod_ScBlock__SW4 )

#define KEES_Mod_SW6_REG                  (* (reg8 *) KEES_Mod_ScBlock__SW6 )
#define KEES_Mod_SW6_PTR                  (  (reg8 *) KEES_Mod_ScBlock__SW6 )

#define KEES_Mod_SW7_REG                  (* (reg8 *) KEES_Mod_ScBlock__SW7 )
#define KEES_Mod_SW7_PTR                  (  (reg8 *) KEES_Mod_ScBlock__SW7 )

#define KEES_Mod_SW8_REG                  (* (reg8 *) KEES_Mod_ScBlock__SW8 )
#define KEES_Mod_SW8_PTR                  (  (reg8 *) KEES_Mod_ScBlock__SW8 )

#define KEES_Mod_SW10_REG                 (* (reg8 *) KEES_Mod_ScBlock__SW10 )
#define KEES_Mod_SW10_PTR                 (  (reg8 *) KEES_Mod_ScBlock__SW10 )

#define KEES_Mod_CLK_REG                  (* (reg8 *) KEES_Mod_ScBlock__CLK )
#define KEES_Mod_CLK_PTR                  (  (reg8 *) KEES_Mod_ScBlock__CLK )

#define KEES_Mod_BST_REG                  (* (reg8 *) KEES_Mod_ScBlock__BST )
#define KEES_Mod_BST_PTR                  (  (reg8 *) KEES_Mod_ScBlock__BST )

#define KEES_Mod_SC_MISC_REG              (* (reg8 *) CYDEV_ANAIF_RT_SC_MISC)
#define KEES_Mod_SC_MISC_PTR              (  (reg8 *) CYDEV_ANAIF_RT_SC_MISC)

// WRK
#define KEES_Mod_SR_REG                   (* (reg8 *) KEES_Mod_ScBlock__SR )
#define KEES_Mod_SR_PTR                   (  (reg8 *) KEES_Mod_ScBlock__SR )

#define KEES_Mod_WRK1_REG                 (* (reg8 *) KEES_Mod_ScBlock__WRK1 )
#define KEES_Mod_WRK1_PTR                 (  (reg8 *) KEES_Mod_ScBlock__WRK1 )

#define KEES_Mod_MSK_REG                  (* (reg8 *) KEES_Mod_ScBlock__MSK )
#define KEES_Mod_MSK_PTR                  (  (reg8 *) KEES_Mod_ScBlock__MSK )

#define KEES_Mod_CMPINV_REG               (* (reg8 *) KEES_Mod_ScBlock__CMPINV )
#define KEES_Mod_CMPINV_PTR               (  (reg8 *) KEES_Mod_ScBlock__CMPINV )

#define KEES_Mod_CPTR_REG                 (* (reg8 *) KEES_Mod_ScBlock__CPTR )
#define KEES_Mod_CPTR_PTR                 (  (reg8 *) KEES_Mod_ScBlock__CPTR )

// power manager
#define KEES_Mod_PM_ACT_CFG_REG           (* (reg8 *) KEES_Mod_ScBlock__PM_ACT_CFG )
#define KEES_Mod_PM_ACT_CFG_PTR           (  (reg8 *) KEES_Mod_ScBlock__PM_ACT_CFG )  

#define KEES_Mod_PM_STBY_CFG_REG          (* (reg8 *) KEES_Mod_ScBlock__PM_STBY_CFG )
#define KEES_Mod_PM_STBY_CFG_PTR          (  (reg8 *) KEES_Mod_ScBlock__PM_STBY_CFG )  


// PM
// *************** MODES *****************
/* PM_ACT_CFG (Active Power Mode CFG Register)mask */ 
#define KEES_Mod_ACT_PWR_EN          KEES_Mod_ScBlock__PM_ACT_MSK 

/* PM_STBY_CFG (Alternate Active Power Mode CFG Register)mask */ 
#define KEES_Mod_STBY_PWR_EN          KEES_Mod_ScBlock__PM_STBY_MSK 


// CFG
// *************** MASKS ****************
// CFG CR0
#define KEES_Mod_DFT_MASK               (0x30u)
#define KEES_Mod_MODE_MASK              (0x0Eu)
// CFG CR1
#define KEES_Mod_GAIN_MASK              (0x20u)
#define KEES_Mod_DIV2_MASK              (0x10u)
#define KEES_Mod_COMP_MASK              (0x0Cu)
#define KEES_Mod_DRIVE_MASK             (0x03u)
// CFG CR2
#define KEES_Mod_PGA_GNDVREF_MASK       (0x80u)
#define KEES_Mod_RVAL_MASK              (0x70u)
#define KEES_Mod_REDC_MASK              (0x0Cu)
#define KEES_Mod_R20_40B_MASK           (0x02u)
#define KEES_Mod_BIAS_CTL_MASK          (0x01u)
// PUMP CR1
#define KEES_Mod_PUMP_MASK              (0x80u)

// *************** SHIFTS ****************
// CFG CR0
#define KEES_Mod_DFT_SHIFT              (4u)
#define KEES_Mod_MODE_SHIFT             (1u)
// CFG CR1
#define KEES_Mod_GAIN_SHIFT             (5u)
#define KEES_Mod_DIV2_SHIFT             (4u)
#define KEES_Mod_COMP_SHIFT             (2u)
#define KEES_Mod_DRIVE_SHIFT            (0u)
// CFG CR2
#define KEES_Mod_PGA_GNDVREF_SHIFT      (7u)
#define KEES_Mod_RVAL_SHIFT             (4u)
#define KEES_Mod_REDC_SHIFT             (2u)
#define KEES_Mod_R20_40B_SHIFT          (1u)
#define KEES_Mod_BIAS_CTL_SHIFT         (0u)
// PUMP CR1
#define KEES_Mod_PUMP_SHIFT             (7u)

// *************** MODES ****************
// CFG CR0
// DFT
#define KEES_Mod_DFT_NORMAL             (0x0u)
#define KEES_Mod_DFT_VBOOST             (0x1u)
// PGA = Voltage Integrator, TIA = Charge integrator, Naked Opamp = Comparator
#define KEES_Mod_DFT_MODE_DEPENDENT     (0x2u)  
#define KEES_Mod_DFT_RESET              (0x3u)
// MODE
#define KEES_Mod_MODE_NAKED_OP_AMP      (0x0u)
#define KEES_Mod_MODE_TIA               (0x1u)
#define KEES_Mod_MODE_CTMIXER           (0x2u)
#define KEES_Mod_MODE_NRZ_SH            (0x3u)
#define KEES_Mod_MODE_UNITY             (0x4u)
#define KEES_Mod_MODE_1ST_MOD           (0x5u)
#define KEES_Mod_MODE_PGA               (0x6u)
#define KEES_Mod_MODE_TRACKANDHOLD      (0x7u)
// CFG CR1
// GAIN
#define KEES_Mod_GAIN_0DB               (0u)
#define KEES_Mod_GAIN_6DB               (1u)
// DIV2
#define KEES_Mod_DIV2_DISABLE           (0u)
#define KEES_Mod_DIV2_ENABLE            (1u)
// COMP
#define KEES_Mod_COMP_3P0PF             (0u)
#define KEES_Mod_COMP_3P6PF             (1u)
#define KEES_Mod_COMP_4P35PF            (2u)
#define KEES_Mod_COMP_5P1PF             (3u)
// DRIVE
#define KEES_Mod_DRIVE_175UA            (0u)
#define KEES_Mod_DRIVE_260UA            (1u)
#define KEES_Mod_DRIVE_330UA            (2u)
#define KEES_Mod_DRIVE_400UA            (3u)
// CFG CR2
// PGA GNDVREF
#define KEES_Mod_PGA_GNDVREF_DISABLE    (0u)
#define KEES_Mod_PGA_GNDVREF_ENABLE     (1u)
// RVAL
#define KEES_Mod_RVAL_20K               (0u)
#define KEES_Mod_RVAL_30K               (1u)
#define KEES_Mod_RVAL_40K               (2u)
#define KEES_Mod_RVAL_80K               (3u)
#define KEES_Mod_RVAL_120K              (4u)
#define KEES_Mod_RVAL_250K              (5u)
#define KEES_Mod_RVAL_500K              (6u)
#define KEES_Mod_RVAL_1M                (7u)
// REDC
#define KEES_Mod_REDC_00                (0u)
#define KEES_Mod_REDC_01                (1u)
#define KEES_Mod_REDC_10                (2u)
#define KEES_Mod_REDC_11                (3u)
// R20 40B
#define KEES_Mod_R20_40B_40K            (0u)
#define KEES_Mod_R20_40B_20K            (1u)
// BIAS CTRL
#define KEES_Mod_BIAS_CTL_1X            (0u)
#define KEES_Mod_BIAS_CTL_2X            (1u)
// PUMP CR1
#define KEES_Mod_PUMP_EXTERNAL          (0u)
#define KEES_Mod_PUMP_INTERNAL          (1u)

// ANAIF
// *************** MASKS ****************
// CLK
#define KEES_Mod_DYN_CNTL_EN_MASK       (0x20u)
#define KEES_Mod_BYPASS_SYNC_MASK       (0x10u)
#define KEES_Mod_CLK_EN_MASK            (0x08u)
#define KEES_Mod_MX_CLK_MASK            (0x07u)
// BST
#define KEES_Mod_BST_CLK_EN_MASK        (0x08u)
#define KEES_Mod_MX_BST_CLK_MASK        (0x07u)
#define KEES_Mod_SC_PUMP_FORCE_MASK		(0x20u)
#define KEES_Mod_SC_PUMP_AUTO_MASK		(0x10u)
#define KEES_Mod_DIFF_PGA_1_3_MASK		(0x02u)
#define KEES_Mod_DIFF_PGA_0_2_MASK		(0x01u)

// *************** SHIFTS ****************
// CLK
#define KEES_Mod_DYN_CNTL_EN_SHIFT      (5u)
#define KEES_Mod_BYPASS_SYNC_SHIFT      (4u)
#define KEES_Mod_CLK_EN_SHIFT           (3u)
#define KEES_Mod_MX_CLK_SHIFT           (0u)
// BST
#define KEES_Mod_BST_CLK_EN_SHIFT       (3u)
#define KEES_Mod_MX_BST_CLK_SHIFT       (0u)
#define KEES_Mod_SC_PUMP_FORCE_SHIFT	(5u)
#define KEES_Mod_SC_PUMP_AUTO_SHIFT		(4u)
#define KEES_Mod_DIFF_PGA_1_3_SHIFT		(1u)
#define KEES_Mod_DIFF_PGA_0_2_SHIFT		(0u)


// *************** MODES ****************
// CLK
// DYN CNTRL EN
#define KEES_Mod_DYN_CNTL_EN_DISABLE    (0u)
#define KEES_Mod_DYN_CNTL_EN_ENABLE     (1u)
// BYPASS SYNC
#define KEES_Mod_BYPASS_SYNC_DISABLE    (0u)
#define KEES_Mod_BYPASS_SYNC_ENABLE     (1u)
// CLK EN
#define KEES_Mod_CLK_EN_DISABLE         (0u)
#define KEES_Mod_CLK_EN_ENABLE          (1u)
// MX CLK
#define KEES_Mod_MX_CLK_0               (0u)
#define KEES_Mod_MX_CLK_1               (1u)
#define KEES_Mod_MX_CLK_2               (2u)
#define KEES_Mod_MX_CLK_3               (3u)
#define KEES_Mod_MX_CLK_UDB             (4u)
// BST
// BST_CLK_EN
#define KEES_Mod_BST_CLK_EN_DISABLE     (0u)
#define KEES_Mod_BST_CLK_EN_ENABLE      (1u)
// MX BST CLK
#define KEES_Mod_MX_BST_CLK_0           (0u)
#define KEES_Mod_MX_BST_CLK_1           (1u)
#define KEES_Mod_MX_BST_CLK_2           (2u)
#define KEES_Mod_MX_BST_CLK_3           (3u)
#define KEES_Mod_MX_BST_CLK_UDB         (4u)
// SC MISC
// FORCE PUMP
#define KEES_Mod_SC_PUMP_NO_FORCE		(0u)
#define KEES_Mod_SC_PUMP_FORCE			(1u)
// PUMP AUTO
#define KEES_Mod_SC_PUMP_NO_AUTO		(0u)
#define KEES_Mod_SC_PUMP_AUTO			(1u)
// DIFF PGA 1 3
#define KEES_Mod_DIFF_PGA_1_3_DISABLE	(0u)
#define KEES_Mod_DIFF_PGA_1_3_ENABLE	(1u)
// DIFF PGA 0 2
#define KEES_Mod_DIFF_PGA_0_2_DISABLE	(0u)
#define KEES_Mod_DIFF_PGA_0_2_ENABLE	(1u)


// **************** ANALOG SWITCHES ****************
// since the switches are different depending on which SC/CT block is used,
// all of the defines are provided, but it is up to the user
// to force the placement of the block to ensure that the switches
// that are needed will be available
// SW0
#define KEES_Mod_VIN_AG7                (0x80u)
#define KEES_Mod_VIN_AG6                (0x40u)
#define KEES_Mod_VIN_AG5                (0x20u)
#define KEES_Mod_VIN_AG4                (0x10u)
#define KEES_Mod_VIN_AG3                (0x08u)
#define KEES_Mod_VIN_AG2                (0x04u)
#define KEES_Mod_VIN_AG1                (0x02u)
#define KEES_Mod_VIN_AG0                (0x01u)
// SW2
#define KEES_Mod_VIN_ABUS3              (0x08u)
#define KEES_Mod_VIN_ABUS2              (0x04u)
//  v-- AVAILABILITY VARIES WITH SC/CT POSITION --v
#define KEES_Mod_VIN_ABUS1              (0x02u)
#define KEES_Mod_VIN_ABUS0              (0x01u)
//  ^-- AVAILABILITY VARIES WITH SC/CT POSITION --^
// SW3
#define KEES_Mod_VREF_BGVREF            (0x20u)
#define KEES_Mod_VIN_BGVREF             (0x02u)
#define KEES_Mod_VIN_AMX                (0x01u)
// SW4
//  v-- AVAILABILITY VARIES WITH SC/CT POSITION --v
#define KEES_Mod_VREF_AG7               (0x80u)
#define KEES_Mod_VREF_AG6               (0x40u)
#define KEES_Mod_VREF_AG5               (0x20u)
#define KEES_Mod_VREF_AG4               (0x10u)
#define KEES_Mod_VREF_AG3               (0x08u)
#define KEES_Mod_VREF_AG2               (0x04u)
#define KEES_Mod_VREF_AG1               (0x02u)
#define KEES_Mod_VREF_AG0               (0x01u)
//  ^-- AVAILABILITY VARIES WITH SC/CT POSITION --^
// SW6
//  v-- AVAILABILITY VARIES WITH SC/CT POSITION --v
#define KEES_Mod_VREF_ABUS1             (0x02u)
#define KEES_Mod_VREF_ABUS0             (0x01u)
//  ^-- AVAILABILITY VARIES WITH SC/CT POSITION --^
// SW7
#define KEES_Mod_VIN_VO                 (0x04u)
// SW8
//  v-- AVAILABILITY VARIES WITH SC/CT POSITION --v
#define KEES_Mod_VO_AG7                 (0x80u)
#define KEES_Mod_VO_AG6                 (0x40u)
#define KEES_Mod_VO_AG5                 (0x20u)
#define KEES_Mod_VO_AG4                 (0x10u)
#define KEES_Mod_VO_AG3                 (0x08u)
#define KEES_Mod_VO_AG2                 (0x04u)
#define KEES_Mod_VO_AG1                 (0x02u)
#define KEES_Mod_VO_AG0                 (0x01u)
//  ^-- AVAILABILITY VARIES WITH SC/CT POSITION --^
// SW10
//  v-- AVAILABILITY VARIES WITH SC/CT POSITION --v
#define KEES_Mod_VO_ABUS3               (0x08u)
#define KEES_Mod_VO_ABUS2               (0x04u)
#define KEES_Mod_VO_ABUS1               (0x02u)
#define KEES_Mod_VO_ABUS0               (0x01u)
//  ^-- AVAILABILITY VARIES WITH SC/CT POSITION --^

// WRK
// *************** MASKS ****************
// SR
#define KEES_Mod_SC3_MODOUT_MASK        (0x08u)
#define KEES_Mod_SC2_MODOUT_MASK        (0x04u)
#define KEES_Mod_SC1_MODOUT_MASK        (0x02u)
#define KEES_Mod_SC0_MODOUT_MASK        (0x01u)
// MSK
#define KEES_Mod_SC3_MSK_MASK           (0x08u)
#define KEES_Mod_SC2_MSK_MASK           (0x04u)
#define KEES_Mod_SC1_MSK_MASK           (0x02u)
#define KEES_Mod_SC0_MSK_MASK           (0x01u)
// CMP INV
#define KEES_Mod_SC3_CMPINV_MASK        (0x08u)
#define KEES_Mod_SC2_CMPINV_MASK        (0x04u)
#define KEES_Mod_SC1_CMPINV_MASK        (0x02u)
#define KEES_Mod_SC0_CMPINV_MASK        (0x01u)
// CPTR
#define KEES_Mod_SC3_CPTR_MASK          (0x08u)
#define KEES_Mod_SC2_CPTR_MASK          (0x04u)
#define KEES_Mod_SC1_CPTR_MASK          (0x02u)
#define KEES_Mod_SC0_CPTR_MASK          (0x01u)

// *************** SHIFTS ****************
// SR
#define KEES_Mod_SC3_MODOUT_SHIFT       (3u)
#define KEES_Mod_SC2_MODOUT_SHIFT       (2u)
#define KEES_Mod_SC1_MODOUT_SHIFT       (1u)
#define KEES_Mod_SC0_MODOUT_SHIFT       (0u)
// MSK
#define KEES_Mod_SC3_MSK_SHIFT          (3u)
#define KEES_Mod_SC2_MSK_SHIFT          (2u)
#define KEES_Mod_SC1_MSK_SHIFT          (1u)
#define KEES_Mod_SC0_MSK_SHIFT          {0u)
// CMP INV
#define KEES_Mod_SC3_CMPINV_SHIFT       (3u)
#define KEES_Mod_SC2_CMPINV_SHIFT       (2u)
#define KEES_Mod_SC1_CMPINV_SHIFT       (1u)
#define KEES_Mod_SC0_CMPINV_SHIFT       (0u)
// CPTR
#define KEES_Mod_SC3_CPTR_SHIFT         (3u)
#define KEES_Mod_SC2_CPTR_SHIFT         (2u)
#define KEES_Mod_SC1_CPTR_SHIFT         (1u)
#define KEES_Mod_SC0_CPTR_SHIFT         (0u)


// *************** MODES ****************
// MSK
#define KEES_Mod_SC3_MSK_DISABLE        (0u)
#define KEES_Mod_SC3_MSK_ENABLE         (1u)
// CMP INV
#define KEES_Mod_SC3_CMPINV_NORMAL      (0u)
#define KEES_Mod_SC3_CMPINV_INVERT      (1u)
// CPTR
#define KEES_Mod_SC3_CPTR_EDGE          (0u)
#define KEES_Mod_SC3_CPTR_LEVEL         (1u)

/***************************************
*            Register Constants        
***************************************/

#endif
//[] END OF FILE