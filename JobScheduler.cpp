// // class JobScheduler {
// //   void schedule(Runnable job, long delayMs);
// // }
// // 
// // - The scheduler should handle executing the jobs.
// // - Jobs should run promptly at their timeout if nothing else is running, but it's ok to fall behind the schedule if jobs take too long to execute and block other jobs.
// // - Do not use any built-in schedulers for your language
// //  */

#include<bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
using namespace std;


// define global variables
long long mod = 1000000007;
pthread_mutex_t mutexBuffer;
priority_queue<pair<int , int>> job_que;
sem_t queue_empty;


void* put_job(void* arg)
{
  int job_id = rand()%mod;
  int current_time = int(std::time(nullptr));
  
  pthread_mutex_lock(&mutexBuffer); // mutex_lock
  
  job_que.push({current_time, job_id});
  sem_post(&queue_empty); // sem post/ notify
  cout << "Push Job Id -" << job_id << " at time " << current_time << endl;
  
  pthread_mutex_unlock(&mutexBuffer); // mutex_unlock
  
}

void* process_job(void* arg)
{
  while(true)
  {
    if (job_que.empty() == false)
    {
      int current_time = int(std::time(nullptr));
      
      pthread_mutex_lock(&mutexBuffer); // mutex_lock
      sem_wait(&queue_empty); // semphore wait 
      
      if (job_que.top().first <= current_time)
      {
        pair<int , int> job_details = job_que.top(); job_que.pop();
        
        int job_id = job_details.second;
        int expected_time_to_start = job_details.first;
        
        //process_job_with_id(job_details.second);
        int time_to_run = rand()%3; 
        sleep(time_to_run); // let's assume all job took time_to_run sec to run
        
        cout << "Run Job Id - " << job_id  << " Expected Time to start" << expected_time_to_start << endl;
      }
      pthread_mutex_unlock(&mutexBuffer);
      
    }
  }
}

int main()
{
  pthread_t th[5];
  sem_init(&queue_empty, 0, 0); // intitalize semaphore
  pthread_mutex_init(&mutexBuffer, NULL); // intialize mutex
  
    for (int i = 0; i < 5; i++) {
        if (i > 0) {
            if (pthread_create(&th[i], NULL, &put_job , NULL) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &process_job, NULL) != 0) {
                perror("Failed to create thread");
            }
        }
    }
    for (int i = 0; i < 5; i++) {
          if (pthread_join(th[i], NULL) != 0) {
              perror("Failed to join thread");
          }
      }
  sem_destroy(&queue_empty); // destroy semaphore
  pthread_mutex_destroy(&mutexBuffer); // destroy mutex
  return 0;
}


