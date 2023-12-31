#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

// 定義邊的結構
struct Edge {
    int startNode;
    int endNode;
    int bandwidth;
    int cost;
};

// 生成隨機數字
int getRandomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// 生成 Graph 測資
void generateGraphTestData(int numNodes, int totalBandwidth, vector<Edge>& edges) {
    // 清空原有的邊
    edges.clear();

    for (int i = 1; i <= numNodes; i++) {
        for (int j = i + 1; j <= numNodes; j++) {
            Edge edge;
            edge.startNode = i;
            edge.endNode = j;
            edge.bandwidth = getRandomNumber(1, 100); 
            edge.cost = getRandomNumber(1, 100); 

            edges.push_back(edge);
        }
    }
}

// 寫入測資到檔案
void writeGraphTestDataToFile(const char* filename, int numNodes, int totalBandwidth, const vector<Edge>& edges) {
    ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    // 寫入節點數量和總頻寬
    outputFile << numNodes << " " << totalBandwidth << endl;

    // 寫入邊的資訊
    for (const Edge& edge : edges) {
        outputFile << edge.startNode << " " << edge.endNode << " " << edge.bandwidth << " " << edge.cost << endl;
    }

    outputFile.close();
}

int main() {
    // 設定亂數種子
    srand(static_cast<unsigned int>(time(0)));

    // 生成測資
    int numNodes = 100;   // 節點數量範圍為 1 到 100
    int t = getRandomNumber(1, 100); // 總頻寬範圍為 1 到 100

    vector<Edge> edges;
    generateGraphTestData(numNodes, t, edges);

    // 寫入檔案
    const char* filename = "graph_test_data.txt";
    writeGraphTestDataToFile(filename, numNodes, t, edges);

    cout << "Graph test data generated and saved to " << filename << endl;

    return 0;
}
