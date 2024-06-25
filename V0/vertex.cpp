#include "vertex.h"


Vertex::Vertex() {
    totalLen = 0;
    N50 = 0;
    L50 = 0;
}


Vertex::~Vertex() {}


void Vertex::calN50(std::vector<int>& vec) {
    long long sum = 0;
    std::sort(vec.begin(), vec.end(), std::greater <int> () );

    for (auto val: vec) {
        sum += val;
        L50 ++;
        if ((sum << 1) >= totalLen) {
            N50 = val;
            break;;
        }
    }
}


void Vertex::statVertex(const DiGraph& diGraph) {
    std::vector<int> tmp; 
    for (auto v: diGraph.vertexVal) {
        totalLen += v.second;
        tmp.push_back(v.second);
    }
    calN50(tmp);
}


void Vertex::statVertex(const BiedgedGraph& biedgedGraph) {
    std::vector<int> tmp; 
    for (int vID = 1; vID < biedgedGraph.vertexNumber; vID += 2) {
        totalLen += biedgedGraph.edge[vID].begin() -> value;
        tmp.push_back(biedgedGraph.edge[vID].begin() -> value);
    }
    calN50(tmp);
}


void Vertex::print2File(const std::string& outputFile) {
    // T. B. C.
    std::cout << "Total Length:\t" << totalLen << " bp\n" 
        << "N50:\t" << N50 << " bp\n" 
        << "L50:\t" << L50 << " bp\n";
        
    // 输出度数分布和deadEndList
}