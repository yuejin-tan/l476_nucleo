/**
 * @file pendSV.h
 * @author yuejinTan
 * @brief use pendSV to creat tasks with lower priority
 * @version 1.2
 */

#ifndef NVICSCHEDULER_PENDSV_H_
#define NVICSCHEDULER_PENDSV_H_

#include "main.h"

#define PENDSV_TASK_SECCESS 0UL

//private vars
extern void (*volatile _PendSVTask)(void);

//do not define PendSV_Handler interrupt Function!
extern void PendSV_Handler(void);

//success return 0; else return the ptr of already exist task
static inline uint32_t pendSVTaskCreat(void (*pendSVTask)(void))
{
    register uint32_t temp;
    __disable_irq();
    __DSB();
    temp = (uint32_t)_PendSVTask;
    if (0 != temp)
    {
        __enable_irq();
        return temp;
    }
    else
    {
        _PendSVTask = pendSVTask;
        __enable_irq();
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
        return PENDSV_TASK_SECCESS;
    }
}

#endif /* NVICSCHEDULER_PENDSV_H_ */
