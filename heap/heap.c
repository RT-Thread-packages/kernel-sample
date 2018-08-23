/*
 * 程序清单：堆内存例程
 *
 * 这个程序会演示堆内存的申请和释放操作。
 */
#include <rtthread.h>
#include <string.h>

static rt_bool_t mem_check(rt_uint8_t *ptr, rt_uint8_t value, rt_uint32_t len)
{
    while (len)
    {
        if (*ptr != value)
            return RT_FALSE;
        ptr ++;
        len --;
    }

    return RT_TRUE;
}

int heap_malloc_init(void)
{
    rt_uint8_t *ptr1, *ptr2, *ptr3, *ptr4, *ptr5;

    ptr1 = rt_malloc(1);
    ptr2 = rt_malloc(13);
    ptr3 = rt_malloc(31);
    ptr4 = rt_malloc(127);
    ptr5 = rt_malloc(0);

    memset(ptr1, 1, 1);
    memset(ptr2, 2, 13);
    memset(ptr3, 3, 31);
    memset(ptr4, 4, 127);

    if (mem_check(ptr1, 1, 1)   == RT_FALSE)
        rt_kprintf("mem_check 1 failed\n");
    if (mem_check(ptr2, 2, 13)  == RT_FALSE)
        rt_kprintf("mem_check 2 failed\n");
    if (mem_check(ptr3, 3, 31)  == RT_FALSE)
        rt_kprintf("mem_check 3 failed\n");
    if (mem_check(ptr4, 4, 127) == RT_FALSE)
        rt_kprintf("mem_check 4 failed\n");

    rt_free(ptr4);
    rt_free(ptr3);
    rt_free(ptr2);
    rt_free(ptr1);

    if (ptr5 != RT_NULL)
    {
        rt_free(ptr5);
    }

    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(heap_malloc_init, heap malloc sample);
