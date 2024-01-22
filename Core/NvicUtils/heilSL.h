/**
 * @file heilSL.h
 * @author yuejinTan
 * @brief simple coroutine implementation
 * @version 3.0
 */

#include "main.h"

#ifndef HEILSL_H_
#define HEILSL_H_

#define MPU_REG_PROTECT 1

#define HEIL_STACK_LENTH 1000

typedef __attribute__((aligned(4))) struct
{
    volatile uint32_t _pc_main;
    volatile uint32_t _sp_main;
    volatile uint32_t _pc_fcn;
    volatile uint32_t _sp_fcn;
    uint32_t _stack[HEIL_STACK_LENTH / 4];
} heilStruct;

typedef heilStruct *heilStructHandler;

//ret yieldVal
extern uint32_t HeilLoad(heilStructHandler heilStructHandler1, uint32_t sendVal);

//ret yieldVal
extern uint32_t HeilInitCall(heilStructHandler heilStructHandler1, void *callFcnPtr, uint32_t sendVal);

//ret sendVal
extern uint32_t HeilYield(heilStructHandler heilStructHandler1, uint32_t yieldVal);

#endif
