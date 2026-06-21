/*
 * FreeRTOS V202111.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_EXAMPLES_COMMON_H
#define FREERTOS_CONFIG_EXAMPLES_COMMON_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* Scheduler Related */
#define configUSE_PREEMPTION 1
#define configUSE_TICKLESS_IDLE 0
#define configUSE_IDLE_HOOK 0
#define configUSE_TICK_HOOK 0
#define configTICK_RATE_HZ ((TickType_t)1000)
#define configMAX_PRIORITIES 56
#define configMINIMAL_STACK_SIZE (configSTACK_DEPTH_TYPE)512
#define configUSE_16_BIT_TICKS 0

#define configIDLE_SHOULD_YIELD 1

/* Synchronization Related */
#define configUSE_MUTEXES 1
#define configUSE_RECURSIVE_MUTEXES 1
#define configUSE_APPLICATION_TASK_TAG 0
#define configUSE_COUNTING_SEMAPHORES 1
#define configQUEUE_REGISTRY_SIZE 8
#define configUSE_QUEUE_SETS 1
#define configUSE_TIME_SLICING 1
#define configUSE_NEWLIB_REENTRANT 0
// todo need this for lwip FreeRTOS sys_arch to compile
#define configENABLE_BACKWARD_COMPATIBILITY 1
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 5

/* System */
#define configSTACK_DEPTH_TYPE uint32_t
#define configMESSAGE_BUFFER_LENGTH_TYPE size_t

/* Memory allocation related definitions. */
#ifndef configSUPPORT_STATIC_ALLOCATION
#define configSUPPORT_STATIC_ALLOCATION 0
#endif
#ifndef configSUPPORT_DYNAMIC_ALLOCATION
#define configSUPPORT_DYNAMIC_ALLOCATION 1
#endif
#define configTOTAL_HEAP_SIZE (128 * 1024)
#define configAPPLICATION_ALLOCATED_HEAP 0

/* Hook function related definitions. */
#define configCHECK_FOR_STACK_OVERFLOW 0
#define configUSE_MALLOC_FAILED_HOOK 0
#define configUSE_DAEMON_TASK_STARTUP_HOOK 0

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS 0
#define configUSE_TRACE_FACILITY 1
#define configUSE_STATS_FORMATTING_FUNCTIONS 0

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES 0
#define configMAX_CO_ROUTINE_PRIORITIES 1

/* Software timer related definitions. */
#define configUSE_TIMERS 1
#define configTIMER_TASK_PRIORITY (configMAX_PRIORITIES - 1)
#define configTIMER_QUEUE_LENGTH 10
#define configTIMER_TASK_STACK_DEPTH 1024

/* Interrupt nesting behaviour configuration. */
/*
#define configKERNEL_INTERRUPT_PRIORITY         [dependent of processor]
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    [dependent on processor and application]
#define configMAX_API_CALL_INTERRUPT_PRIORITY   [dependent on processor and application]
*/
/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */

#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY 15

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

#define configPRIO_BITS 4

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

#if FREE_RTOS_KERNEL_SMP  // set by the RP2xxx SMP port of FreeRTOS
/* SMP port only */
#ifndef configNUMBER_OF_CORES
#define configNUMBER_OF_CORES 2
#endif
#define configNUM_CORES configNUMBER_OF_CORES
#define configTICK_CORE 0
#define configRUN_MULTIPLE_PRIORITIES 1
#if configNUMBER_OF_CORES > 1
#define configUSE_CORE_AFFINITY 1
#endif
#define configUSE_PASSIVE_IDLE_HOOK 0
#endif

/* RP2040 specific */
#define configSUPPORT_PICO_SYNC_INTEROP 1
#define configSUPPORT_PICO_TIME_INTEROP 1

#include <assert.h>
/* Define to trap errors during development. */
#define configASSERT(x) assert(x)

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet 1
#define INCLUDE_uxTaskPriorityGet 1
#define INCLUDE_vTaskDelete 1
#define INCLUDE_vTaskSuspend 1
#define INCLUDE_vTaskDelayUntil 1
#define INCLUDE_vTaskDelay 1
#define INCLUDE_xTaskGetSchedulerState 1
#define INCLUDE_xTaskGetCurrentTaskHandle 1
#define INCLUDE_uxTaskGetStackHighWaterMark 1
#define INCLUDE_xTaskGetIdleTaskHandle 1
#define INCLUDE_eTaskGetState 1
#define INCLUDE_xTimerPendFunctionCall 1
#define INCLUDE_xTaskAbortDelay 1
#define INCLUDE_xTaskGetHandle 1
#define INCLUDE_xTaskResumeFromISR 1
#define INCLUDE_xQueueGetMutexHolder 1

