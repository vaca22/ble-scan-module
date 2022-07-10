
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_heap_caps.h"
#include "plat_os.h"

void port_switch_isr(int higher_prio_task_woken)
{
    if(higher_prio_task_woken == pdTRUE)
    {
        portYIELD_FROM_ISR();
    }
}

extern unsigned port_interruptNesting[];

int is_isr_context(void)
{
    return (port_interruptNesting[xPortGetCoreID()] != 0);
    //return xPortInIsrContext();    
}

spinlock_t os_lock;
static int32_t os_lock_init_flag = 0;
void os_thread_enter_critical(void)
{
    if(os_lock_init_flag == 0)
    {
        spinlock_initialize(&os_lock);
        os_lock_init_flag = 1;
    }
    taskENTER_CRITICAL(&os_lock);
}

void os_thread_exit_critical(void)
{
    taskEXIT_CRITICAL(&os_lock);
}

#define OS_CREATE_THREAD_LOG    0

int os_thread_create(os_thread_t* thandle, const char* name, 
                    os_thread_entry(*entry)(os_thread_arg_t arg), void *arg, 
                    int32_t stack_size, int prio)
{
    int rev = 0;
#if OS_CREATE_THREAD_LOG == 1
    char buf[32];
#endif

extern void prvCheckTasksWaitingTermination( void );
    //prvCheckTasksWaitingTermination(); //检查有没有任务被删除，在这里把ram释放掉
    
    rev = xTaskCreate(entry, name, stack_size, arg, prio, thandle);
    //rev = xTaskCreatePinnedToCore(entry, name, stack_size, arg, prio, thandle, 0);
	//rev = xTaskCreatePinnedToCore_psram(entry, name, stack_size, arg, prio, thandle, 0);
    if(rev == pdPASS)
    {
#if OS_CREATE_THREAD_LOG == 1
        sprintf(buf, "%s, r:%d s:%dk", name, rev, stack_size / 1024);
        os_heap_free_show(buf);
#endif
        return OS_EOK;
    }
#if OS_CREATE_THREAD_LOG == 1
    sprintf(buf, "%s, r:%d s:%dk", name, rev, stack_size / 1024);
    os_heap_free_show(buf);
#endif
    return rev;
}

int os_thread_delete(os_thread_t* thandle)
{
    if(thandle == NULL)
    {
void plat_thread_stack_used(void);
        //plat_thread_stack_used();
        vTaskDelete(NULL);
    }
    else
    {
		os_thread_t thread = *thandle;
		*thandle = NULL;
        vTaskDelete(thread);
    }
    return OS_EOK;
}

/*
 * 现在sleep的单位为ms
 */
int os_thread_sleep(int ms)
{
    vTaskDelay(ms);
    return 0;
}

char* os_thread_get_name(os_thread_t handle)
{
    return pcTaskGetTaskName(handle);
}

int os_semaphore_create_binary(os_semaphore_t *mhandle, const char* name)
{
    SemaphoreHandle_t sem = NULL;
    vSemaphoreCreateBinary(sem);
    if(sem == NULL)
    {
        printf("[os]sem_create failed!\n");
        return OS_EFAIL;
    }
    *mhandle = sem;
    return OS_EOK;
}

int os_semaphore_create_counting(os_semaphore_t *mhandle, const char* name, unsigned long maxcount, unsigned long initcount)
{
    *mhandle = xSemaphoreCreateCounting(maxcount, initcount);
    if(*mhandle)
    {
        return OS_EOK;
    }
    else
    {
        return OS_EFAIL;
    }
}

#define SEMAPHORE_MAX   8
int os_semaphore_create(os_semaphore_t *mhandle, const char* name, int initval)
{
    return os_semaphore_create_counting(mhandle, name, SEMAPHORE_MAX, initval);
}

int os_semaphore_get(os_semaphore_t *mhandle, unsigned long wait)
{
    int rev;
    signed portBASE_TYPE higher_prio_task_woken = pdFALSE;
    if(!mhandle || !(*mhandle))
    {
        return OS_EFAIL;
    }
    if(is_isr_context())
    {
        rev = xSemaphoreTakeFromISR(*mhandle, &higher_prio_task_woken);
        //port_switch_isr(higher_prio_task_woken);
    }
    else
    {
        rev = xSemaphoreTake(*mhandle, wait);
    }
    rev = (rev == pdTRUE) ? OS_EOK : OS_EFAIL;
    return rev;
}

