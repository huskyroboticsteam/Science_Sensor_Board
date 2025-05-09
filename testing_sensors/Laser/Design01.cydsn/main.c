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
#include <project.h>

#include "project.h"

int main(void)
{
    CyGlobalIntEnable; // Enable global interrupts

    /*
    for(;;)
    {
        ComponentPin_Write(1); // Power ON the component
        CyDelay(1000);         // Wait 1 second

        ComponentPin_Write(0); // Power OFF the component
        CyDelay(1000);         // Wait 1 second
    }
    */
    ComponentPin_Write(1); // Power ON the component 
}
