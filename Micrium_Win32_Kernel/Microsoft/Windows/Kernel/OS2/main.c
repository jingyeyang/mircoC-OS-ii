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
*                                            EXAMPLE CODE
*
* Filename : main.c
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>

#include  "app_cfg.h"


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

#define TASK_STAKSIZE 2048
static  OS_STK  StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#if defined (M11102155_PA1_PART_2_RM) | defined (M11102155_PA2_PART_1_EDF)
static void task(void* p_arg);
#endif /* M11102155_PA1_PART_2_RM | M11102155_PA2_PART_1_EDF */

#ifdef M11102155_PA1_PART_1
static void task1(void* p_arg);
static void task2(void* p_arg);
#endif /* M11102155_PA1_PART_1 */

static  void  StartupTask (void  *p_arg);


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*
* Notes       : none
*********************************************************************************************************
*/

int  main (void)
{
#if OS_TASK_NAME_EN > 0u
    CPU_INT08U  os_err;
#endif


    CPU_IntInit();

    Mem_Init();                                                 /* Initialize Memory Managment Module                   */
    CPU_IntDis();                                               /* Disable all Interrupts                               */
    CPU_Init();                                                 /* Initialize the uC/CPU services                       */

    OSInit();                                                   /* Initialize uC/OS-II                                  */

  

    /* Initialize Output File */
    OutFileInit();

    /* Input File */
    InputFile();


#ifdef M11102155_PA2_PART_1_EDF

    // Init EDF heap.
    EDFHeapInit();

#endif /* M11102155_PA2_PART_1_EDF */

#ifdef M11102155_PA2_PART_2_CUS 

    // Init CUS FIFO queue.
    CUSQInit();

    // Insert the aperiodic job into CUS FIFO queue.
    int job_id;
    for (job_id = 0; job_id < cus_job_number; job_id++)
    {
        CUSQInsert(cus_job_parameter[job_id].TaskID, cus_job_parameter[job_id].TaskArriveTime, cus_job_parameter[job_id].TaskExecutionTime, cus_job_parameter[job_id].JobDeadline);
    }
    printf("donw insert CUS queue!!!\n");

    int test_q_ptr = cus_fifo_q_info -> end;
    while(test_q_ptr != cus_fifo_q_info -> front)
    {
        //printf("%d job in the CUS FIFO QUEUE : \n");
        printf("  job id = %d, arrive time = %d, execution time = %d, user defined deadline = %d\n", cus_fifo_q[test_q_ptr].job_id, cus_fifo_q[test_q_ptr].arrive_time, cus_fifo_q[test_q_ptr].execution_time, cus_fifo_q[test_q_ptr].user_define_deadline);
        test_q_ptr++;
    }


#endif /* M11102155_PA2_PART_2_CUS */


    /* Dynamic Create the Stack size */
    Task_STK = malloc(TASK_NUMBER * sizeof(int*));

    /* for each pointer, allocate storage for an array of int */
    int n;
    for (n = 0; n < TASK_NUMBER; n++)
    {
        Task_STK[n] = malloc(TASK_STAKSIZE * sizeof(int));
    }



#ifdef M11102155_PA1_PART_1

    /* Create Task Set */
    OSTaskCreateExt(task1,
        &TaskParameter[0],
        &Task_STK[0][TASK_STAKSIZE - 1],
        TaskParameter[0].TaskPriority,
        TaskParameter[0].TaskID,
        &Task_STK[0][0],
        TASK_STAKSIZE,
        &TaskParameter[0],
        (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

    OSTaskCreateExt(task2,
        &TaskParameter[1],
        &Task_STK[1][TASK_STAKSIZE - 1],
        TaskParameter[1].TaskPriority,
        TaskParameter[1].TaskID,
        &Task_STK[1][0],
        TASK_STAKSIZE,
        &TaskParameter[0],
        (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#endif // M11102155_PA1_PART_1


#ifdef M11102155_PA1_PART_2_RM

    for (n = 0; n < TASK_NUMBER; n++)
    {
        OSTaskCreateExt(task,
            &TaskParameter[n],
            &Task_STK[n][TASK_STAKSIZE - 1],
            TaskParameter[n].TaskPeriodic,
            TaskParameter[n].TaskID,
            &Task_STK[n][0],
            TASK_STAKSIZE,
            &TaskParameter[0],
            (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    }

#endif /* M11102155_PA1_PART_2_RM */


// Set task ID as priority.
#ifdef M11102155_PA2_PART_1_EDF

    printf(" NUMEBER OF task == %d\n", TASK_NUMBER);
    for (n = 0; n < TASK_NUMBER; n++)
    {
        OSTaskCreateExt(task,
            &TaskParameter[n],
            &Task_STK[n][TASK_STAKSIZE - 1],
            TaskParameter[n].TaskID,
            TaskParameter[n].TaskID,
            &Task_STK[n][0],
            TASK_STAKSIZE,
            &TaskParameter[0],
            (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    }


    printf(" after task create :: \n");
    for (int edf_heap_id = 1; edf_heap_id <= edf_heap_info->num_item; edf_heap_id++)
    {
        printf("< %d, %d >\n", edf_heap[edf_heap_id].task_id, edf_heap[edf_heap_id].deadline);
    }


#endif /* M11102155_PA2_PART_1_EDF */




 

//    OSTaskCreateExt( StartupTask,                               /* Create the startup task                              */
//                     0,
//                    &StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE - 1u],
//                     APP_CFG_STARTUP_TASK_PRIO,
//                     APP_CFG_STARTUP_TASK_PRIO,
//                    &StartupTaskStk[0u],
//                     APP_CFG_STARTUP_TASK_STK_SIZE,
//                     0u,
//                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
//
//#if OS_TASK_NAME_EN > 0u
//    OSTaskNameSet(         APP_CFG_STARTUP_TASK_PRIO,
//                  (INT8U *)"Startup Task",
//                           &os_err);
//#endif

#ifdef M11102155_HW1
    printf("Tick \t CurrentTask ID \t\t NextTask ID \t\t Number of ctx switches\n");
#endif /* M11102155_HW1 */

#if defined (M11102155_PA1_PART_2_RM) | defined (M11102155_PA2_PART_1_EDF)
    printf("Tick \t  Event \t CurrentTask ID \t NextTask ID \t ResponseTime \t PreemptionTime \t OSTimeDly\n");
#endif /* M11102155_PA1_PART_2_RM | M11102155_PA2_PART_1_EDF */



    OSTimeSet(0);
    OSStart();                                                  /* start multitasking (i.e. give control to uc/os-ii)   */

    while (DEF_ON) {                                            /* should never get here.                               */
        ;
    }
}


/*
*********************************************************************************************************
*                                            STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'StartupTask()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

#if defined (M11102155_PA1_PART_2_RM) | defined (M11102155_PA2_PART_1_EDF)

static void task(void* p_arg)
{
    task_para_set* task_date;
    task_date = p_arg;
    
    while (1)
    {
        // For every task keep executing until finish or preemptive
        while (OSTCBCur->num_recent_execute_time < OSTCBCur->total_execute_time);    // TimeTick interrupt happen before while loop end !!!

        OSTimeDly(0);
    }
}

#endif /* M11102155_PA1_PART_2_RM | M11102155_PA2_PART_1_EDF */




#ifdef M11102155_PA1_PART_1
static void task1(void* p_arg)
{
    task_para_set* task_date;
    task_date = p_arg;
    while (1)
    {
        //printf("Tick : %d, Hello from task %d\n", OSTime, task_date->TaskID);
        //printf("%d \t task(%d)(%d) \t %d\n", OSTime, task_date->TaskID, OSTCBCur->OSTCBCtxSwCtr, OSCtxSwCtr);
        OSTimeDly(task_date->TaskPeriodic);
        //if ((Output_err = fopen_s(&Output_fp, "./Output.txt", "a")) == 0)
        //{
        //    fprintf(Output_fp, "Tick : %d,Hello from task %d\n", OSTime, task_date->TaskID);
        //    fclose(Output_fp);
        //}
    }
}

static void task2(void* p_arg)
{
    task_para_set* task_date;
    task_date = p_arg;
    while (1)
    {
        //printf("Tick : %d, Hello from task %d\n", OSTime, task_date->TaskID);
        //printf("%d \t task(%d)(%d) \t %d\n", OSTime, task_date->TaskID, OSTCBCur->OSTCBCtxSwCtr, OSCtxSwCtr);
        OSTimeDly(task_date->TaskPeriodic);
        //if ((Output_err = fopen_s(&Output_fp, "./Output.txt", "a")) == 0)
        //{
        //    fprintf(Output_fp, "Tick : %d,Hello from task %d\n", OSTime, task_date->TaskID);
        //    fclose(Output_fp);
        //}
    }
}
#endif /* M11102155_PA1_PART_1 */


static  void  StartupTask (void *p_arg)
{
   (void)p_arg;

    OS_TRACE_INIT();                                            /* Initialize the uC/OS-II Trace recorder               */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
    
    APP_TRACE_DBG(("uCOS-III is Running...\n\r"));

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
        OSTimeDlyHMSM(0u, 0u, 1u, 0u);
		APP_TRACE_DBG(("Time: %d\n\r", OSTimeGet()));
    }
}

