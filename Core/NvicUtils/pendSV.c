/**
 * @file pendSV.c
 * @author yuejinTan
 * @brief use pendSV to creat tasks with lower priority
 * @version 1.2
 */

#include "pendSV.h"

void (*volatile _PendSVTask)(void);

__attribute__((naked)) void PendSV_Handler(void)
{
    __asm volatile(
        ".syntax unified\n"
        "ldr r1, task_addr\n" //r1 = task pointer addr
        "movs r0, #0\n"       //r0 = 0
        "\n"                  //
        "cpsid i\n"           //disable interrupt
        "ldr r2, [r1, #0]\n"  //task addr
        "\n"                  //
        "cmp r2, #0\n"        //
        "beq not_enable\n"    //
        "\n"                  //
        "is_enable:\n"        //r0 = 0
        "str r0, [r1, #0]\n"  //task addr=null
        "cpsie i\n"           //enable interrupt
        "push {lr}\n"         //preserve lr
        "blx r2\n"            //call task
        "pop {pc}\n"          //return
        "\n"                  //
        "not_enable:\n"       //
        "cpsie i\n"           //enable interrupt
        "bx lr\n"             //return
        "\n"                  //
        ".align 4, 0xff\n"    //
        "task_addr:\n"        //
        ".word _PendSVTask\n" //
    );
}
