#include <pipesUser.h>

int pipe_open(uint32_t id) {
    int ans = 0;
    pipeOpenSyscall(id, &ans);
    return ans;
}

int pipe_close(uint32_t id) {
    int ans = 0;
    pipeCloseSyscall(id, &ans);
    return ans;
}

int pipe_read(uint32_t id) {
    int ans = 0;
    pipeReadSyscall(id, &ans);
    return ans;
}

int pipe_write(uint32_t id) {
    char buffer[2048];
    int ans = 0;
    pipeWriteSyscall(id, buffer, &ans);
    return ans;
}
