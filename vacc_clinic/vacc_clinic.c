/*
 * Your info here.
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define NUM_VIALS 30
#define SHOTS_PER_VIAL 6
#define NUM_CLIENTS (NUM_VIALS * SHOTS_PER_VIAL)
#define NUM_NURSES 10
#define NUM_STATIONS NUM_NURSES
#define NUM_REGISTRATIONS_SIMULTANEOUSLY 4

/* global variables */

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
    usleep(get_rand_in_range(lower, upper));
}

// arg is the nurses station number.
void *nurse(void *arg) {
    long int id = (long int)arg;

    fprintf(stderr, "%s: nurse %ld started\n", curr_time_s(), id);

    fprintf(stderr, "%s: nurse %ld is done\n", curr_time_s(), id);

    pthread_exit(NULL);
}

void *client(void *arg) {
    long int id = (long int)arg;

    fprintf(stderr, "%s: client %ld has arrived and is walking to register\n",
            curr_time_s(), id);

    fprintf(stderr, "%s: client %ld leaves the clinic!\n", curr_time_s(), id);

    pthread_exit(NULL);
}

int main() {
    srand(time(0));
<<<<<<< HEAD
    pthread_t nurse_threads[NUM_NURSES];
    pthread_t client_threads[NUM_CLIENTS];

    // pthread_create(&client_threads[0], NULL, client, NULL);
    // pthread_create(&nurse_threads[0], NULL, nurse, NULL);
    for(int i = 0; i < NUM_CLIENTS; i++){
        walk(0, 1);
        void *number = (void *)(intptr_t)i;
        pthread_create(&client_threads[i], NULL, client, number);
    }
    for(int i = 0; i < NUM_NURSES; i++){
        void *number = (void *)(intptr_t)i;
        pthread_create(&nurse_threads[i], NULL, nurse, number);
    }
    
=======
>>>>>>> 5d96e9425f14b36969344c6610aeba97c571ba85

    pthread_exit(0);
}
