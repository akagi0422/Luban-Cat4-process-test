#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

// fork函数有三种返回值,父进程的返回值就是子进程的PID,而子进程的返回值则是0,函数执行错误返回-1
// fork函数中父进程与子进程是一样的,子进程能干的活父进程也一样能干,单独使用fork意义不大

int main(void)
{
    pid_t pid;  //fork函数的返回值
    pid_t child_pid; //wait函数返回值
    int err; //execl()函数的返回值
    int status; //exit函数的返回值

    printf("This is a process demo!\n\n");

    pid = fork();  /*调用 fork()函数*/

    if(pid == -1)  /*通过 result 的值来判断 fork()函数的返回情况，首先进行出错处理*/
    {
        printf("Fork error\n");
    }

    else if (pid == 0) /*fork返回值为 0 代表子进程*/
    {
        printf("The fork returned value is %d, In child process!! My PID is %d\n\n", pid, getpid()); //getpid()函数 获取当前进程的pid
        err = execl("/bin/ls", "ls", "-la", NULL); //调用execl函数来替换子进程

        if (err < 0) //excel失败返回-1
        {
        printf("execl fail!\n\n");
        }

        sleep(1);
        printf("child-process %d is finish!\n", getpid());
        exit(0); //传入0表示正常退出
    }

    else /*fork返回值大于 0 返回值为子进程pid 代表父进程*/
    {
        printf("The fork returned value is %d, In father process!! My PID is %d\n\n", pid, getpid());
        child_pid = wait(&status); //wait函数返回子进程pid，并且把退出状态存入status中
        if (child_pid == pid) 
        {
            printf("Get exit child process id: %d\n",child_pid);
            printf("Get child exit status: %d\n\n",status);
        } 
        else 
        {
            printf("Some error occured.\n\n");
        }

        exit(0);
    }
}
/*
//  exec系列函数是直接将当前进程给替换掉的,当调用exec系列函数后,当前进程将不会再继续执行,所以示例程序中的“Done!”将不被输出
//  因为当前进程已经被替换了,一般情况下,exec系列函数函数是不会返回的,除非发生了错误
//  出现错误时,exec系列函数将返回-1,并且会设置错误变量errno
int main(void)
{
    int err;

    printf("this is a execl function test demo!\n\n");

    err = execl("/bin/ls", "ls", "-la", NULL);
    // execl()函数用于执行参数path字符串所代表的文件路径（必须指定路径）
    // 接下来是一系列可变参数,它们代表执行该文件时传递过去的argv[0]、argv[1]…argv[n],最后一个参数必须用空指针NULL作为结束的标志。
    // 此例中argv[0] = ls;argv[1] = -la;这个进程与在终端上运行”ls -la”产生的结果是一样的

    if (err < 0) {
        printf("execl fail!\n\n");
    }

    printf("Done!\n\n");
}
//  因此可通过调用fork()复制启动一个子进程,并且在子进程中调用exec系列函数替换子进程
//  把fork()和exec系列函数结合在一起使用就是创建一个新进程所需要的一切了。*/