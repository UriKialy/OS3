// reactor.cpp

#include "proactor.hpp"
using namespace std;

Reactor::Reactor() : running(false) {
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
}

Reactor::~Reactor() {
    close(epoll_fd);
}

void Reactor::addFd(int fd, ReactorFunc func) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
        perror("epoll_ctl: add");
        exit(EXIT_FAILURE);
    }

    handlers[fd] = func;
}

void Reactor::removeFd(int fd) {
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr) == -1) {
        perror("epoll_ctl: remove");
        exit(EXIT_FAILURE);
    }

    handlers.erase(fd);
}

void Reactor::run() {
    running = true;
    const int MAX_EVENTS = 100;
    struct epoll_event events[MAX_EVENTS];

    while (running) {
        int num_fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_fds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < num_fds; ++i) {
            int fd = events[i].data.fd;
            if (handlers.find(fd) != handlers.end()) {
                handlers[fd](fd);
            }
        }
    }
}

void Reactor::stop() {
    running = false;
}

// Proactor functions

struct ProactorData {
    int sockfd;
    ProactorFunc threadFunc;
};

void* proactorThread(void* arg) {
    ProactorData* data = (ProactorData*)arg;
    data->threadFunc(data->sockfd);
    delete data;
    return nullptr;
}

pthread_t startProactor(int sockfd, ProactorFunc threadFunc) {
    pthread_t tid;
    ProactorData* data = new ProactorData{sockfd, threadFunc};
    if (pthread_create(&tid, nullptr, proactorThread, data) != 0) {
        perror("pthread_create");
        delete data;
        exit(EXIT_FAILURE);
    }
    return tid;
}

int stopProactor(pthread_t tid) {
    return pthread_cancel(tid);
}
