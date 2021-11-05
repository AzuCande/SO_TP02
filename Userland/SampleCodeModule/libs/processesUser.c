#include <processesUser.h>

void yield(){
    yieldSyscall();
}

uint64_t createProcess(void (*function)(), int foreground,char **argv, int *fds){
    uint64_t pid;
    createProcessSyscall(function, foreground, argv, &pid, fds);
    return pid;
}

void myExit(){
    uint64_t pid;
    getPidSyscall(&pid);
    int ans;
    killSyscall(pid,&ans);
    while (1);
}

uint64_t getPid(){
    uint64_t pid;
    getPidSyscall(&pid);
    return pid;
}

int nice(uint64_t pid, uint64_t prio){
    int ans;
    niceSyscall(pid, prio,&ans);
    return ans;
}

int block(uint64_t pid) {
    int ans;
    blockSyscall(pid, &ans);
    return ans;
}

int unblock(uint64_t pid) {
    int ans;
    unblockSyscall(pid, &ans);
    return ans;
}

int kill(uint64_t pid){
    int ans;
    killSyscall(pid,  &ans);
    return ans;
}