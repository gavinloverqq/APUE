#include "apue.h"
#include<dirent.h>
#include <sys/wait.h>

//  ./ls /Users/wankun
//  实现ls
void ls(int argc,char **argv) {
    DIR 	       *dp;
    struct dirent *dirp;
    if(argc!=2)
        err_quit("usage:ls directory_name");
    if((dp=opendir(argv[1]))==NULL)
        err_sys("can't open %s",argv[1]);
    while((dirp=readdir(dp))!=NULL)
        printf("%s\n",dirp->d_name);
    closedir(dp);
    exit(0);
}

//  不带缓冲的IO
//  ./cp < data.in > data.out
//  把data.in的数据cp到data.out里

void cp(){
    const int BUFFERSIZE = 4096;
    char buf[BUFFERSIZE];
    ssize_t n;
    while ((n = read(STDIN_FILENO, buf, BUFFERSIZE)) > 0)
        if(write(STDOUT_FILENO, buf, n) != n)
            err_sys("write error");
    if(n < 0)
        err_sys("write error");
    exit(0);
}

//  标准IO
void cp_std(){
    int c;
    while ((c = getc(stdin)) != EOF)
        if(putc(c, stdout) == EOF)
            err_sys("out put error");
}

//  获取当前进程ID
void pid_display(){
    printf("process ID: %ld\n", (long)getpid());
}

//  类似shell功能

//  %ls
//  parent: process ID: 10909
//  child: process ID: 10910
//  CMakeCache.txt		Makefile		chapter1_unix.cbp
//  CMakeFiles		chapter1_unix		cmake_install.cmake

//  注意此shell无法使用带参数的命令 如: ls -al
//  注意pid的输出，父进程始终是一个，子进程号却在不断变化，每次子进程结束后，父进程在fork新的
void shell(){
    char buf[MAXLINE];// from apue.h referrnce
    pid_t pid;
    int status;
    printf("%%");
    while(fgets(buf,MAXLINE,stdin)!=NULL){
        if((buf[strlen(buf)-1])=='\n')
            buf[strlen(buf)-1]=0;//relplace newline with null，因为命令不是以/n结尾而是以null结尾

        printf("parent: ");//加入pid显示
        pid_display();

        if ( (pid = fork() ) < 0)//调用一次，返回两次，父进程返回一次，子进程返回一次
            err_sys("fork error");
        else if(pid==0){//子进程返回为0

            printf("child: ");//加入pid显示
            pid_display();

            execlp(buf,buf, (char*)0);
            err_ret("couldn't execute :%s ",buf);
            exit(127);
        }

        if((pid=waitpid(pid,&status,0))<0)//父进程等待子进程结束，waitpid返回的子进程的终止状态（status变量）
            err_sys("waitpaid error");
        printf("%%");
    }
    exit(0);
}

//  获取用户id与组id
void id(){
    printf("uid = %d, gid = %d", getuid(), getgid());
}

//  处理了中断信号的shell

//  信号处理函数
static void sig_int(int){
    printf("interrupt\n%%= ");
}

void shell_sig(){
    char buf[MAXLINE];// from apue.h referrnce
    pid_t pid;
    int status;

    if(signal(SIGINT, sig_int) == SIG_ERR)//处理信号
        err_sys("signal error");

    printf("%%");
    while(fgets(buf,MAXLINE,stdin)!=NULL){
        if((buf[strlen(buf)-1])=='\n')
            buf[strlen(buf)-1]=0;//relplace newline with null，因为命令不是以/n结尾而是以null结尾

        printf("parent: ");//加入pid显示
        pid_display();

        if ( (pid = fork() ) < 0)//调用一次，返回两次，父进程返回一次，子进程返回一次
            err_sys("fork error");
        else if(pid==0){//子进程返回为0

            printf("child: ");//加入pid显示
            pid_display();

            execlp(buf,buf, (char*)0);
            err_ret("couldn't execute :%s ",buf);
            exit(127);
        }

        if((pid=waitpid(pid,&status,0))<0)//父进程等待子进程结束，waitpid返回的子进程的终止状态（status变量）
            err_sys("waitpaid error");
        printf("%%");
    }
    exit(0);
}

int main(int argc,char **argv) {
//    ls(argc, argv);
//    cp();
//    cp_std();
//    pid_display();
//    shell();
//    id();
    shell_sig();
}