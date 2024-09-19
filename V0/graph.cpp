#include "graph.h"


Graph::Graph() {
    vertexNumber = 0;
    vertexVal = std::map <long long, long long>();
    map2RawGraph = std::map <long long, long long> ();
}


Graph::~Graph() {}


DiGraph::DiGraph() {
    edge = std::vector <std::set <long long> > ();
    inDegree = std::map <long long, long long> ();
    outDegree = std::map <long long, long long> ();
}


DiGraph::~DiGraph() {}


void DiGraph::vertexvalStat(const std::string& outputFile) {
    std::ofstream output(outputFile);
    if (!output.is_open()) {
        std::cerr << "Error opening file: " << outputFile << std::endl;
        return;
    }
    
    std::unordered_map <long long, long long> vertexvalMap = std::unordered_map <long long, long long> ();
    for (auto v: vertexVal) {
        if (v.second) {
            vertexvalMap[v.second] ++;
        }
    }
    
    // 输出点权分布（每一行第一个数字代表具体点权，第二个数字代表有几个点刚好是这个点权）
    for (auto mapElement: vertexvalMap) {
        output << mapElement.first << "\t" << mapElement.second << "\n";
    }
    std::cerr << "--- The node size of the digraph has been successfully exported to the file: " << outputFile << std::endl;
    output.close();
}


void DiGraph::print() {
    std::cout << "Edges in the digraph:\n";
    long long vertexMaxNumber = edge.size();
    for(long long bid = 1; bid < vertexMaxNumber; bid ++) {
        for(auto eid: edge[bid]) {
            std::cout << bid << ":" << vertexVal.at(bid) << " -> " 
                << eid << ":" << vertexVal.at(eid) << '\n';
        }
    }
}


Biedge::Biedge() {
    value = 0;
}


Biedge::Biedge(long long nTo, long long nValue) {
    to = nTo;
    value = nValue;
}


Biedge::~Biedge() {}


bool Biedge::operator<(const Biedge& other) const {
    if (value != other.value) {
        return value > other.value;
    }
    return to < other.to; 
}


BiedgedGraph::BiedgedGraph() {
    edge = std::vector <std::set <Biedge> > (1, std::set <Biedge>());
}


BiedgedGraph::~BiedgedGraph() {}


void BiedgedGraph::edgevalStat(const std::string& outputFile) {
    std::ofstream output(outputFile);
    if (!output.is_open()) {
        std::cerr << "Error opening file: " << outputFile << std::endl;
        return;
    }
    
    std::unordered_map <long long, long long> edgevalMap = std::unordered_map <long long, long long> ();
    for (auto v: edge) {
        for (auto e: v) {
            if (e.value) {
                edgevalMap[e.value] ++;
            }
        }
    }
    
    // 输出点权分布（每一行第一个数字代表具体点权，第二个数字代表有几个点刚好是这个点权）
    for (auto mapElement: edgevalMap) {
        output << mapElement.first << "\t" << (mapElement.second >> 1) << "\n";
    }
    std::cerr << "--- The node size of the bidirected graph has been successfully exported to the file: " << outputFile << std::endl;
    output.close();
}


void BiedgedGraph::print() {
    std::cout << "Edges in the bigraph:\n";
    long long vertexMaxNumber = edge.size();
    for(long long bid = 1; bid < vertexMaxNumber; bid ++) {
        for(auto eid: edge[bid]) {
            std::cout << bid << " -> " << eid.to << ": " << eid.value << '\n';
        }
    }
}