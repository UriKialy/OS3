#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "reactor.hpp"

#define PORT 3490 // The port users will be connecting to

// Function declarations
void Newgraph(int numVertices, int numEdges);
void Kosaraju(int client_fd);
void Newedge(int u, int v);
void Removeedge(int u, int v);
void* handle_client(int client_fd);

