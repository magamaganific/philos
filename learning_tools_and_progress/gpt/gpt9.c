#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct {
    int id;
    pthread_mutex_t *mutex;
    int *start_flag;
} ThreadData;

void *thread_func(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    // Spin-wait until start_flag is set
    while (1) {
        pthread_mutex_lock(data->mutex);
        int ready = *(data->start_flag);
        pthread_mutex_unlock(data->mutex);
        if (ready) break;
        // Optional: reduce CPU usage
        usleep(100);  // sleep 100 microseconds
    }

    // Record time
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("Thread %d started at: %ld.%06ld\n",
           data->id, tv.tv_sec, tv.tv_usec);

    // Simulate some work
    printf("Thread %d is doing work...\n", data->id);

    free(data);
    return NULL;
}

int main() {
    pthread_t threads[2];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int start_flag = 0;

    for (int i = 0; i < 2; i++) {
        ThreadData *data = malloc(sizeof(ThreadData));
        data->id = i;
        data->mutex = &mutex;
        data->start_flag = &start_flag;

        pthread_create(&threads[i], NULL, thread_func, data);
    }

    // Let threads spin up and wait
    usleep(100000); // 100 ms

    // Set the start flag
    pthread_mutex_lock(&mutex);
    start_flag = 1;
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}
