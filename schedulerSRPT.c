#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"

// SRPT scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    list_t* schedule_queue;
    job_t * input_job;
    uint64_t time;
} scheduler_SRPT_t;

int SRPT_C(void* job1, void* job2) {
    job_t* first = (job_t*)job1;
    job_t* second = (job_t*)job2;
    if (jobGetRemainingTime(first) < jobGetRemainingTime(second)) {
        return -1;
    }
    if (jobGetRemainingTime(first) == jobGetRemainingTime(second) && jobGetId(first) < jobGetId(second)) {
        return -1;
    }
    else{
        return 1;
    }
}
// Creates and returns scheduler specific info
void* schedulerSRPTCreate()
{
    scheduler_SRPT_t* info = malloc(sizeof(scheduler_SRPT_t));
    if (info == NULL) {
        return NULL;
    }
    /* IMPLEMENT THIS */
    info->schedule_queue = list_create(SRPT_C);
    info->input_job = NULL;
    info->time = 0;
    return info;
}

// Destroys scheduler specific info
void schedulerSRPTDestroy(void* schedulerInfo)
{
    scheduler_SRPT_t* info = (scheduler_SRPT_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    list_destroy(info->schedule_queue);
    free(info);
}

// Called to schedule a new job in the queue
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// job - new job being added to the queue
// currentTime - the current simulated time
void schedulerSRPTScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_SRPT_t* info = (scheduler_SRPT_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    if (info->input_job != NULL) {
        job_t * curr = info->input_job;
        uint64_t prempt = jobGetRemainingTime(info->input_job) - (currentTime - info->time);
        jobSetRemainingTime(info->input_job, prempt);
        schedulerCancelNextCompletion(scheduler);
        info->time = currentTime;
        list_remove(info->schedule_queue, list_find(info->schedule_queue, curr));
        list_insert(info->schedule_queue, job);
        list_insert(info->schedule_queue, curr);
    }
    else{
        info->time = currentTime;
        list_insert(info->schedule_queue, job);

    }

   info->input_job = list_head(info->schedule_queue)->data;
   schedulerScheduleNextCompletion(scheduler, currentTime + jobGetRemainingTime(info->input_job));


}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerSRPTCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_SRPT_t* info = (scheduler_SRPT_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    info->time = currentTime;
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
