#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"

// PSJF scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    list_t* schedule_queue;
    job_t * input_job;
    uint64_t time;
} scheduler_PSJF_t;

int find_shortest_prempt(void* job1, void* job2) {
    job_t* first = (job_t*)job1;
    job_t* second = (job_t*)job2;
    if (jobGetJobTime(first) < jobGetJobTime(second)) {
        return -1;
    }
    if (jobGetJobTime(first) == jobGetJobTime(second) && jobGetId(first) < jobGetId(second)) {
        return -1;
    }
    else{
        return 1;
    }
}

// Creates and returns scheduler specific info
void* schedulerPSJFCreate()
{
    scheduler_PSJF_t* info = malloc(sizeof(scheduler_PSJF_t));
    if (info == NULL) {
        return NULL;
    }
    /* IMPLEMENT THIS */
    info->schedule_queue = list_create(find_shortest_prempt);
    info->input_job = NULL;
    info->time = 0;
    return info;
    
}

// Destroys scheduler specific info
void schedulerPSJFDestroy(void* schedulerInfo)
{
    scheduler_PSJF_t* info = (scheduler_PSJF_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    list_destroy(info->schedule_queue);
    free(info);
}

// Called to schedule a new job in the queue
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// job - new job being added to the queue
// currentTime - the current simulated time
void schedulerPSJFScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_PSJF_t* info = (scheduler_PSJF_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    if (info->input_job != NULL) {
        
        
        if (jobGetJobTime(job) < jobGetJobTime(info->input_job))
        {
            uint64_t prempt = jobGetRemainingTime(info->input_job) - (currentTime - info->time);
            jobSetRemainingTime(info->input_job, prempt);
            schedulerCancelNextCompletion(scheduler);
            info->time = currentTime;
            info->input_job = job;
            //list_insert(info->schedule_queue, job);
        }
        list_insert(info->schedule_queue, job);
        
        
        
    }
    else{
        info->time = currentTime;
        info->input_job = job;
        list_insert(info->schedule_queue, job);
    }
    
    schedulerScheduleNextCompletion(scheduler, currentTime + jobGetRemainingTime(job));
}


// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerPSJFCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_PSJF_t* info = (scheduler_PSJF_t*)schedulerInfo;
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
