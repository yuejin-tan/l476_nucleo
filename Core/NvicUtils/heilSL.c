/**
 * @file heilSL.c
 * @author yuejinTan
 * @brief save and load function, simple coroutine implementation
 * @version 3.0
 */

#include "heilSL.h"
#include "stdlib.h"

#if 1

// "return" wrongly used
void _HeilError(uint32_t heilRet)
{
    heilRet = heilRet;
    while (1)
    {
        //empty
    }
}

uint32_t HeilInitCall(heilStructHandler heilStructHandlerx, void *callFcnPtr, uint32_t sendVal)
{
    uint32_t heilRet;
    __asm volatile(
        ".syntax unified\n"
        "\n"                                       //
        "ldr %[heilRet], =_HeilReturn_initCall\n"  //
        "adds %[heilRet], %[heilRet], #1\n"        //
        "str %[heilRet], [%[handle] , #0]\n"       //
        "mov %[heilRet], sp\n"                     //
        "str %[heilRet], [%[handle] , #4]\n"       //
        "\n"                                       //
        "adds %[handle], %[handle], %[heilSize]\n" //
        "mov sp, %[handle]\n"                      //
        "\n"                                       //
        "ldr %[heilRet], _HeilError_ADDR\n"        //
        "mov lr, %[heilRet]\n"                     //
        "\n"                                       //
        "push {%[callFcnPtr]}\n"                   //
        "mov r0, %[sendVal]\n"                     //
        "pop {pc}\n"                               //
        "\n"                                       //
        ".align 4, 0xff\n"                         //
        "_HeilError_ADDR:\n"                       //
        ".word _HeilError\n"                       //
        "\n"                                       //
        "\n"                                       //
        "_HeilReturn_initCall:\n"                  //
        "mov %[heilRet], r0\n"                     //
        : [heilRet] "=&l"(heilRet)
        : [handle] "l"(heilStructHandlerx), [callFcnPtr] "l"(callFcnPtr), [sendVal] "l"(sendVal), [heilSize] "l"(HEIL_STACK_LENTH + 16)
        : "cc", "memory", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r14"
#if MPU_REG_PROTECT
          ,
          "d8", "d9", "d10", "d11", "d12", "d13", "d14", "d15"
#endif
    );
    return heilRet;
}

uint32_t HeilLoad(heilStructHandler heilStructHandlerx, uint32_t sendVal)
{
    uint32_t heilRet;
    __asm volatile(
        ".syntax unified\n"
        "\n"                                  //
        "ldr %[heilRet], =_HeilReturn_load\n" //
        "adds %[heilRet], %[heilRet], #1\n"   //
        "str %[heilRet], [%[handle] , #0]\n"  //
        "mov %[heilRet], sp\n"                //
        "str %[heilRet], [%[handle] , #4]\n"  //
        "\n"                                  //
        "ldr %[heilRet], [%[handle] , #8]\n"  //
        "mov lr, %[heilRet]\n"                //
        "ldr %[heilRet], [%[handle] , #12]\n" //
        "mov sp, %[heilRet]\n"                //
        "\n"                                  //
        "mov r0, %[sendVal]\n"                //
        "bx lr\n"                             //
        "\n"                                  //
        "\n"                                  //
        "_HeilReturn_load:\n"                 //
        "mov %[heilRet], r0\n"                //
        "\n"                                  //
        : [heilRet] "=&l"(heilRet)
        : [handle] "l"(heilStructHandlerx), [sendVal] "l"(sendVal)
        : "cc", "memory", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r14"
#if MPU_REG_PROTECT
          ,
          "d8", "d9", "d10", "d11", "d12", "d13", "d14", "d15"
#endif
    );
    return heilRet;
}

uint32_t HeilYield(heilStructHandler heilStructHandlerx, uint32_t yieldVal)
{
    uint32_t heilRet;
    __asm volatile(
        ".syntax unified\n"
        "\n"                                    //
        "ldr %[heilRet], =_HeilReturn_yieldl\n" //
        "adds %[heilRet], %[heilRet], #1\n"     //
        "str %[heilRet], [%[handle] , #8]\n"    //
        "mov %[heilRet], sp\n"                  //
        "str %[heilRet], [%[handle] , #12]\n"   //
        "\n"                                    //
        "ldr %[heilRet], [%[handle] , #0]\n"    //
        "mov lr, %[heilRet]\n"                  //
        "ldr %[heilRet], [%[handle] , #4]\n"    //
        "mov sp, %[heilRet]\n"                  //
        "\n"                                    //
        "mov r0, %[yieldVal]\n"                 //
        "bx lr\n"                               //
        "\n"                                    //
        "\n"                                    //
        "_HeilReturn_yieldl:\n"                 //
        "mov %[heilRet], r0\n"                  //
        "\n"                                    //
        : [heilRet] "=&l"(heilRet)
        : [handle] "l"(heilStructHandlerx), [yieldVal] "l"(yieldVal)
        : "cc", "memory", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r14"
#if MPU_REG_PROTECT
          ,
          "d8", "d9", "d10", "d11", "d12", "d13", "d14", "d15"
#endif
    );
    return heilRet;
}

#endif
