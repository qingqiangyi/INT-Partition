#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace chrono;

class FatTree {
public:
    int k;
    vector<vector<int>> adj;
    vector<bool> visited;

    FatTree(int k) : k(k) {
        int num_switches = k * k * 5 / 4;  // Core + Aggregate + Edge switches
        adj.resize(num_switches);
        visited.resize(num_switches, false);
        buildTopology();
    }

    void buildTopology() {
        int core_start = k * k / 4;
        int agg_start = core_start + k * k / 2;
        int edge_start = agg_start + k * k / 2;

        // Connect core to aggregation
        for (int i = 0; i < k * k / 4; i++) {
            for (int j = 0; j < k / 2; j++) {
                int agg = agg_start + (i / (k / 2)) * (k / 2) + j;
                adj[i].push_back(agg);
                adj[agg].push_back(i);
            }
        }

        // Connect aggregation to edge
        for (int i = 0; i < k * k / 2; i++) {
            for (int j = 0; j < k / 2; j++) {
                int edge = edge_start + (i / (k / 2)) * (k / 2) + j;
                adj[agg_start + i].push_back(edge);
                adj[edge].push_back(agg_start + i);
            }
        }
    }

    void DFS(int node) {
        visited[node] = true;
        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                DFS(neighbor);
            }
        }
    }

    double runDFS() {
        fill(visited.begin(), visited.end(), false);
        auto start = high_resolution_clock::now();
        DFS(0);
        auto end = high_resolution_clock::now();
        return duration<double>(end - start).count();
    }
};

int main() {
    for (int k = 4; k <= 14; k += 2) {
        FatTree fatTree(k);
        double time = fatTree.runDFS();
        cout << "k = " << k << " Execution time: " << time << " s\n";
    }
    return 0;
}
