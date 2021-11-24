#include <semaphoresUser.h>

int sem_open(uint32_t id, uint32_t initValue) {
    int ans = 0;
    openSemaphoreSyscall(id, initValue, &ans);
    return ans;
}

int sem_wait(uint32_t id) {
    int ans = 0;
    waitSemaphoreSyscall(id, &ans);
    return ans;
}

int sem_post(uint32_t id) {
    int ans = 0;
    postSemaphoreSyscall(id, &ans);
    return ans;
}

int sem_close(uint32_t id) {
    int ans = 0;
    closeSemaphoreSyscall(id, &ans);
    return ans;
}

void sem_status() {
    char buffer[2048];
    semSyscall(buffer);
    printf("%s\n", buffer);
}