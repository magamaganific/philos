#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Struct to hold thread-specific data
typedef struct {
    int id;
    int value;
} ThreadData;

void *thread_func(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    printf("Thread %d received value: %d\n", data->id, data->value);
    free(data);  // Free memory if dynamically allocated
    return NULL;
}

int main() {
    pthread_t threads[2];

    for (int i = 0; i < 2; i++) {
        ThreadData *data = malloc(sizeof(ThreadData)); // Allocate per-thread data
        data->id = i;
        data->value = (i + 1) * 100;

        pthread_create(&threads[i], NULL, thread_func, data);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
