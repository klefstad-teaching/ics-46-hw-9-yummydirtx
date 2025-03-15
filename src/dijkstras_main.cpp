#include "dijkstras.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    // Read graph from file
    Graph G;
    try {
        file_to_graph(argv[1], G);
    } catch (const exception &ex) {
        cerr << ex.what() << endl;
        return 1;
    }

    // Run Dijkstra from source = 0
    vector<int> previous(G.numVertices, -1);
    vector<int> dist = dijkstra_shortest_path(G, 0, previous);

    // For each vertex, extract and print the path and cost
    for (int v = 0; v < G.numVertices; v++) {
        vector<int> path = extract_shortest_path(dist, previous, v);
        print_path(path, dist[v]);
    }

    return 0;
}