// reactor.hpp

#pragma once

#include <functional>
#include <unordered_map>
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>

using ReactorFunc = std::function<void(int)>;

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
    std::unordered_map<int, ReactorFunc> handlers;
};