#if PICO_RP2350
#define configENABLE_MPU 0
#define configENABLE_TRUSTZONE 0
#define configRUN_FREERTOS_SECURE_ONLY 1
#define configENABLE_FPU 1
// #define configMAX_SYSCALL_INTERRUPT_PRIORITY 16
#endif

/* A header file that defines trace macro can be included here. */

#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0

/// Priority values.
typedef enum {
    osPriorityNone = 0,               ///< No priority (not initialized).
    osPriorityIdle = 1,               ///< Reserved for Idle thread.
    osPriorityLow = 8,                ///< Priority: low
    osPriorityLow1 = 8 + 1,           ///< Priority: low + 1
    osPriorityLow2 = 8 + 2,           ///< Priority: low + 2
    osPriorityLow3 = 8 + 3,           ///< Priority: low + 3
    osPriorityLow4 = 8 + 4,           ///< Priority: low + 4
    osPriorityLow5 = 8 + 5,           ///< Priority: low + 5
    osPriorityLow6 = 8 + 6,           ///< Priority: low + 6
    osPriorityLow7 = 8 + 7,           ///< Priority: low + 7
    osPriorityBelowNormal = 16,       ///< Priority: below normal
    osPriorityBelowNormal1 = 16 + 1,  ///< Priority: below normal + 1
    osPriorityBelowNormal2 = 16 + 2,  ///< Priority: below normal + 2
    osPriorityBelowNormal3 = 16 + 3,  ///< Priority: below normal + 3
    osPriorityBelowNormal4 = 16 + 4,  ///< Priority: below normal + 4
    osPriorityBelowNormal5 = 16 + 5,  ///< Priority: below normal + 5
    osPriorityBelowNormal6 = 16 + 6,  ///< Priority: below normal + 6
    osPriorityBelowNormal7 = 16 + 7,  ///< Priority: below normal + 7
    osPriorityNormal = 24,            ///< Priority: normal
    osPriorityNormal1 = 24 + 1,       ///< Priority: normal + 1
    osPriorityNormal2 = 24 + 2,       ///< Priority: normal + 2
    osPriorityNormal3 = 24 + 3,       ///< Priority: normal + 3
    osPriorityNormal4 = 24 + 4,       ///< Priority: normal + 4
    osPriorityNormal5 = 24 + 5,       ///< Priority: normal + 5
    osPriorityNormal6 = 24 + 6,       ///< Priority: normal + 6
    osPriorityNormal7 = 24 + 7,       ///< Priority: normal + 7
    osPriorityAboveNormal = 32,       ///< Priority: above normal
    osPriorityAboveNormal1 = 32 + 1,  ///< Priority: above normal + 1
    osPriorityAboveNormal2 = 32 + 2,  ///< Priority: above normal + 2
    osPriorityAboveNormal3 = 32 + 3,  ///< Priority: above normal + 3
    osPriorityAboveNormal4 = 32 + 4,  ///< Priority: above normal + 4
    osPriorityAboveNormal5 = 32 + 5,  ///< Priority: above normal + 5
    osPriorityAboveNormal6 = 32 + 6,  ///< Priority: above normal + 6
    osPriorityAboveNormal7 = 32 + 7,  ///< Priority: above normal + 7
    osPriorityHigh = 40,              ///< Priority: high
    osPriorityHigh1 = 40 + 1,         ///< Priority: high + 1
    osPriorityHigh2 = 40 + 2,         ///< Priority: high + 2
    osPriorityHigh3 = 40 + 3,         ///< Priority: high + 3
    osPriorityHigh4 = 40 + 4,         ///< Priority: high + 4
    osPriorityHigh5 = 40 + 5,         ///< Priority: high + 5
    osPriorityHigh6 = 40 + 6,         ///< Priority: high + 6
    osPriorityHigh7 = 40 + 7,         ///< Priority: high + 7
    osPriorityRealtime = 48,          ///< Priority: realtime
    osPriorityRealtime1 = 48 + 1,     ///< Priority: realtime + 1
    osPriorityRealtime2 = 48 + 2,     ///< Priority: realtime + 2
    osPriorityRealtime3 = 48 + 3,     ///< Priority: realtime + 3
    osPriorityRealtime4 = 48 + 4,     ///< Priority: realtime + 4
    osPriorityRealtime5 = 48 + 5,     ///< Priority: realtime + 5
    osPriorityRealtime6 = 48 + 6,     ///< Priority: realtime + 6
    osPriorityRealtime7 = 48 + 7,     ///< Priority: realtime + 7
    osPriorityISR = 56,               ///< Reserved for ISR deferred thread.
    osPriorityError = -1,             ///< System cannot determine priority or illegal priority.
    osPriorityReserved = 0x7FFFFFFF,  ///< Prevents enum down-size compiler optimization.
} osPriority_t;

#endif /* FREERTOS_CONFIG_H */
