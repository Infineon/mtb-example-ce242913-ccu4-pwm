/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSOC™ Control C1 MCU: CCU4 PWM Example
*              for ModusToolbox.
*
* Related Document: See README.md
*
******************************************************************************
* (c) 2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*****************************************************************************/
#include "cybsp.h"
#include "cy_utils.h"

/*******************************************************************************
* Macros
******************************************************************************/
/* Defines priority level of the period match event interrupt */
#define PWM_0_PERIOD_MATCH_EVENT_PRIORITY 63

/* Defines handler of the period match event interrupt */
#define PWM_0_PERIOD_MATCH_EVENT_HANDLER CCU41_3_IRQHandler

/* Defines IRQ number of the period match event interrupt */
#define PWM_0_PERIOD_MATCH_EVENT_IRQN CCU41_3_IRQn

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function. It sets up and starts a CCU4 timer that triggers a 
* periodic interrupt based on the compare match. 
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/

int main(void)
{
    cy_rslt_t result;

    /*Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Set NVIC priority */
    NVIC_SetPriority(PWM_0_PERIOD_MATCH_EVENT_IRQN, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), PWM_0_PERIOD_MATCH_EVENT_PRIORITY, 0));

    /* Enable IRQ */
    NVIC_EnableIRQ(PWM_0_PERIOD_MATCH_EVENT_IRQN);

    /* Start the timer */
    Cy_CCU4_SLICE_StartTimer(PWM_0_HW);

    while (1);
}

/*******************************************************************************
* Function Name: PWM_0_PERIOD_MATCH_EVENT_HANDLER
********************************************************************************
* Summary:
* This is the interrupt handler function for the CCU4 compare match interrupt.
* It clears the event flag and restarts the timer. 
*
* Parameters:
*  none
*
* Return:
*  void
*
*******************************************************************************/
void PWM_0_PERIOD_MATCH_EVENT_HANDLER(void)
{
    /* Clear pending interrupt */
    Cy_CCU4_SLICE_ClearEvent(PWM_0_HW, CY_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);

    /* Restart the Timer using SCU.GSC41 signal*/
    Cy_SCU_SetCcuTriggerLow(CY_SCU_CCU_TRIGGER_CCU41);
    Cy_SCU_SetCcuTriggerHigh(CY_SCU_CCU_TRIGGER_CCU41);
}

/* [] END OF FILE */
