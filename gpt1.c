#include <stdio.h>
#include <pthread.h>

void* my_thread_function(void* arg) {
    printf("Hello from the thread!\n");
    return NULL;
}

int main() {
    pthread_t thread;

    // Create a thread
    if (pthread_create(&thread, NULL, my_thread_function, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    // Wait for the thread to finish
    if (pthread_join(thread, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }

    printf("Thread has finished executing.\n");
    return 0;
}
