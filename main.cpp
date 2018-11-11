#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

#define INFINITE 100
using namespace std;

struct Graph {
    const int vexs;
    int **g;

    Graph(int vexs) : vexs(vexs) {
        g = new int *[this->vexs + 1];
        for (int i = 0; i <= this->vexs; i++) {
            g[i] = new int[vexs + 1];
        }
    }

    ~Graph() {
        cout << "Delete Graph" << endl;
        for (int i = 0; i <= this->vexs; i++) {
            delete[] g[i];
        }
        delete[] g;
    }

    bool hasEdge(int i, int j) {
        return i != j && g[i][j] < INFINITE;
    }
};

class Solution {
    Graph *graph;
    int ***D; // D[k][i][j]: i, j两点在所有中间节点都取自1~k时, i与j的最短距离
    int ***paths; // 前驱矩阵 paths[k][i][j]: i, j两点在所有中间节点都取自1~k时, j的前驱结点
public:
    Solution(Graph *g) {
        this->graph = g;
        this->D = new int **[g->vexs + 1];
        this->paths = new int **[g->vexs + 1];
    }

    void solveByFloydWarshall() {
        D[0] = graph->g;
        paths[0] = new int *[graph->vexs + 1];
        for (int i = 0; i <= graph->vexs; i++) {
            paths[0][i] = new int[graph->vexs];
            for (int j = 0; j <= graph->vexs; j++) {
                paths[0][i][j] = graph->hasEdge(i, j) ? i : -1;
            }
        }
        int t1 = 0, t2 = 0;
        for (int k = 1; k <= this->graph->vexs; k++) {

            D[k] = new int *[graph->vexs + 1];
            paths[k] = new int *[graph->vexs + 1];

            for (int i = 1; i <= graph->vexs; i++) {

                D[k][i] = new int[graph->vexs + 1];
                paths[k][i] = new int[graph->vexs + 1];

                for (int j = 1; j <= graph->vexs; j++) {
                    t1 = D[k - 1][i][j];
                    t2 = D[k - 1][i][k] + D[k - 1][k][j];
                    if (t1 <= t2) {
                        if (t1 < 32767) {
                            D[k][i][j] = t1;
                            paths[k][i][j] = paths[k - 1][i][j];
                        } else {
                            D[k][i][j] = 32767;
                            paths[k][i][j] = -1;
                        }
                    } else {
                        if (t2 < 32767) {
                            D[k][i][j] = t2;
                            paths[k][i][j] = paths[k - 1][k][j];
                        } else {
                            D[k][i][j] = 32767;
                            paths[k][i][j] = -1;
                        }
                    }
                }
            }
        }
    }

    void showPath(int i, int j) {
        cout << "i " << i << " j " << j << ": " << D[graph->vexs][i][j] << "\t" << j;
        if (paths[graph->vexs][i][j] < 0) {
            cout << " <- no way" << j << endl;
            return;
        }
        int lastVex = i;
        for (int k = graph->vexs - 1; k > 0; k--) {
            if (D[k][i][j] <= D[k][i][k + 1] + D[k][k + 1][j]) {
                if (paths[k][i][j] != lastVex) {
                    lastVex = paths[k][i][j];
                    cout << " <- " << paths[k][i][j];
                }
            } else {
                if (paths[k][k + 1][j] != lastVex) {
                    lastVex = paths[k][k + 1][j];
                    cout << " <- " << paths[k][k + 1][j];
                }
            }

        }
        cout << " <- " << i << endl;

    }

    ~Solution() {
        for (int i = 0; i <= graph->vexs; i++) {
            for (int j = 0; j <= graph->vexs; j++) {
                delete[] D[i][j];
                delete[] paths[i][j];
            }
            delete[] D[i];
            delete[] paths[i];
        }
        delete[] D;
        delete[] paths;
    }

};

int main() {
    int data[5][5] = {{0,   3,   8,   100, -4},
                      {100, 0,   100, 1,   7},
                      {100, 4,   0,   100, 100},
                      {2,   100, -5,  0,   100},
                      {100, 100, 100, 6,   0}};
    Graph graph(5);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (data[i][j] < 100)
                graph.g[i + 1][j + 1] = data[i][j];
            else graph.g[i + 1][j + 1] = 32767;
        }
    }
    Solution s(&graph);
    s.solveByFloydWarshall();
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            s.showPath(i + 1, j + 1);
        }
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}