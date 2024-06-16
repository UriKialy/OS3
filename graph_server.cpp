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

using namespace std;

#define PORT 9034   // Port we're listening on

// Define global variables for graph representation
int n = 0, m = 0; // Number of vertices and edges
vector<list<int>> adj; // Adjacency list for the graph
vector<list<int>> adjT; // Transpose adjacency list for Kosaraju's algorithm

void *handle_client(void *arg);

// Function to initialize a new graph
void Newgraph(int numVertices, int numEdges) {
    n = numVertices;
    m = numEdges;

    adj.clear();
    adj.resize(n);
    adjT.clear();
    adjT.resize(n);
}

// Kosaraju's algorithm function
void Kosaraju(int client_fd) {
    if (n <= 0 || m <= 0 || m > 2 * n) {
        string msg = "Invalid input\n";
        send(client_fd, msg.c_str(), msg.size(), 0);
        return;
    }

    vector<bool> visited(n, false);
    list<int> order;

    function<void(int)> dfs1 = [&](int u) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) {
                dfs1(v);
            }
        }
        order.push_back(u);
    };

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            dfs1(i);
        }
    }

    reverse(order.begin(), order.end());
    vector<int> component(n, -1);
    vector<list<int>> components; // To store the nodes of each component

    function<void(int, int)> dfs2 = [&](int u, int comp) {
        component[u] = comp;
        components[comp].push_back(u);
        for (int v : adjT[u]) {
            if (component[v] == -1) {
                dfs2(v, comp);
            }
        }
    };

    int comp = 0;
    for (int u : order) {
        if (component[u] == -1) {
            components.push_back(list<int>()); // Add a new component
            dfs2(u, comp++);
        }
    }

    string result = "Number of strongly connected components: " + to_string(comp) + "\n";
    for (int i = 0; i < comp; ++i) {
        result += "Component " + to_string(i + 1) + ": ";
        for (int node : components[i]) {
            result += to_string(node) + " ";
        }
        result += "\n";
    }

    send(client_fd, result.c_str(), result.size(), 0);
}

// Function to add a new edge
void Newedge(int u, int v) {
    adj[u].push_back(v);
    adjT[v].push_back(u);
}

// Function to remove an edge
void Removeedge(int u, int v) {
    auto it = find(adj[u].begin(), adj[u].end(), v);
    if (it != adj[u].end()) {
        adj[u].erase(it);
    }

    it = find(adjT[v].begin(), adjT[v].end(), u);
    if (it != adjT[v].end()) {
        adjT[v].erase(it);
    }
}

int main() {
    int listener;     // Listening socket descriptor
    struct sockaddr_in serveraddr;    // server address
    struct sockaddr_in clientaddr;    // client address
    socklen_t addrlen;
    char buf[1024];    // Buffer for client data

    // Create a new listener socket
    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Set the socket option to reuse the address
    int yes = 1;
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    // Set up the server address struct
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);
    memset(&(serveraddr.sin_zero), '\0', 8);

    // Bind the listener socket to our port
    if (bind(listener, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
        perror("bind");
        exit(1);
    }

    // Start listening for incoming connections
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(1);
    }

    printf("Server is running on port %d\n", PORT);

    while (1) {
        addrlen = sizeof(clientaddr);
        int newfd = accept(listener, (struct sockaddr *)&clientaddr, &addrlen);

        if (newfd == -1) {
            perror("accept");
            continue;
        }

        printf("New connection from %s on socket %d\n", inet_ntoa(clientaddr.sin_addr), newfd);

        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client, (void *)(intptr_t)newfd) != 0) {
            perror("pthread_create");
            close(newfd);
        }
    }

    return 0;
}

void *handle_client(void *arg) {
    int client_fd = (intptr_t)arg;
    char buf[1024];
    int numbytes;

    while ((numbytes = recv(client_fd, buf, sizeof(buf) - 1, 0)) > 0) {
        buf[numbytes] = '\0';
        string command(buf);
        stringstream ss(command);
        string cmd;
        ss >> cmd;

        if (cmd == "Newgraph") {
            int numVertices, numEdges;
            ss >> numVertices >> numEdges;
            Newgraph(numVertices, numEdges);

            // Inform the client to send the edges
            string msg = "Enter " + to_string(numEdges) + " edges:\n";
            send(client_fd, msg.c_str(), msg.size(), 0);

            for (int i = 0; i < numEdges; ++i) {
                if ((numbytes = recv(client_fd, buf, sizeof(buf) - 1, 0)) > 0) {
                    buf[numbytes] = '\0';
                    stringstream edge_ss(buf);
                    int u, v;
                    edge_ss >> u >> v;
                    Newedge(u, v);
                }
            }

            msg = "Graph created with " + to_string(numVertices) + " vertices and " + to_string(numEdges) + " edges.\n";
            send(client_fd, msg.c_str(), msg.size(), 0);
        } else if (cmd == "Kosaraju") {
            Kosaraju(client_fd);
        } else if (cmd == "Newedge") {
            int u, v;
            ss >> u >> v;
            Newedge(u, v);
            string msg = "Edge " + to_string(u) + " " + to_string(v) + " added.\n";
            send(client_fd, msg.c_str(), msg.size(), 0);
        } else if (cmd == "Removeedge") {
            int u, v;
            ss >> u >> v;
            Removeedge(u, v);
            string msg = "Edge " + to_string(u) + " " + to_string(v) + " removed.\n";
            send(client_fd, msg.c_str(), msg.size(), 0);
        } else {
            string msg = "Invalid command\n";
            send(client_fd, msg.c_str(), msg.size(), 0);
        }
    }

    close(client_fd);
    return NULL;
}