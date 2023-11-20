/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                              uC/OS-II
*                                          Application Hooks
*
* Filename : app_hooks.c
* Version  : V2.92.13
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <os.h>


/*
*********************************************************************************************************
*                                      EXTERN  GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
**                                        uC/OS-II APP HOOKS
*********************************************************************************************************
*********************************************************************************************************
*/

void OutFileInit()
{
    /* Clear the file */
    if ((Output_err = fopen_s(&Output_fp, OUTPUT_FILE_NAME, "w")) == 0)
    {   
        printf("%s open success \n", OUTPUT_FILE_NAME);
    }
    else
    {
        printf("Error to clear output file\n");
        fclose(Output_fp);
    }
}

#ifndef M11102155_PA2_PART_2_CUS

void InputFile()
{
    /*
    *  Read file
    *  Task Information :
    *  Task_ID ArriveTime ExecutionTime Periodic
    */

    errno_t err;
    if ((err = fopen_s(&fp, INPUT_FILE_NAME, "r")) == 0)
    {
        printf("The file 'TaskSet.txt' was opened\n");
    }
    else
    {
        printf("The file 'TaskSet.txt' was not opened\n");
    }

    char str[MAX];
    char* ptr;
    char* pTmp = NULL;
    int TaskInfo[INFO], i, j = 0;
    TASK_NUMBER = 0;

    while (!feof(fp))
    {
        i = 0;
        memset(str, 0, sizeof(str));
        fgets(str, sizeof(str) - 1, fp);
        ptr = strtok_s(str, " ", &pTmp);
        while (ptr != NULL)
        {
            TaskInfo[i] = atoi(ptr);
            ptr = strtok_s(NULL, " ", &pTmp);
            /* printf("Info : %d\n", task_info[i]); */
            if (i == 0)
            {
                TASK_NUMBER++;
                TaskParameter[j].TaskID = TASK_NUMBER;
            }
            else if (i == 1)
            {
                TaskParameter[j].TaskArriveTime = TaskInfo[i];
            }
            else if (i == 2)
            {
                TaskParameter[j].TaskExecutionTime = TaskInfo[i];
            }
            else if (i == 3)
            {
                TaskParameter[j].TaskPeriodic = TaskInfo[i];
            }
             
            i++;
        }

        /* Initial Priority */
        TaskParameter[j].TaskPriority = j;
        j++;
    }
    fclose(fp);
    /* read file */
}

#else
void InputFile()
{
    /*
    *  Read file
    *  Task Information :
    *  Task_ID ArriveTime ExecutionTime Periodic
    */

    errno_t err;
    if ((err = fopen_s(&fp, INPUT_FILE_NAME, "r")) == 0)
    {
        printf("The file 'TaskSet.txt' was opened\n");
    }
    else
    {
        printf("The file 'TaskSet.txt' was not opened\n");
    }

    if ((err = fopen_s(&fp_cus, CUS_INTPUT_FILE_NAME, "r")) == 0)
    {
        printf("The file 'AperiodicJobs.txt' was opened\n");
    }
    else
    {
        printf("The file 'AperiodicJobs.txt' was not opened\n");
    }

    char str[MAX];
    char* ptr;
    char* pTmp = NULL;
    int TaskInfo[INFO], i, j = 0;
    TASK_NUMBER = 0;

    while (!feof(fp))
    {
        i = 0;
        memset(str, 0, sizeof(str));
        fgets(str, sizeof(str) - 1, fp);
        ptr = strtok_s(str, " ", &pTmp);
        while (ptr != NULL)
        {
            TaskInfo[i] = atoi(ptr);
            ptr = strtok_s(NULL, " ", &pTmp);
            //printf("Info : %d\n", TaskInfo[i]);
            if (i == 0)
            {
                TASK_NUMBER++;
                TaskParameter[j].TaskID = TASK_NUMBER;
            }
            else if (i == 1)
            {
                TaskParameter[j].TaskArriveTime = TaskInfo[i];
            }
            else if (i == 2)
            {
                TaskParameter[j].TaskExecutionTime = TaskInfo[i];
            }
            else if (i == 3)
            {
                TaskParameter[j].TaskPeriodic = TaskInfo[i];
            }
            i++;
        }
        /* Initial Priority */
        TaskParameter[j].TaskPriority = j;
        j++;
    }

    cus_job_number = 0;
    i, j = 0;
    while (!feof(fp_cus))
    {
        i = 0;
        memset(str, 0, sizeof(str));
        fgets(str, sizeof(str) - 1, fp_cus);
        ptr = strtok_s(str, " ", &pTmp);
        while (ptr != NULL)
        {
            TaskInfo[i] = atoi(ptr);
            ptr = strtok_s(NULL, " ", &pTmp);
            //printf("Info : %d\n", TaskInfo[i]);
            if (i == 0)
            {
                cus_job_parameter[j].TaskID = cus_job_number;
                cus_job_number++;
                //printf(" cus_job_number %d\n ", cus_job_number);
            }
            else if (i == 1)
            {
                cus_job_parameter[j].TaskArriveTime = TaskInfo[i];
            }
            else if (i == 2)
            {
                cus_job_parameter[j].TaskExecutionTime = TaskInfo[i];
            }
            else if (i == 3)
            {
                cus_job_parameter[j].JobDeadline = TaskInfo[i];
            }
            i++;
        }
        /* Initial Priority */
        cus_job_parameter[j].TaskPriority = j;
        j++;
    }

    fclose(fp);
    fclose(fp_cus);
    /* read file */
}

