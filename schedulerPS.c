#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"

// PS scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    list_t * schedule_queue;
    job_t * input_job;
    uint64_t time;
    uint64_t unaccounted;
} scheduler_PS_t;

uint64_t check(uint64_t a1, uint64_t a2)
{
    if (a1 > a2)
        return a1;
    else
        return a2;
}

int PS_C(void* job1, void* job2) {
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

void decrease(list_t* schedule_queue, uint64_t time) {
    list_node_t* nn = schedule_queue->head;
    for (int i = 0; i < schedule_queue->count; i++) {
        job_t* job = (job_t*)nn->data;
        jobSetRemainingTime(job, jobGetRemainingTime(job) - time);
        nn = nn->next;
    }
}
// Creates and returns scheduler specific info
void* schedulerPSCreate()
{
    scheduler_PS_t* info = malloc(sizeof(scheduler_PS_t));
    if (info == NULL) {
        return NULL;
    }
    /* IMPLEMENT THIS */
    info->schedule_queue = list_create(PS_C);
    info->input_job = NULL;
    info->time = 0;
    info->unaccounted = 0;
    return info;
}

// Destroys scheduler specific info
void schedulerPSDestroy(void* schedulerInfo)
{
    scheduler_PS_t* info = (scheduler_PS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    list_destroy(info->schedule_queue);
    free(info);
}

// Called to schedule a new job in the queue
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// job - new job being added to the queue
// currentTime - the current simulated time
void schedulerPSScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_PS_t* info = (scheduler_PS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    if (info->input_job != NULL) {
        uint64_t ps = (currentTime - info->time + info->unaccounted) / list_count(info->schedule_queue);
        decrease(info->schedule_queue, ps);
        info->unaccounted = (info->unaccounted + currentTime - info->time) % list_count(info->schedule_queue);
        schedulerCancelNextCompletion(scheduler);
       // uint64_t prempt = jobGetRemainingTime(info->input_job) - (currentTime - info->time);
    }
    info -> time = currentTime;
    list_insert(info->schedule_queue, job);
    info->input_job = list_head(info->schedule_queue)->data;
    schedulerScheduleNextCompletion(scheduler, check((jobGetRemainingTime(info->input_job)*list_count(info->schedule_queue)) + (currentTime - info->unaccounted), currentTime));

}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerPSCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_PS_t* info = (scheduler_PS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    
    job_t *comp = list_head(info->schedule_queue)->data;
    uint64_t ps = (info->unaccounted + currentTime - info->time) / list_count(info->schedule_queue);
    info->unaccounted = (info->unaccounted + currentTime - info->time) % list_count(info->schedule_queue);
    decrease(info->schedule_queue, ps);
    list_remove(info->schedule_queue, list_head(info->schedule_queue));
    
    
    info->time = currentTime;
    if (list_count(info->schedule_queue) > 0) {
        info->input_job = list_head(info->schedule_queue)->data;
        schedulerScheduleNextCompletion(scheduler, check((jobGetRemainingTime(info->input_job)*list_count(info->schedule_queue)) + (currentTime - info->unaccounted), currentTime));
    }
    else {
        info->input_job = NULL;
    }
    return comp;
}
