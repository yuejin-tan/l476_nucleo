/**
 * @file mutex.h
 * @author yuejinTan
 * @brief mutex lock
 * @version 1.0
 */

#ifndef NVICSCHEDULER_MUTEX_H_
#define NVICSCHEDULER_MUTEX_H_

#include "main.h"

#define MUTEX_CLEAR_OK 0UL
#define MUTEX_CLEAR_ALREADY 1UL

#define MUTEX_A_MASK 0x0000ffffUL
#define MUTEX_B_MASK 0xffff0000UL
#define MUTEX_A_LOCK_MASK 0x000000ffUL
#define MUTEX_A_HANG_MASK 0x0000ff00UL
#define MUTEX_B_LOCK_MASK 0x00ff0000UL
#define MUTEX_B_HANG_MASK 0xff000000UL
#define MUTEX_BOTH_LOCK_MASK 0x00ff00ffUL
#define MUTEX_BOTH_HANG_MASK 0xff00ff00UL
#define MUTEX_ALL_MASK 0xffffffffUL

//should be allocated in .bss segment
//_whole   high   xx     xx     low
//_ui8[4]  3      2      1      0
//meaning  B_Hang B_Lock A_Hang A_Lock
typedef union MutexUnion
{
    volatile uint32_t _whole;
    volatile uint8_t _ui8[4];
} mutexType;

typedef mutexType *(mutexHandle);

//if judge, failed, setmask
//else, succeed, set lockmask
//return the unchanged value
// e.g. for A:
// uint32_t ret = MutexSet(myHandler, MUTEX_A_LOCK_MASK, MUTEX_B_LOCK_MASK, MUTEX_A_HANG_MASK);
// if (ret & MUTEX_B_LOCK_MASK)
// {
//     //failed... see which lock by (ret | MUTEX_BOTH_LOCK_MASK)
// }
// else
// {
//     //seccees... see hang sta by (ret | MUTEX_BOTH_HANG_MASK)
// }
static inline uint32_t MutexSet(mutexHandle mutexHandle1, uint32_t mutexLockMask, uint32_t mutexJudgeMask, uint32_t mutexSetMask)
{
    __disable_irq();
    __DSB();
    register uint32_t temp;
    temp = mutexHandle1->_whole;
    if (temp & mutexJudgeMask)
    {
        //fail
        mutexHandle1->_whole = temp | mutexSetMask;
        __enable_irq();
        return temp;
    }
    //succeed
    mutexHandle1->_whole = temp | mutexLockMask;
    __enable_irq();
    return temp;
}

//clear lockmask
//if judge, succeed, clearmask
//else, failed
//return the value just after step 1
// e.g. for A:
// uint32_t ret = MutexClear(myHandle, MUTEX_A_LOCK_MASK, MUTEX_B_HANG_MASK, MUTEX_B_HANG_MASK);
// if (ret & MUTEX_BOTH_HANG_MASK)
// {
//     //B is hanged, let B get the lock
// }
// else
// {
//     //the resourse is free
// }
static inline uint32_t MutexClear(mutexHandle mutexHandle1, uint32_t mutexLockMask, uint32_t mutexJudgeMask, uint32_t mutexClearMask)
{
    __disable_irq();
    __DSB();
    register uint32_t temp;
    temp = mutexHandle1->_whole;
    temp = temp & (~mutexLockMask);
    if (temp & mutexJudgeMask)
    {
        mutexHandle1->_whole = temp & (~mutexClearMask);
        __enable_irq();
        return temp;
    }
    mutexHandle1->_whole = temp;
    __enable_irq();
    return temp;
}

#endif /* NVICSCHEDULER_MUTEX_H_ */