#endif /* M11102155_PA2_PART_2_CUS */

#ifdef M11102155_PA2_PART_1_EDF

// Doing EDF Heap's node swapping.
void EDFHeapSwap(TASK_PAIR* a, TASK_PAIR* b)
{
    TASK_PAIR temp = *b;
    *b = *a;
    *a = temp;
}

// Doing the Heapify.
void EDFHeapify(int location)
{
    int mini = location;
    int left = (location* 2);
    int right = left + 1;

    if ((left <= edf_heap_info->num_item) && (edf_heap[left].deadline < edf_heap[mini].deadline))
    {
        mini = left;
    }

    if ((right <= edf_heap_info->num_item) && (edf_heap[right].deadline < edf_heap[mini].deadline))
    {
        mini = right;
    }

    if (mini != location)
    {
        EDFHeapSwap(&(edf_heap[location]), &(edf_heap[mini]));
        EDFHeapify(mini);
    }
}

// Delete the task from EDF Heap.
void EDFHeapDelete()
{  
    if (edf_heap_info->num_item > 0)
    {
        //printf("TICK %d :: delete < %d , %d >\n", OSTime, edf_heap[1].task_id, edf_heap[1].deadline);
        EDFHeapSwap(&(edf_heap[1]), &(edf_heap[edf_heap_info->num_item]));
        edf_heap_info->num_item--;
        EDFHeapify(1);
    }
}

// Insert the task into EDF Heap (minimum heap order as the deadline).
void EDFHeapInsert(INT16U insert_task_id, INT16U insert_task_deadline)
{
    //printf("TICK %d :: insert < %d , %d >\n", OSTime, insert_task_id, insert_task_deadline);
    edf_heap_info->num_item++;
    edf_heap[edf_heap_info->num_item].task_id = insert_task_id;
    edf_heap[edf_heap_info->num_item].deadline = insert_task_deadline;

    INT16U location = edf_heap_info->num_item;

    while ((edf_heap[location].deadline < edf_heap[(location / 2)].deadline) && ((location / 2) > 0))
    {

        EDFHeapSwap(&(edf_heap[location]), &(edf_heap[(location / 2)]));
        location = (location / 2);
    }

    if ((edf_heap[location].deadline == edf_heap[(location / 2)].deadline) && (edf_heap[location].task_id < edf_heap[(location / 2)].task_id) && ((location / 2) > 0))
    {
        EDFHeapSwap(&(edf_heap[location]), &(edf_heap[(location / 2)]));
    }

}

// Initial EDF Heap and set Idle task as the zero position of the heap.
void EDFHeapInit()
{
    edf_heap = (TASK_PAIR*)malloc((OS_MAX_TASKS + 1) * sizeof(TASK_PAIR));
    edf_heap_info = (EDF_HEAP_INFO*)malloc(1 * sizeof(EDF_HEAP_INFO));

    if (edf_heap == NULL | edf_heap_info == NULL)
    {
        printf("EDF HEAP malloc failed !!! \n");
    }
    else
    {
        printf("EDF HEAP malloc success !!! \n");
    }

    edf_heap_info->num_item = 0;
    edf_heap_info->size = OS_MAX_TASKS + 1;

    for (int edf_heap_id = 0; edf_heap_id < edf_heap_info->size; edf_heap_id++)
    {
        edf_heap[edf_heap_id].task_id = 0;
        edf_heap[edf_heap_id].deadline = 0;
    }

    // Assign the heap[0] as Idle task.
    edf_heap[0].task_id = 63;


    printf("EDF HEAP INIT DONE !!! \n");

}

#endif /* M11102155_PA2_PART_1_EDF */


#ifdef M11102155_PA2_PART_2_CUS
//INT16U job_id;
//INT16U arrive_time;
//INT16U execution_time;
//INT16U user_define_deadline;

//INT16U TaskID;
//INT16U TaskArriveTime;
//INT16U TaskExecutionTime;
//INT16U TaskPeriodic;
//INT16U TaskNumber;
//INT16U TaskPriority;

