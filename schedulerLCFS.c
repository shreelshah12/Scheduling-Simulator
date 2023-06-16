#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"

// LCFS scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    list_t* schedule_queue;
    job_t * input_job;
} scheduler_LCFS_t;

// Creates and returns scheduler specific info
void* schedulerLCFSCreate()
{
    scheduler_LCFS_t* info = malloc(sizeof(scheduler_LCFS_t));
    if (info == NULL) {
        return NULL;
    }
    /* IMPLEMENT THIS */
    info->schedule_queue = list_create(NULL);
    info->input_job = NULL;
    return info;
}

// Destroys scheduler specific info
void schedulerLCFSDestroy(void* schedulerInfo)
{
    scheduler_LCFS_t* info = (scheduler_LCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    list_destroy(info->schedule_queue);
    free(info);
}

// Called to schedule a new job in the queue
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// job - new job being added to the queue
// currentTime - the current simulated time
void schedulerLCFSScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_LCFS_t* info = (scheduler_LCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
   list_insert(info->schedule_queue, job);
    if (info->input_job == NULL) {
        schedulerScheduleNextCompletion(scheduler, currentTime + jobGetRemainingTime(job));
        info->input_job = job;
    }

    
}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerLCFSCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_LCFS_t* info = (scheduler_LCFS_t*)schedulerInfo;
    job_t *comp = info -> input_job;
    list_remove(info->schedule_queue, list_find(info->schedule_queue, comp));
    if (list_count(info->schedule_queue) > 0) {
        info->input_job = list_head(info->schedule_queue)->data;
        schedulerScheduleNextCompletion(scheduler, currentTime + jobGetRemainingTime(info->input_job));
        
    }
    else {
        info->input_job = NULL;
    }
    return comp;
    
}