int os_semaphore_put(os_semaphore_t *mhandle)
{
    int rev;
    signed portBASE_TYPE higher_prio_task_woken = pdFALSE;
    if(!mhandle || !(*mhandle))
    {
        return OS_EFAIL;
    }
    if(is_isr_context())
    {
        rev = xSemaphoreGiveFromISR(*mhandle, &higher_prio_task_woken);
        //port_switch_isr(higher_prio_task_woken);
    }
    else
    {
        rev = xSemaphoreGive(*mhandle);
    }
    rev = (rev == pdTRUE) ? OS_EOK : OS_EFAIL;
    return rev;
}

int os_semaphore_getcount(os_semaphore_t *mhandle)
{
    int count;
    count = uxQueueMessagesWaiting(*mhandle);
    return count;
}

int os_semaphore_delete(os_semaphore_t *mhandle)
{
    vSemaphoreDelete(*mhandle);
    return OS_EOK;
}

int os_mutex_create(os_mutex_t *mhandle, const char* name, int flags)
{
    if(flags == OS_MUTEX_NO_INHERIT)
    {
        *mhandle = NULL;
        return OS_EFAIL;
    }
    *mhandle = xSemaphoreCreateMutex();
    if(*mhandle)
    {
        return OS_EOK;
    }
    else
    {
        return OS_EFAIL;
    }
}

int os_mutex_get(os_mutex_t *mhandle, unsigned long wait)
{
    int rev;
    rev = xSemaphoreTake(*mhandle, wait);
    rev = (rev == pdTRUE) ? OS_EOK : OS_EFAIL;
    return rev;
}

int os_mutex_put(os_mutex_t *mhandle)
{
    int rev;
    if(is_isr_context())
    {
        printf("[os]os_mutex_put in isr context)");
        while(1);
    }
    rev = xSemaphoreGive(*mhandle);
    rev = (rev == pdTRUE) ? OS_EOK : OS_EFAIL;
    return rev;
}

int os_mutex_delete(os_mutex_t *mhandle)
{
    vSemaphoreDelete(*mhandle);
    return OS_EOK;
}

#define OS_MEM_LOG      0 //mem_alloc和mem_free的时候是否打印log

void* os_mem_alloc(unsigned int size)
{
    void* ptr;
    ptr = malloc(size);
    if(ptr)
    {
        memset(ptr, 0, size);
    }
#if OS_MEM_LOG == 1
    printf("[os]m a:0x%x [%s][%d]\n", (uint32_t)ptr, pcTaskGetTaskName(NULL) ,size);
#endif
    return ptr;
}

/*
 * 申请一段内存并且清零
 */
void* os_mem_calloc(unsigned int num, unsigned int size)
{
    void* ptr = os_mem_alloc(num * size);
    if(ptr)
    {
        memset(ptr, 0, num * size);
    }
    return ptr;
}

void* os_mem_realloc(void* old_ptr, unsigned int new_size)
{
    return realloc(old_ptr, new_size);
}

void os_mem_free(void* ptr)
{
#if OS_MEM_LOG == 1
    printf("[os]m f:0x%x [%s]\n", (uint32_t)ptr, pcTaskGetTaskName(NULL));
#endif
    free(ptr);
}

