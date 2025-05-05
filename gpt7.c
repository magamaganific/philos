#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h> // for sleep()

#define NUM_THREADS 5
#define NUM_RESOURCES 5

pthread_mutex_t resource_mutex[NUM_RESOURCES];
pthread_mutex_t turn_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t turn_cond = PTHREAD_COND_INITIALIZER;

int current_turn = 0;

typedef struct {
    int thread_id;
    int res1;
    int res2;
} thread_args;

void* thread_function(void* arg) {
    thread_args* args = (thread_args*)arg;
    int id = args->thread_id;
    int r1 = args->res1;
    int r2 = args->res2;

    struct timeval start_time, end_time;

    pthread_mutex_lock(&turn_mutex);
    while (id != current_turn) {
        pthread_cond_wait(&turn_cond, &turn_mutex);
    }

    gettimeofday(&start_time, NULL); // Start timing before resource locking

    // Lock resources in consistent order
    if (r1 < r2) {
        pthread_mutex_lock(&resource_mutex[r1]);
        pthread_mutex_lock(&resource_mutex[r2]);
    } else {
        pthread_mutex_lock(&resource_mutex[r2]);
        pthread_mutex_lock(&resource_mutex[r1]);
    }

    // Simulate work
    printf("Thread %d accessing resources %d and %d\n", id, r1, r2);
    sleep(1);

    gettimeofday(&end_time, NULL); // End timing after work is done

    // Unlock resources
    pthread_mutex_unlock(&resource_mutex[r1]);
    pthread_mutex_unlock(&resource_mutex[r2]);

    // Advance turn
    current_turn++;
    pthread_cond_broadcast(&turn_cond);
    pthread_mutex_unlock(&turn_mutex);

    // Calculate elapsed time in microseconds
    long seconds = end_time.tv_sec - start_time.tv_sec;
    long useconds = end_time.tv_usec - start_time.tv_usec;
    long duration_us = seconds * 1000000 + useconds;

    printf("Thread %d took %ld microseconds to complete its task.\n", id, duration_us);

    free(arg);
    return NULL;
}
