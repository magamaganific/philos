#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct {
    int id;
    int repetitions;              // How many times this thread should run
    pthread_mutex_t *mutex;
    int *start_flag;
} ThreadData;

void *thread_func(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    // Wait for start signal
    while (1) {
        pthread_mutex_lock(data->mutex);
        int ready = *(data->start_flag);
        pthread_mutex_unlock(data->mutex);
        if (ready) break;
        usleep(100);
    }

    // Record actual start time
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("Thread %d started at: %ld.%06ld\n", data->id, tv.tv_sec, tv.tv_usec);

    for (int i = 1; i <= data->repetitions; i++) {
        printf("Thread %d is performing repetition %d\n", data->id, i);
        usleep(50000);  // Simulate work: 50 ms
    }

    printf("Thread %d finished.\n", data->id);
    free(data);
    return NULL;
}

int main() {
    const int NUM_THREADS = 2;
    const int REPS_PER_THREAD = 10;

    pthread_t threads[NUM_THREADS];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int start_flag = 0;

    for (int i = 0; i < NUM_THREADS; i++) {
        ThreadData *data = malloc(sizeof(ThreadData));
        data->id = i;
        data->repetitions = REPS_PER_THREAD;
        data->mutex = &mutex;
        data->start_flag = &start_flag;

        pthread_create(&threads[i], NULL, thread_func, data);
    }

    usleep(100000); // Wait to ensure all threads are ready

    // Start all threads simultaneously
    pthread_mutex_lock(&mutex);
    start_flag = 1;
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}
