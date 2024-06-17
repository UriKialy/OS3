#include "reactor.hpp"

using namespace std;

void *reactor::startReactor()
{
    running = true;
    while (running)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        int maxfd = -1;
        for (const auto &handler : handlers)
        {
            int fd = handler.first;
            FD_SET(fd, &readfds); // Add fd to the set of file descriptors to monitor
            if (fd > maxfd)
            {
                maxfd = fd; // Track the highest fd number for select
            }
        }

        if (maxfd == -1)
        {
            continue; // no file descriptors to monitor continue to the next one
        }
        struct timeval timeout = {3, 0}; // Timeout of 1 second
        int result = select(maxfd + 1, &readfds, nullptr, nullptr, &timeout);
        if (result == -1)
        {
            perror("select");   // Print error if select fails
            exit(EXIT_FAILURE); // Exit program on select error
        }
        else if (result == 0)
        {
            // Timeout occurred, continue to check for events
            continue;
        }

        // Check each file descriptor for activity
        for (int fd = 0; fd <= maxfd; ++fd)
        {
            if (FD_ISSET(fd, &readfds))
            {
                // If fd is set in the read_fds set, call its handler function
                if (handlers.find(fd) != handlers.end())
                {
                    handlers[fd](fd); // Call the handler function for the fd
                }
            }
        }
    }
}
/*    void* reactor::fdhandler(int fd){
        char buffer[1024];
        ssize_t n = read(fd, buffer, sizeof(buffer)); // Read data from the fd into buffer
        if (n > 0)
        {
            std::cout << "Received " << n << " bytes from client on fd " << fd << ": " << std::string(buffer, n) << std::endl;
            addFdToReactor(this,fd,handlers[fd]);
        }
        else if (n == 0)
        {
            std::cout << "Client closed connection on fd " << fd << std::endl;
            removeFdFromReactor(this, fd); // Remove the fd from the reactor
        }
        else
        {
            perror("read"); // Print error if read fails
        }
        return nullptr; // Return nullptr (not used in this example)
    }*/

int reactor::stopReactor(void *reactor)
{
    running = false;
}

int reactor::addFdToReactor(void *reactor, int fd, reactorFunc func)
{
    if (running == false)
    {
        return -1;
    }
    else
    {
        handlers[fd] = func;
        return 0;
    }
}

int reactor::removeFdFromReactor(void *reactor, int fd)
{
    if (running == false)
    {
        return -1;
    }
    else
    {
        handlers.erase(fd);
        return 0;
    }
}
