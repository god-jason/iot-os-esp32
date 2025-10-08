#include "luat_base.h"
#include "luat_rtos.h"

#ifdef LUAT_FREERTOS_FULL_INCLUDE
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#else
#include "FreeRTOS.h"
#include "queue.h"
#endif

/**
 * luat_task_cpu_usage
 * Description: 获得CPU使用率
 * @param total CPU 总时间
 * @param idle IDLE 时间
 * @return 0 失败 1 成功
 */
int luat_task_cpu_usage(uint32_t *total, uint32_t *idle)
{
    int ret = 0;
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;

    uxArraySize = uxTaskGetNumberOfTasks(); // uxCurrentNumberOfTasks;
    pxTaskStatusArray = pvPortMallocEc(uxArraySize * sizeof(TaskStatus_t));

    if (pxTaskStatusArray != NULL)
    {
        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, totalTime);
        for (x = 0; x < uxArraySize; x++)
        {
            if (0 == strcmp(pxTaskStatusArray[x].pcTaskName, "IDLE"))
            {
                *idle = pxTaskStatusArray[x].ulRunTimeCounter;
                ret = 1;
                break;
            }
        }

        vPortFreeEc(pxTaskStatusArray);
    }

    return ret;
}