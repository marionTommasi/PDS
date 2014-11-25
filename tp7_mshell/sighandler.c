
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <assert.h>


#include "jobs.h"
#include "common.h"
#include "sighandlers.h"


/*
 * Signal - wrapper for the sigaction function
 */
int
signal_wrapper(int signum, handler_t *handler) 
{
  struct sigaction sa;
  sa.sa_handler = handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  
  assert(sigaction(signum, &sa, NULL) !=-1);

  return 0;
}


/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children
 */
void
sigchld_handler(int sig) 
{
  int status;
  pid_t pid;
  struct job_t *job;
  
  
  if (verbose)
    printf("sigchld_handler: entering\n");
  
  while ((pid = waitpid(-1, &status, WNOHANG|WUNTRACED)) >0) {
    
    if (WIFEXITED(status)) {
      printf("fils %d termine normalement\n", pid);
      jobs_deletejob(pid);
    }
    
    if (WIFSIGNALED(status)) {
      printf("fils %d termine a cause du signal %d\n", pid, WTERMSIG(status));
      jobs_deletejob(pid);
    }
    
    if (WIFSTOPPED(status)) {
      printf("fils %d stoppe\n", pid);
      job = jobs_getjobpid(pid);
      if (job != NULL) 
	job->jb_state=ST;
    }
    
  }    
  if (verbose)
    printf("sigchld_handler: exiting\n");
  
  return;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void
sigint_handler(int sig) 
{
  pid_t pid = jobs_fgpid();
  
  if (verbose)
    printf("sigint_handler: entering\n");
  
  if (pid != 0) 
    kill(-pid, sig);
  printf("kill %d (sigint)\n", pid); 
  
  
  if (verbose)
    printf("sigint_handler: exiting\n");
  
  return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void
sigtstp_handler(int sig) 
{
  pid_t pid = jobs_fgpid();
  
  if (verbose)
    printf("sigtstp_handler: entering\n");
  
  if (pid != 0) 
    kill(-pid, sig);
  printf("kill %d (sigtstp)\n", pid);

  if (verbose)
    printf("sigtstp_handler: exiting\n");
  
  return;
}
