#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <algorithm>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sstream>
#include <unordered_map>

class reactor
{
    typedef void *(*reactorFunc)(int fd);
    bool running = false;
    typedef reactor;
    std::unordered_map<int, reactorFunc> handlers; // Map of file descriptors to their handler functions
    // starts new reactor and returns pointer to it
    void *startReactor();
    // adds fd to Reactor (for reading) ; returns 0 on success.
    int addFdToReactor(void *reactor, int fd, reactorFunc func);
    // removes fd from reactor
    int removeFdFromReactor(void *reactor, int fd);
    // stops reactor
    int stopReactor(void *reactor);
    void * fdhandler(int fd);
};