#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <algorithm>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <pthread.h>
#include <mutex>

// Function declarations
void Newgraph(int numVertices, int numEdges);
void Newedge(int u, int v);
void Removeedge(int u, int v);
void Kosaraju(int client_fd);
void* handle_client(void* arg);

extern std::mutex graph_mutex;

