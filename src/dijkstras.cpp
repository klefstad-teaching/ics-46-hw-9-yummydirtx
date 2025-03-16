#include "dijkstras.h"
#include <algorithm>

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous) {
    int n = G.numVertices;
    vector<int> distance(n, INF);
    vector<bool> visited(n, false);
    previous.resize(n, -1);
    
    // Using a min priority queue (distance, vertex)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    // Initialize source
    distance[source] = 0;
    pq.push({0, source});
    
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        
        if (visited[u]) continue;
        
        visited[u] = true;
        
        // Process all adjacent edges
        for (const Edge& edge : G[u]) {
            int v = edge.dst;
            int weight = edge.weight;
            
            // Relaxation step
            if (!visited[v] && distance[u] != INF && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                previous[v] = u;
                pq.push({distance[v], v});
            }
        }
    }
    
    return distance;
}

vector<int> extract_shortest_path(const vector<int>& distances, const vector<int>& previous, int destination) {
    vector<int> path;
    
    // Check if destination is reachable
    if (distances[destination] == INF) {
        return path;  // Return empty path if unreachable
    }
    
    // Backtrack from destination to source using previous array
    for (int vertex = destination; vertex != -1; vertex = previous[vertex]) {
        path.push_back(vertex);
    }
    
    // Reverse to get path from source to destination
    reverse(path.begin(), path.end());
    
    return path;
}

void print_path(const vector<int>& path, int total) {
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i] << " ";
    }
    cout << endl << "Total cost is " << total << endl;
}
