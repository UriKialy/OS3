#include "graph_server.hpp"
#include "reactor.hpp"
#include <signal.h>
using namespace std;
bool running; // Global variable to control the main loop
#define PORT 9034   // Port we're listening on

void sproblem_handler(int signal)
{
    if (signal == SIGINT)
    {
        running = false;
    }
    if (signal == SIGALRM)
    {
        running = false;
    }
}

int main(int argc, char *argv[])
{

    signal(SIGINT, sproblem_handler);  // Handle Ctrl+C to terminate the program gracefully
   
}