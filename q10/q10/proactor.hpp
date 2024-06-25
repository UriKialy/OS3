// reactor.hpp

#ifndef REACTOR_HPP
#define REACTOR_HPP

#include <sys/epoll.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <pthread.h>
#include <functional>
#include <mutex>

typedef void (*ReactorFunc)(int);
typedef void* (*ProactorFunc)(int);

class Reactor {
public:
    Reactor();
    ~Reactor();
    void addFd(int fd, ReactorFunc func);
    void removeFd(int fd);
    void run();
    void stop();

private:
    int epoll_fd;
    bool running;
    std::map<int, ReactorFunc> handlers;
};

// Proactor functions
pthread_t startProactor(int sockfd, ProactorFunc threadFunc);
int stopProactor(pthread_t tid);

#endif // REACTOR_HPP
