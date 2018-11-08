#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>

struct job {
  /* Link field for linked list.  */
  struct job* next; 

  /* Other fields describing work to be done... */
};

/* A linked list of pending jobs.  */
struct job* job_queue;

void process_job (){
	printf("done with job \n");
}

void job_added(){
	printf("job added \n");
}

/* A mutex protecting job_queue.  */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;


/* Process queued jobs until the queue is empty.  */

void* dequeue_job (void* arg)
{
  while (1) {
    struct job* next_job;

    /* Lock the mutex on the job queue.  */
    pthread_mutex_lock (&job_queue_mutex);
    
    if(job_queue == NULL){
    	next_job = NULL;
    }
    else{
    next_job = job_queue;
    job_queue = job_queue->next;
    }
    pthread_mutex_unlock (&job_queue_mutex);
    if(job_queue == NULL)
	    return NULL;
    /* Carry out the work.  */
    process_job ();
    /* Clean up.  */
    free (next_job);
  }
  return NULL;
}

/* Add a new job to the front of the job queue.  */

void* enqueue_job (void* args)
{
  while(1){
  	struct job* new_job;

  	/* Allocate a new job object.  */
  	new_job = (struct job*) malloc (sizeof (struct job));

	pthread_mutex_lock (&job_queue_mutex);
  	new_job->next = job_queue;
  	job_queue = new_job;


  	pthread_mutex_unlock (&job_queue_mutex);
	job_added();
  }
  return NULL;
}

int main(){

  pthread_t thread1_id;
  pthread_t thread2_id;


  pthread_create (&thread2_id, NULL, &enqueue_job, NULL);
  pthread_create (&thread1_id, NULL, &dequeue_job, NULL);
  pthread_join (thread1_id, NULL);
  pthread_join (thread2_id, NULL);

  return 0;
}
