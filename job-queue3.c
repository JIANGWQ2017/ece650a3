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

/* A semaphore counting the number of jobs in the queue.  */
sem_t job_queue_count;

/* Perform one-time initialization of the job queue.  */

void initialize_job_queue ()
{
  /* The queue is initially empty.  */
  job_queue = NULL;
  /* Initialize the semaphore which counts jobs in the queue.  Its
     initial value should be zero.  */
  sem_init (&job_queue_count, 0, 0);
}

/* Process queued jobs until the queue is empty.  */

void* dequeue_job (void* arg)
{
  while (1) {
    struct job* next_job;

    /* Wait on the job queue semaphore.  If its value is positive,
       indicating that the queue is not empty, decrement the count by
       one.  If the queue is empty, block until a new job is enqueued.  */
    sem_wait (&job_queue_count);

    /* Lock the mutex on the job queue.  */
    pthread_mutex_lock (&job_queue_mutex);
    /* Because of the semaphore, we know the queue is not empty.  Get
       the next available job.  */
    next_job = job_queue;
    /* Remove this job from the list.  */
    job_queue = job_queue->next;
    /* Unlock the mutex on the job queue, since we're done with the
       queue for now.  */
    pthread_mutex_unlock (&job_queue_mutex);

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

  /* Post to the semaphore to indicate another job is available.  If
     threads are blocked, waiting on the semaphore, one will become
     unblocked so it can process the job.  */
  	sem_post (&job_queue_count);

  /* Unlock the job queue mutex.  */
  	pthread_mutex_unlock (&job_queue_mutex);
	job_added();
  }
  return NULL;
}

int main(){

  pthread_t thread1_id;
  pthread_t thread2_id;

  pthread_create (&thread1_id, NULL, &dequeue_job, NULL);
  pthread_create (&thread2_id, NULL, &enqueue_job, NULL);

  pthread_join (thread1_id, NULL);
  pthread_join (thread2_id, NULL);

  return 0;
}
