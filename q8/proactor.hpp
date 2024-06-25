#pragma once
#include <sys/epoll.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <pthread.h>
#include <functional>
#include <mutex>

typedef void (*proactorFunc)(int);
typedef void* (*ProactorFunc)(int);

class proactor {
public:
    proactor();
    ~proactor();
    void addFd(int fd, proactorFunc func);
    void removeFd(int fd);
    void run();
    void stop();

private:
    int epoll_fd;
    bool running;
    std::map<int, proactorFunc> handlers;
};

// Proactor functions
pthread_t startProactor(int sockfd, ProactorFunc threadFunc);
int stopProactor(pthread_t tid);

