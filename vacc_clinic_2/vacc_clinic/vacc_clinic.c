/*
 * AJ Vrieland (ajv234)
 * Date: 04/10/21
 * Thank you again for letting me re-do this assignment
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
//#define NUM_VIALS 1  //Used as a debug value
#define SHOTS_PER_VIAL 6
//#define SHOTS_PER_VIAL 6 //Used as a debug value
#define NUM_CLIENTS (NUM_VIALS * SHOTS_PER_VIAL)
//#define NUM_CLIENTS 6  //Used as a debug value
#define NUM_NURSES 10
//#define NUM_NURSES 1  //Used as a debug value
#define NUM_STATIONS NUM_NURSES
#define NUM_REGISTRATIONS_SIMULTANEOUSLY 4


/* global variables */
int number_vials = NUM_VIALS;
pthread_mutex_t vial_mutex;

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
    /* So this says to use usleep() which is in mircoseconds, and the assignment on the google docs says
     * for the deleay to be in seconds. I created both, since they are a single line a piece, but the 
     * disconnect is a bit confusing
     */


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

    // While loop is for when a vial runs out of shots, and the nurse needs to check to see if ther are more vials available
    while(1){
        walk(1, 3);
        //vial_mutex only allows a single nurse to access the vials at a time
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
            //rvs1&2 are rendezvous semaphores in order to sync up nurse threads with 
            sem_wait(&rvs2);
           
            //locks access to the Registration buffer
            pthread_mutex_lock(&registration_mutex);

            long int current_client = registration_buffer[rb_out];
            rb_out = (rb_out + 1) %rb_size;
            rb_count--;

            pthread_mutex_unlock(&registration_mutex);
            fprintf(stderr, "%s: nurse %ld is vaccinating client %ld\n", curr_time_s(), id, current_client);

            sem_post(&rvs1);

        } 
    }   
}

void *client(void *arg) {
    long int id = (long int)arg;

    fprintf(stderr, "%s: client %ld has arrived and is walking to register\n",
            curr_time_s(), id);
    walk(3, 10);

    /* I was unsure of the exact details of this section so I only allowed four clients to be in this section at a time, and only
     * released the counting semaphore after a client got a shot. This means that there will only be four clients at a time will be waiting in the
     * rendezvous at a time. I think that the sem_post(&client_semaphore) should be above the rendezvous but I am unsure on how to emplement that
    */
    sem_wait(&client_semaphore);

    while(rb_count == rb_size){
        fprintf(stderr, "rb_count %i == rb_size %i", rb_count, rb_size);  
    }

        walk(3, 10);
        // adds the clients id to the registration buffer so that a nurse can 
        pthread_mutex_lock(&registration_mutex);
        fprintf(stderr, "%s: client %ld is registering at desk %i\n",
                curr_time_s(), id, rb_in);
        registration_buffer[rb_in] = id;
        rb_in = (rb_in + 1) %rb_size;
        rb_count++;
        pthread_mutex_unlock(&registration_mutex);
        walk(3, 10);

    //rendezvous
    sem_post(&rvs2);

    sem_wait(&rvs1);
   
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
        walk(0, 1);
        void *number = (void *)(intptr_t)i;
        pthread_create(&client_threads[i], NULL, client, number);
    }
    pthread_exit(0);
}
