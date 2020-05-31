/*created by Omer Kalif 31.5.20*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h> // for wait
#include <sys/wait.h>  // for wait

#define MAX 5

int val = 0;
int fd[2];
int pingPong()
{
   int x = 0;
   read(fd[0], &x, sizeof(x));
   //printf("val is = %d\n", x);
   printf("%d\n", x);
   val = x + 1;
   write(fd[1], &val, sizeof(val));
   return val;
}

void catch_sigusr1(int sig_num)
{
   signal(SIGUSR1, catch_sigusr1);
   //printf("process %d caught signal SIGUSR1\n", getpid());
}

void do_son()
{
   
   int i;
   sleep(1);

   if (val < MAX)
      pingPong();
   kill(getppid(), SIGUSR1);
   pause();
}

void do_dad(pid_t son_pid)
{
   
   int i;

   //printf("pid of dad = %d, pid of son = %d\n", getpid(), son_pid);

   pause();
   if (val < MAX)
      pingPong();
   kill(son_pid, SIGUSR1);
}

int main()
{
   pipe(fd);
   write(fd[1], &val, sizeof(val));
   pid_t pid;
   signal(SIGUSR1, catch_sigusr1);
   pid = fork();
   while (val < MAX)
   {
      sleep(1);
      switch (pid)
      {
      case -1:
         printf("Fork error\n");
         return -1;
         break;
      case 0:
         do_son();
         break;

      default:
         do_dad(pid);
         break;
      }
   }
   if(pid == 0){
      printf("Child is going to be terminated\n");
      kill(pid, SIGKILL);
      
   }
   else{
      printf("Parent is going to be terminated\n");
      kill(pid, SIGKILL);
      
   }
   
   close(fd[1]);
   close(fd[0]);

   return 0;
}