void CUSQInsert(INT16U job_id, INT16U arrive_time, INT16U execution_time, INT16U deadline)
{
    printf("......... %d %d %d %d\n", job_id, arrive_time, execution_time, deadline);
    if (cus_fifo_q_info->num_item != cus_fifo_q_info->size)
    {
        cus_fifo_q[cus_fifo_q_info->front].job_id = job_id;
        cus_fifo_q[cus_fifo_q_info->front].arrive_time = arrive_time;
        cus_fifo_q[cus_fifo_q_info->front].execution_time = execution_time;
        cus_fifo_q[cus_fifo_q_info->front].user_define_deadline = deadline;
        cus_fifo_q_info->front++;
        cus_fifo_q_info->num_item++;
    }
    else
    {
        printf("ERROR : os_core.c ... FIFO QUEUE overflow !!!\n");
    }
}

void CUSQInit()
{
    cus_fifo_q = (CUS_FIFO_Q_NODE*)malloc((OS_MAX_TASKS + 1) * sizeof(CUS_FIFO_Q_NODE));
    cus_fifo_q_info = (CUS_FIFO_Q_INFO*)malloc(1 * sizeof(CUS_FIFO_Q_INFO));

    if ((cus_fifo_q == NULL) | (cus_fifo_q_info == NULL))
    {
        printf("CUS FIFO Q malloc failed !!! \n");
    }
    else
    {
        printf("CUS FIFO Q success !!! \n");
    }

    cus_fifo_q_info->front = 0;
    cus_fifo_q_info->end = 0;
    cus_fifo_q_info->num_item = 0;
    cus_fifo_q_info->size = (OS_MAX_TASKS + 1);

    printf("END OF FIFO QUEUE INIT !!!\n");
}


#endif /* M11102155_PA2_PART_2_CUS */



#if (OS_APP_HOOKS_EN > 0)

/*
*********************************************************************************************************
*                                  TASK CREATION HOOK (APPLICATION)
*
* Description : This function is called when a task is created.
*
* Argument(s) : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void  App_TaskCreateHook (OS_TCB *ptcb)
{
#if (APP_CFG_PROBE_OS_PLUGIN_EN == DEF_ENABLED) && (OS_PROBE_HOOKS_EN > 0)
    OSProbe_TaskCreateHook(ptcb);
#endif
}


/*
*********************************************************************************************************
*                                  TASK DELETION HOOK (APPLICATION)
*
* Description : This function is called when a task is deleted.
*
* Argument(s) : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void  App_TaskDelHook (OS_TCB *ptcb)
{
    (void)ptcb;
}


/*
*********************************************************************************************************
*                                    IDLE TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskIdleHook(), which is called by the idle task.  This hook
*               has been added to allow you to do such things as STOP the CPU to conserve power.
*
* Argument(s) : none.
*
* Note(s)     : (1) Interrupts are enabled during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 251
void  App_TaskIdleHook (void)
{
}
#endif


/*
*********************************************************************************************************
*                                  STATISTIC TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskStatHook(), which is called every second by uC/OS-II's
*               statistics task.  This allows your application to add functionality to the statistics task.
*
* Argument(s) : none.
*********************************************************************************************************
*/

void  App_TaskStatHook (void)
{
}


/*
*********************************************************************************************************
*                                   TASK RETURN HOOK (APPLICATION)
*
* Description: This function is called if a task accidentally returns.  In other words, a task should
*              either be an infinite loop or delete itself when done.
*
* Arguments  : ptcb      is a pointer to the task control block of the task that is returning.
*
* Note(s)    : none
*********************************************************************************************************
*/


#if OS_VERSION >= 289
void  App_TaskReturnHook (OS_TCB  *ptcb)
{
    (void)ptcb;
}
#endif


/*
*********************************************************************************************************
*                                   TASK SWITCH HOOK (APPLICATION)
*
* Description : This function is called when a task switch is performed.  This allows you to perform other
*               operations during a context switch.
*
* Argument(s) : none.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*
*               (2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                   will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                  task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/

#if OS_TASK_SW_HOOK_EN > 0
void  App_TaskSwHook (void)
{
#if (APP_CFG_PROBE_OS_PLUGIN_EN > 0) && (OS_PROBE_HOOKS_EN > 0)
    OSProbe_TaskSwHook();
#endif
}
#endif


/*
*********************************************************************************************************
*                                   OS_TCBInit() HOOK (APPLICATION)
*
* Description : This function is called by OSTCBInitHook(), which is called by OS_TCBInit() after setting
*               up most of the TCB.
*
* Argument(s) : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)     : (1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 204
void  App_TCBInitHook (OS_TCB *ptcb)
{
    (void)ptcb;
}
#endif


/*
*********************************************************************************************************
*                                       TICK HOOK (APPLICATION)
*
* Description : This function is called every tick.
*
* Argument(s) : none.
*
* Note(s)     : (1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_TIME_TICK_HOOK_EN > 0
void  App_TimeTickHook (void)
{
#if (APP_CFG_PROBE_OS_PLUGIN_EN == DEF_ENABLED) && (OS_PROBE_HOOKS_EN > 0)
    OSProbe_TickHook();
#endif
}
#endif
#endif
