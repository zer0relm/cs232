/*
 * Name: AJ Vrieland (ajv234)
 * Date: 3-25-21
 * 
 * Use make to compile then ./main to run
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

//No debug was used in this version, because I restarted on a clean slate to try to get unstuck



#define NUM_VIALS 30 // Given number
//#define NUM_VIALS 10 //Smaller values to shorten testing
#define SHOTS_PER_VIAL 6
#define NUM_CLIENTS (NUM_VIALS * SHOTS_PER_VIAL) 
// #define NUM_CLIENTS 10 //Smaller values to shorten testing
#define NUM_NURSES 10
#define NUM_STATIONS NUM_NURSES
#define NUM_REGISTRATIONS_SIMULTANEOUSLY 4


/* global variables */
//This mutex is used to protect the usable_vials variable 
//when multiple nurses are done with their current vial at the same time
pthread_mutex_t vial_mutex = PTHREAD_MUTEX_INITIALIZER;

//nurse and client registration semaphores are used to sync 4 clients with 4 nurses
sem_t nurse_reg;
sem_t client_reg;

//station semaphore is used to ensure only 4 nurses are at the 4 station
sem_t station_s;

int status;

//usable_vials is used to ensure that the program is not edditing a constant
int usable_vials = NUM_VIALS;

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
    int current_vial;
    long int id = (long int)arg;
                fprintf(stderr, "%s: nurse %ld started\n", curr_time_s(), id);
    //while(true) did not work so this was a hack to get around that
    while (1==1){
            //a spin lock for if the mutex is held by another thread
            while (0 != pthread_mutex_lock(&vial_mutex)){
            
            }
            walk(1, 3);
            //if statement checks if there are any vials left, and if there are none left, the nurse thread leaves
            if (usable_vials > 0){
                 current_vial = usable_vials; 
                usable_vials --;
            }else{
                pthread_mutex_unlock(&vial_mutex);
                fprintf(stderr, "%s: nurse %ld is done\n", curr_time_s(), id);
                pthread_exit(NULL);
                return 0;
            }
            fprintf(stderr, "%s: nurse %ld is getting vial %i\n", curr_time_s(), id, current_vial);
            walk(1, 3);
            pthread_mutex_unlock(&vial_mutex);

            //makes sure that only four nurses are at stations at a time
            sem_wait(&station_s);
            for(int shots_left = SHOTS_PER_VIAL; shots_left > 0; shots_left--){
                //the rendezvous with client threads
                sem_post(&client_reg);
                fprintf(stderr, "%s: nurse %ld is waiting for client\n", curr_time_s(), id);
                sem_wait(&nurse_reg);
                fprintf(stderr, "%s: there are %i shots left in vial %i\n", curr_time_s(), shots_left - 1, current_vial);
            }  
            sem_post(&station_s);
    }

}

void *client(void *arg) {
    long int id = (long int)arg;

    fprintf(stderr, "%s: client %ld has arrived and is walking to register\n",
            curr_time_s(), id);
    walk(3, 10);

    fprintf(stderr, "%s: client %ld is regsitering\n", curr_time_s(), id);
    walk(3, 10);

    //the rendezvous with the nurse threads
    sem_wait(&client_reg);
    fprintf(stderr, "%s: client %ld is getting their vaccine\n", curr_time_s(), id);
    sem_post(&nurse_reg);

    fprintf(stderr, "%s: client %ld leaves the clinic!\n", curr_time_s(), id);


    pthread_exit(NULL);
}

int main() {
    //Initializing the rendezvous semaphores 
    //acts as kind of a pseudo bounded buffer
    status = sem_init(&nurse_reg, 0, 0);
    status = sem_init(&client_reg, 0, 0);
    //Initializes the station semaphore so only four nurses can access the stations at a time
    status = sem_init(&station_s, 0, 4);
    srand(time(0));
    //Initializing the nurse and client threads with sizes defined at start of file
    pthread_t nurse_threads[NUM_NURSES];
    pthread_t client_threads[NUM_CLIENTS];

    //creates the correct number of nurse threads
    for(int i = 0; i < NUM_NURSES; i++){
        void *number = (void *)(intptr_t)i;
        pthread_create(&nurse_threads[i], NULL, nurse, number);
    }
    //creates the correct number of client threads with a randomly generated delay
    for(int i = 0; i < NUM_CLIENTS; i++){
        walk(0, 1);
        void *number = (void *)(intptr_t)i;
        pthread_create(&client_threads[i], NULL, client, number);
    }
    
    

    pthread_exit(0);
}
