/*
 * Name: AJ Vrieland (ajv234)
 * Date: 3-22-21
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define DEBUG 1

// #define NUM_VIALS 30 // Given number
#define NUM_VIALS 10 //so it doesn't run forever when writing code
#define SHOTS_PER_VIAL 6
#define NUM_CLIENTS (NUM_VIALS * SHOTS_PER_VIAL)
#define NUM_NURSES 10
#define NUM_STATIONS NUM_NURSES
#define NUM_REGISTRATIONS_SIMULTANEOUSLY 4


/* global variables */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t nurse_reg;
sem_t client_reg;
int status;

int get_rand_in_range(int lower, int upper) {
    return (rand() % (upper - lower + 1)) + lower;
}

char *time_str;
char *curr_time_s() {
    time_t t;
    time(&t);
    time_str = ctime(&t);
    // replace ending newline with end-of-string.
    time_str[strlen(time_str) - 1] = '\0';
    return time_str;
}

// lower and upper are in seconds.
void walk(int lower, int upper) {
    // TODO: fill in code here.  Use usleep() and get_rand_in_range() from
    // above.
    sleep(get_rand_in_range(lower, upper));

}

// arg is the nurses station number.
void *nurse(void *arg) {

    long int id = (long int)arg;
    //int id = pthread_self();


#if DEBUG
    fprintf(stderr, "\nnurse id: %i\n", id);
#endif

    fprintf(stderr, "%s: nurse %ld started\n", curr_time_s(), id);
    walk(1, 3);
    fprintf(stderr, "%s: nurse %ld is done\n", curr_time_s(), id);
    pthread_exit(NULL);
}

void *client(void *arg) {
    long int id = (long int)arg;
    sem_wait(&client_reg);
    fprintf(stderr, "%s: client %ld has arrived and is walking to register\n",
            curr_time_s(), id);
    walk(3, 10);
    

    fprintf(stderr, "%s: client %ld leaves the clinic!\n", curr_time_s(), id);
    sem_post(&client_reg);

    pthread_exit(NULL);
}

int main() {
    status = sem_init(&nurse_reg, 0, 4);
    status = sem_init(&client_reg, 0, 4);
    srand(time(0));
    pthread_t nurse_threads[NUM_NURSES];
    pthread_t client_threads[NUM_CLIENTS];
    for(int i = 0; i < NUM_NURSES; i++){
        void *number = (void *)(intptr_t)i;
        pthread_create(&nurse_threads[i], NULL, nurse, number);
    }
    for(int i = 0; i < NUM_CLIENTS; i++){
        void *number = (void *)(intptr_t)i;
        pthread_create(&client_threads[i], NULL, client, number);
    }

    pthread_exit(0);
}
