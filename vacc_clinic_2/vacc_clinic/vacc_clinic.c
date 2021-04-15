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

#define DEBUG 0

#define NUM_VIALS 30
//#define NUM_VIALS 1
#define SHOTS_PER_VIAL 6
//#define SHOTS_PER_VIAL 6
#define NUM_CLIENTS (NUM_VIALS * SHOTS_PER_VIAL)
//#define NUM_CLIENTS 6
#define NUM_NURSES 10
//#define NUM_NURSES 1
#define NUM_STATIONS NUM_NURSES
#define NUM_REGISTRATIONS_SIMULTANEOUSLY 4


/* global variables */
int number_vials = NUM_VIALS;
pthread_mutex_t vial_mutex;

int station_buffer[NUM_STATIONS];
int sb_in = 0;
int sb_out = 0;
int sb_size = NUM_STATIONS;
int sb_count = 0;
pthread_mutex_t station_mutex;

int registration_buffer[NUM_REGISTRATIONS_SIMULTANEOUSLY];
int rb_in = 0;
int rb_out = 0;
int rb_size = NUM_REGISTRATIONS_SIMULTANEOUSLY;
int rb_count = 0;
pthread_mutex_t registration_mutex;

sem_t client_semaphore;
sem_t nurse_semaphore;
sem_t rvs1;
sem_t rvs2;


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
    //usleep(get_rand_in_range(lower, upper));
    sleep(get_rand_in_range(lower, upper));
}

// arg is the nurses station number.
void *nurse(void *arg) {
    long int id = (long int)arg;
    int shots = SHOTS_PER_VIAL;
    fprintf(stderr, "%s: nurse %ld started\n", curr_time_s(), id);


    while(1){
        walk(1, 3);
        pthread_mutex_lock(&vial_mutex);
        if (number_vials != 0 ){
            fprintf(stderr, "%s: nurse %ld is getting vial %i\n", curr_time_s(), id, number_vials);
            number_vials--;
            pthread_mutex_unlock(&vial_mutex);
        }else{
            pthread_mutex_unlock(&vial_mutex);
            fprintf(stderr, "%s: nurse %ld is leaving\n", curr_time_s(), id);
            pthread_exit(NULL);
        }
        
        for(unsigned i = 0; i < shots; i++){
            sem_wait(&rvs2);
        
    #if DEBUG
        fprintf(stderr, "nurse %ld is at rendezvous\n", id);
    #endif

            
            //move this
            pthread_mutex_lock(&registration_mutex);

    #if DEBUG
        fprintf(stderr, "nurse %ld entered the mutex\nrb_out = %i, rb_count = %i\n", id, rb_out, rb_count);
    #endif

            long int current_client = registration_buffer[rb_out];
            rb_out = (rb_out + 1) %rb_size;
            rb_count--;

    #if DEBUG
        fprintf(stderr, "rb_out = %i, rb_count = %i\n", rb_out, rb_count);
    #endif

            pthread_mutex_unlock(&registration_mutex);
            fprintf(stderr, "%s: nurse %ld is vaccinating client %ld\n", curr_time_s(), id, current_client);

            sem_post(&rvs1);
        
    #if DEBUG
        fprintf(stderr, "nurse %ld left the rendezvous\n", id);
    #endif
        } 
    }   
}

void *client(void *arg) {
    long int id = (long int)arg;

    fprintf(stderr, "%s: client %ld has arrived and is walking to register\n",
            curr_time_s(), id);
    walk(3, 10);

    sem_wait(&client_semaphore);
#if DEBUG
    fprintf(stderr, "%s: client %ld is in the semaphor\n", curr_time_s(), id);
#endif
    while(rb_count == rb_size){
        fprintf(stderr, "rb_count %i == rb_size %i", rb_count, rb_size);
        
    }
        
        
        walk(3, 10);

#if DEBUG
    fprintf(stderr, "%s: before: client_id: %i.  rb_in: %i.  rb_count:  %i.\n", curr_time_s(), id, rb_in, rb_count);
#endif

        pthread_mutex_lock(&registration_mutex);
        fprintf(stderr, "%s: client %ld is registering at desk %i\n",
                curr_time_s(), id, rb_in);
        registration_buffer[rb_in] = id;
        rb_in = (rb_in + 1) %rb_size;
        rb_count++;
        pthread_mutex_unlock(&registration_mutex);
        walk(3, 10);

#if DEBUG
    fprintf(stderr, "%s: after: client_id: %i rb_in: %i.  rb_count:  %i.\n", curr_time_s(), id, rb_in, rb_count);
#endif
    
    sem_post(&rvs2);
    
#if DEBUG
    fprintf(stderr, "%s: client %ld entered the rendezvous\n", curr_time_s(), id);
#endif

    sem_wait(&rvs1);

#if DEBUG
    fprintf(stderr, "%s: client %ld left the rendezvous\n", curr_time_s(), id);
#endif

    
    sem_post(&client_semaphore);

    fprintf(stderr, "%s: client %ld leaves the clinic!\n", curr_time_s(), id);

    pthread_exit(NULL);
}

int main() {
    srand(time(0));
    sem_init(&client_semaphore, 0, 4);
    sem_init(&nurse_semaphore, 0, 0);
    sem_init(&rvs1, 0, 0);
    sem_init(&rvs2, 0, 0);
    pthread_mutex_init(&registration_mutex, NULL);
    pthread_t nurse_threads[NUM_NURSES]; 
    pthread_t client_threads[NUM_CLIENTS]; 
    for (unsigned i = 0; i < NUM_NURSES; i++){
        void *number = (void *)(intptr_t)i;
        pthread_create(&nurse_threads[i], NULL, nurse, number);
    }
    for (unsigned i = 0; i < NUM_CLIENTS; i++){
        void *number = (void *)(intptr_t)i;
        pthread_create(&client_threads[i], NULL, client, number);
    }
    pthread_exit(0);
}
