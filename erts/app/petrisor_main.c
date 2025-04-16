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


// mutex on the queue of nuts and screw
 pthread_mutex_t  mutexOnQueue  = PTHREAD_MUTEX_INITIALIZER;

// shared variables : number of nuts and screw produced
int queue_drill = 0;				
int queue_cutter = 0;

// mutex on the Cart
 pthread_mutex_t  mutexOnCartArriving  = PTHREAD_MUTEX_INITIALIZER;

// condition variable on Cart
pthread_cond_t condOnCartArriving =  PTHREAD_COND_INITIALIZER; 

// shared variable for the cart_operation
bool cart_operation = false;



void ActivityCutter(int duration) {

	pthread_mutex_lock( &mutexOnQueue );
    if(queue_cutter <= 7){
        if (cart_operation == true){
            queue_cutter += 1;
         printf("                                    -----  CUTTER-WORK   queue = %d  \n", queue_cutter);
            
        }else{
            queue_cutter += 1;
         printf("                                    -----  CUTTER-WORK   queue = %d  \n", queue_cutter);
        }

    }else{
        if (cart_operation == true){
            queue_cutter += 1;
         printf("                                    -----  CUTTER-WAIT \n");   
        } else{
            queue_cutter = 0;
        }

    }
	pthread_mutex_unlock( &mutexOnQueue );    	
}

void ActivityDrill(int duration) {
	
	pthread_mutex_lock( &mutexOnQueue );

    if(queue_drill <= 9){
        if (cart_operation == true){
            queue_drill += 1;
         printf("-----  DRILL-WORK      queue = %d  \n", queue_drill);
            
        }else{
            queue_drill += 1;
         printf("-----  DRILL-WORK      queue = %d  \n", queue_drill);
        }

    }else{
        if (cart_operation == true){
            queue_drill += 1;
            printf("-----  DRILL-WAIT \n"); 
        } else{
            queue_drill = 0;
        }

    }

	pthread_mutex_unlock( &mutexOnQueue );  

}

void ActivityWorkCell(int duration) {
	/*
	 * check if there is a queue full
	 */ 
    pthread_mutex_lock( &mutexOnQueue );
	if(queue_drill == 10 || queue_cutter == 8) {
             pthread_mutex_lock( &mutexOnCartArriving );
			    pthread_cond_signal(&condOnCartArriving);
			 pthread_mutex_unlock( &mutexOnCartArriving );   	
		}
    pthread_mutex_unlock( &mutexOnQueue );


}
	


void* ActivityCartArriving(void* par) {
	int duration = *(int*) par;
	while(1) {
        
		pthread_mutex_lock( &mutexOnCartArriving );
		printf("\n*** *** *** CART WAITING  *** *** ***\n");
		    pthread_cond_wait(&condOnCartArriving, &mutexOnCartArriving);
		cart_operation = true;
		pthread_mutex_unlock( &mutexOnCartArriving );    	

		printf("\n*** *** *** CART ARRIVING *** *** ***\n");
		activity_load(duration);
		printf("\n*** *** *** CART  DONE *** *** ***\n");

		pthread_mutex_lock( &mutexOnCartArriving );
		cart_operation = false;
		pthread_mutex_unlock( &mutexOnCartArriving );    	
	}
	
}





int main(int argc, char* argv[]) {
    pthread_t thread1, thread2, thread3, thread4;
    int ret_err;
	
	// Initialization of the random generator. It should only be called once.
	srand(time(NULL));   

	
	int duration = 50;
    ret_err = pthread_create( &thread1, NULL, ActivityCartArriving, (void*) &duration);
    handle_error(ret_err, "Error in creating aPeriodicTask 1");



	
	t_activity_par activity_2;
	sprintf(activity_2.name,	"ActivityDrill ");
	activity_2.function = ActivityDrill;
	activity_2.period = 1000;
	activity_2.parameter = 500;
	activity_2.print = false;
    ret_err = pthread_create( &thread2, NULL, PeriodicTask, (void*) &activity_2);
    handle_error(ret_err, "Error in creating PeriodicTask 2");


	
	
	t_activity_par activity_3;
	sprintf(activity_3.name,	"ActivityCutter");
	activity_3.function = ActivityCutter;
	activity_3.period = 2000;
	activity_3.parameter = 750;
	activity_3.print = false;
    ret_err = pthread_create( &thread3, NULL, PeriodicTask, (void*) &activity_3);
    handle_error(ret_err, "Error in creating PeriodicTask 3");

    
	t_activity_par activity_4;
	sprintf(activity_4.name,	"ActivityWorkCell");
	activity_4.function = ActivityWorkCell;
	activity_4.period = 50;
	activity_4.parameter = 1;
	activity_4.print = false;
    ret_err = pthread_create( &thread4, NULL, PeriodicTask, (void*) &activity_4);
    handle_error(ret_err, "Error in creating PeriodicTask 4");

    /* Wait till threads are complete before main continues.  */
    pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);
	pthread_join( thread3, NULL);
	pthread_join( thread4, NULL);
       	   	
    exit(0);
}


