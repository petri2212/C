/*
 * Main program
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <stdint.h>  	
#include <time.h>       
#include <math.h>
#include <stdbool.h>

#include "../lib/time_library.h"
#include "../lib/activity_library.h"

/*
 * init a mutex
 */
pthread_mutex_t  mutex_activity  = PTHREAD_MUTEX_INITIALIZER;
bool forchette[5] = {false, false, false, false, false};
/*
 * Function that implements the activity
 * the input parameter is used to change the computational cost
 */
void Attivita(int parameter) {
	// get the lock
	pthread_mutex_lock( &mutex_activity );
	

    if(parameter==0)
    {
       if(forchetta[4] == false && forchetta[parameter+1]==false){
         forchetta[4] = true;
         forchetta[parameter+1] = true;
       } 
    } else if(parameter==4){
        if(forchetta[0] == false && forchetta[parameter-1]==false){
         forchetta[4] = true;
         forchetta[parameter-1] = true;
       } else{
        if(forchetta[parameter+1] == false && forchetta[parameter-1]==false){
         forchetta[parameter+1] = true;
         forchetta[parameter-1] = true;

       }
    }
    }

  
	// access the critical sectio
    double result = 0;
    for (long i=0; i<parameter*1000*1000; i++) {
		result = result + sin(i) + cos(i) + i *tan(i);
		mario+=parameter;
    }

	
	// release the lock
	pthread_mutex_unlock( &mutex_activity );    	
}


	
	
	// release the lock
	pthread_mutex_unlock( &mutex_activity );    	
}


int main(int argc, char* argv[]) {
    pthread_t thread1, thread2, thread3, thread4, thread5;
    
    printf("fork %d\n\n\n",forchette[1] );
    int ret_err;
	
	/* Create the first periodic thread */
	t_activity_par activity_1;
	sprintf(activity_1.name,	"Attivita 1");
	activity_1.function = Attivita;
	activity_1.period = 800;
	activity_1.parameter = 0;
    ret_err = pthread_create( &thread1, NULL, PeriodicTask, (void*) &activity_1);
    handle_error(ret_err, "Error in creating PeriodicTask 1");

	/* Create the second periodic thread */
	t_activity_par activity_2;
	sprintf(activity_2.name,	"Attivita 2");
	activity_2.function = Attivita;
	activity_2.period = 800;
	activity_2.parameter = 1;
    ret_err = pthread_create( &thread2, NULL, PeriodicTask, (void*) &activity_2);
    handle_error(ret_err, "Error in creating PeriodicTask 2");

     //thread3
    t_activity_par activity_3;
	sprintf(activity_3.name,	"Attivita 3");
	activity_3.function = Attivita;
	activity_3.period = 800;
	activity_3.parameter = 2;
    ret_err = pthread_create( &thread3, NULL, PeriodicTask, (void*) &activity_3);
    handle_error(ret_err, "Error in creating PeriodicTask 3");

    //thread4
    t_activity_par activity_4;
	sprintf(activity_4.name,	"Attivita 4");
	activity_4.function = Attivita;
	activity_4.period = 800;
	activity_4.parameter = 3;
    ret_err = pthread_create( &thread4, NULL, PeriodicTask, (void*) &activity_4);
    handle_error(ret_err, "Error in creating PeriodicTask 4");

     //thread5
     t_activity_par activity_5;
	sprintf(activity_5.name,	"Attivita 5");
	activity_5.function = Attivita;
	activity_5.period = 800;
	activity_5.parameter = 3;
    ret_err = pthread_create( &thread5, NULL, PeriodicTask, (void*) &activity_5);
    handle_error(ret_err, "Error in creating PeriodicTask 5");

    /* Wait till threads are complete before main continues.  */
    pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);
    pthread_join( thread3, NULL);
	pthread_join( thread4, NULL);
    pthread_join( thread5, NULL);
	
       	   	
    exit(0);
}


