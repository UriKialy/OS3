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

void *handle_client(void *arg);

void Kosaraju(int client_fd);

void Newedge(int u, int v);

void Removeedge(int u, int v);

void Newgraph(int numVertices, int numEdges);
