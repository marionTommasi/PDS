
#include<stdio.h>
#include<unistd.h>
#include<assert.h>
#include "pipe.h"
#include "jobs.h"
#include "cmd.h"


void do_pipe(char *cmds[MAXCMDS][MAXARGS], int nbcmd, int bg) {
  int pfd[MAXCMDS][2]; /*nbcmd-1 utilisés, mais peut pas initialiser tab taille variable*/
  pid_t pid;
  int i, state;
  char cmdline[MAXLINE];

  if (verbose) 
    printf("entré dans do_pipe\n\tnbcmd = %d\n",nbcmd);

  assert( pipe (pfd[nbcmd-2]) != -1); 

  snprintf(cmdline, MAXLINE, "%s | %s", *cmds[0], *cmds[1]);

  switch (pid = fork()) {
  case -1 : 
    assert(0);

  case 0 : 

    setpgid(0,0);

    dup2(pfd[nbcmd-2][0], STDIN_FILENO);
    close(pfd[nbcmd-2][0]);
    close(pfd[nbcmd-2][1]);

    execvp(cmds[nbcmd-1][0], cmds[nbcmd-1]);
    assert(0);

  }

  for (i = nbcmd-2; i>0 ; i++) {

    close(pfd[i][0]);

    assert(pipe (pfd[i-1]) != -1);
  
    switch (fork()) {
    case -1 : 
      assert(0);
      
    case 0 : 
      setpgid(0, pid);
      
      dup2(pfd[i-1][0], STDIN_FILENO);
      close(pfd[i-1][0]);
      close(pfd[i-1][1]);
      
      dup2(pfd[i][1], STDOUT_FILENO);
      close(pfd[i][1]);
      
      execvp(cmds[i][0], cmds[i]);
      assert(0);
    }

    close(pfd[i-1][0]);
    close(pfd[i][0]);
  }

  switch (fork()) {

  case -1 : 
    assert(0);

  case 0 :
    assert(i==0);

    setpgid(0, pid);

    dup2(pfd[0][1], STDOUT_FILENO);
    close(pfd[0][1]);
    close(pfd[0][0]);

    execvp(cmds[0][0], cmds[0]);
    assert(0);
  }

  close(pfd[0][1]);

  if (verbose) 
    printf("\tcommande : %s\n", cmdline);

  if(bg)
    state = BG;
  else
    state = FG;
  assert(jobs_addjob(pid, state, cmdline) != -1);
  if (!bg)
    waitfg(pid);

  if (verbose) 
    printf("fin du do_pipe\n");

  return;
}

