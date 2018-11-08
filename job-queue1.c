#include <malloc.h>
#include <pthread.h>

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


/* Process queued jobs until the queue is empty.  */

void* dequeue_job (void* arg)
{
  while (job_queue != NULL) {
    /* Get the next available job.  */
    struct job* next_job = job_queue;
    /* Remove this job from the list.  */
    job_queue = job_queue->next;
    /* Carry out the work.  */
    process_job (next_job);
    /* Clean up.  */
    free (next_job);
  }
  return NULL;
}


void* enqueue_job (void* args)
{
  while(1){
        struct job* new_job;

        /* Allocate a new job object.  */
        new_job = (struct job*) malloc (sizeof (struct job));
        new_job->next = job_queue;
        job_queue = new_job;

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