void* os_mem_alloc_ext(unsigned int size, int type)
{
    switch(type)
    {
        case OS_MEM_IRAM:
            return heap_caps_malloc(size, MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
        case OS_MEM_ERAM:
            return heap_caps_malloc(size, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
        default:
            return NULL;
    }
    return NULL;
}

void *os_mem_calloc_ext(unsigned int num, unsigned int size, int32_t type)
{
    void *ptr = os_mem_alloc_ext(num * size, type);
    if(ptr)
    {
        memset(ptr, 0, num * size);
    }
    return ptr;
}

void* os_mem_realloc_ext(void* old_ptr, unsigned int new_size, int type)
{
    switch(type)
    {
        case OS_MEM_IRAM:
            return heap_caps_realloc(old_ptr, new_size, MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
        case OS_MEM_ERAM:
            return heap_caps_realloc(old_ptr, new_size, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
        default:
            return NULL;
    }
    return NULL;
}

void os_mem_free_ext(void* ptr, int type)
{
    heap_caps_free(ptr);
}

int os_rwlock_create_with_cb(os_rw_lock_t *lock, const char *mutex_name, const char *lock_name, cb_fun r_fn)
{
    int rev;
    rev = os_mutex_create(&(lock->reader_mutex), mutex_name, OS_MUTEX_INHERIT);
    if(rev != OS_EOK)
    {
        return OS_EFAIL;
    }
    //rev = os_semaphore_create(&(lock->rw_lock), lock_name)
    rev = os_semaphore_create_binary(&(lock->rw_lock), lock_name);
    if(rev != OS_EOK)
    {
        return OS_EFAIL;
    }
    lock->reader_count = 0;
    lock->reader_cb = r_fn; 
    return OS_EOK;
}

#define RWLOCK_EN   1 //读写锁是否生效

int os_rwlock_create(os_rw_lock_t *lock, const char *mutex_name, const char *lock_name)
{
#if RWLOCK_EN == 0
    return 0;
#else
    return os_rwlock_create_with_cb(lock, mutex_name, lock_name, NULL);
#endif
}

int os_rwlock_delete(os_rw_lock_t *lock)
{
#if RWLOCK_EN == 0
    return 0;
#else
    lock->reader_cb = NULL;
    os_semaphore_delete(&(lock->rw_lock));
    os_mutex_delete(&(lock->reader_mutex));
    lock->reader_count = 0;
    return OS_EOK;
#endif
}

int os_rwlock_write_lock(os_rw_lock_t *lock, unsigned int wait_time)
{
#if RWLOCK_EN == 0
    return 1;
#else
    int rev = os_semaphore_get(&(lock->rw_lock), wait_time);
    //printf("[rw_lock]write_lock count:%d sem get rev:%d\n", lock->reader_count, rev);
    return rev;
#endif
}

int os_rwlock_write_unlock(os_rw_lock_t *lock)
{
#if RWLOCK_EN == 0
    return 1;
#else
    int rev;
    rev = os_semaphore_put(&(lock->rw_lock));
    //printf("[rw_lock}write_unlock count:%d\n", lock->reader_count);
    return rev;
#endif
}

int os_rwlock_read_lock(os_rw_lock_t *lock, unsigned int wait_time)
{
#if RWLOCK_EN == 0
    return 0;
#else
    int rev = OS_EOK;
    rev = os_mutex_get(&(lock->reader_mutex), wait_time);
    if(rev != OS_EOK)
    {
        //printf("[rw_lock]read_lock mux get rev:%d\n", rev);
        return OS_EFAIL;
    }
    lock->reader_count++;
    if(lock->reader_count == 1)
    {
        if(lock->reader_cb)
        {
            rev = lock->reader_cb(lock, wait_time);
            if(rev != OS_EOK)
            {
                lock->reader_count--;
                os_mutex_put(&(lock->reader_mutex));
                return OS_EFAIL;
            }
        }
        else
        {
            rev = os_semaphore_get(&(lock->rw_lock), wait_time);
            //printf("[rw_lock]read_lock sem get rev:%d\n", rev);
            if(rev != OS_EOK)
            {
                lock->reader_count--;
                os_mutex_put(&(lock->reader_mutex));
                return rev;
            }
        }
    }
    //printf("[rw_lock]read_lock count:%d\n", lock->reader_count);
    os_mutex_put(&(lock->reader_mutex));
    return OS_EOK;
#endif
}

int os_rwlock_read_unlock(os_rw_lock_t *lock)
{
#if RWLOCK_EN == 0
    return 0;
#else
    int rev;
    rev = os_mutex_get(&(lock->reader_mutex), OS_WAIT_FOREVER);
    if(rev != OS_EOK)
    {
        return OS_EFAIL;
    }
    lock->reader_count--;
    if(lock->reader_count == 0)
    {
        os_semaphore_put(&(lock->rw_lock));
    }
    //printf("[rw_lock]read_unlock count:%d\n", lock->reader_count);
    os_mutex_put(&(lock->reader_mutex));
    return OS_EOK;
#endif
}

unsigned int os_ticks_get(void)
{
    return xTaskGetTickCount();
}

/*********************test**********************/

#if 0

#define TEST_SEM    0
#define TEST_MUX    1

os_semaphore_t sem;
os_mutex_t  mutex;

void entry1(void* arg)
{
    char* parm;
    int rev, cnt = 0;
    parm = (char*)arg;
    printf("This is Entry1 parm:%s\n", parm);
    while(1)
    {
#if TEST_SEM == 1
        rev = os_semaphore_get(&sem, OS_WAIT_FOREVER);
        printf("entry1 get sem rev:%d\n", rev);
#endif

#if TEST_MUX == 1
        rev = os_mutex_get(&mutex, 0);
        printf("entry1 get mutex rev:%d\n", rev);
#endif
        printf("entry1 cnt:%d\n", cnt++);
    }
    printf("entry1 over\n");
    os_thread_delete(NULL);
}

void entry2(void* arg)
{
    char* parm;
    int cnt = 0;
    parm = (char*)arg;
    printf("This is Entry2\n");
    while(cnt < 20)
    {
        printf("entry2:%s------>cnt:%d\n", parm, cnt++);
        os_thread_sleep(1000);
#if TEST_SEM == 1
        os_semaphore_put(&sem);
#endif
#if TEST_MUX == 1
        os_mutex_put(&mutex);
#endif
    }
    printf("entry2 over\n");
    os_thread_delete(NULL); 
}

void plat_os_test(void)
{
    int rev;
    os_thread_t handle1, handle2;
    os_thread_stack_define(stack1, 1024);
    os_thread_stack_define(stack2, 1024);
    printf("this is main of test.c\n");

    rev = os_semaphore_create_counting(&sem, "testsem", 10, 1);
    printf("os_semaphore create rev:%d\n", rev);
    rev = os_semaphore_put(&sem);
    printf("os_semaphore create rev:%d\n", rev);

    rev = os_mutex_create(&mutex, "testmutex", OS_MUTEX_NO_INHERIT);
    printf("os_mutex_create rev:%d\n", rev);
     
    rev = os_thread_create(&handle1, NULL, entry1, "parm1", &stack1, 0);
    printf("create entry1 rev:%d handle:0x%x\n", rev, (unsigned int)handle1);

    rev = os_thread_create(&handle2, NULL, entry2, "parm2", &stack2, 0);
    printf("create entry2 rev:%d handle:0x%x\n", rev, (unsigned int)handle2);

    os_thread_sleep(10000);
    printf("delete thread1\n");
    os_thread_delete(&handle1);
    os_thread_sleep(10000);
    printf("delete thread2\n");
    os_thread_delete(&handle2);

    printf("plat_test over\n");
}

#endif

void os_heap_free_show(char* parm)
{
    uint32_t heap_left, psram_left;
    heap_left = heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    psram_left = heap_caps_get_free_size(MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if(parm == NULL)
    {
        printf("[plat_os]<I:%dk, P:%dk>\n", heap_left / 1024, psram_left / 1024);
    }
    else
    {
        printf("[plat_os]%s <I:%dk, P:%dk>\n", (char*)parm, heap_left / 1024, psram_left / 1024);
    }
}

void os_thread_stack_free_show(void)
{
    char* name;
    uint32_t free_word;
    name = pcTaskGetTaskName(NULL);
    free_word = uxTaskGetStackHighWaterMark(NULL); 
    printf("[os]------------------------------->[%s]stack min free:%d\n", name, free_word);
}

char* list_buf = NULL;

#if 1
esp_err_t audio_sys_get_real_time_stats(void);

void os_thread_list_show(void)
{
#if 1
//extern void vTaskList( char * pcWriteBuffer );
    if(list_buf == NULL)
    {
        list_buf = os_mem_alloc(4 * 1024 + 128);
    }
    vTaskList(list_buf);
    printf("%s\n", list_buf);
#else
    audio_sys_get_real_time_stats();
#endif
}

#else

typedef struct
{
    char name[16];
    char status[2];
    int32_t prio;
    int32_t size;
    int32_t num;
} task_list_s;
task_list_s task_list[20] = {0};

void os_thread_list_show(void)
{
    if(list_buf == NULL)
    {
        list_buf = os_mem_alloc(4 * 1024 + 128);
    }
    vTaskList(list_buf);
    {
      char *p;
      int i;
      int flag = 0;
      p = strtok(list_buf, "\r\n");
      while (p != NULL)
      {
          sscanf(p, "%s%s%d%d%d", task_list[0].name, task_list[0].status, &task_list[0].prio, &task_list[0].size, &task_list[0].num);
          for (i=1; i<20; i++)
          {
              if (strlen(task_list[i].name) == 0)
              {
                  memcpy(&task_list[i], &task_list[0], sizeof(task_list_s));
                  flag = 1;
                  break;
              }
              else if (strcmp(task_list[i].name, task_list[0].name) == 0)
              {
                  if (task_list[i].size > task_list[0].size)
                  {
                      task_list[i].size = task_list[0].size;
                      flag = 1;
                  }
                  break;
            }
          }
          p = strtok(NULL, "\r\n");
      }
      if (flag)
      {
          for (i=1; i<20; i++)
          {
              if (strlen(task_list[i].name) == 0)
              {
                break;
              }
              printf("%-14s\t%s\t\%d\t%d\t%d\r\n", task_list[i].name, task_list[i].status, task_list[i].prio, task_list[i].size, task_list[i].num);
          }
      }
    }
}
#endif

