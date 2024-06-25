#include "edge.h"


Edge::Edge() {
    edgeCount = 0;
    linkEdgeCount = 0;
    loopCount = 0;
    loopLen = std::unordered_map <long long, int> ();
}


Edge::~Edge() {}


void Edge::stat(const DiGraph& diGraph) {
    int maxVertexNumber = diGraph.edge.size();
    for (int vID = 1; vID < maxVertexNumber; vID ++) {
        edgeCount += diGraph.edge[vID].size();
        if (diGraph.edge[vID].find(vID) != diGraph.edge[vID].end()) {
            loopCount ++;
            if (diGraph.vertexVal.count(vID)) {
                loopLen[diGraph.vertexVal.at(vID)] ++;
            } else {
                std::cerr << "There were some problems with counting the loops in the digraph !!!" << std::endl;
            }
        }
    }
}


void Edge::stat(const BiedgedGraph& biedgedGraph) {
    int maxVertexNumber = biedgedGraph.edge.size();
    for (int vID = 1; vID < maxVertexNumber; vID ++) {
        linkEdgeCount += biedgedGraph.edge[vID].size() - 1;
        
        Biedge tmp;
        tmp.value = 0;
        tmp.to = vID;
        if (biedgedGraph.edge[vID].find(tmp) != biedgedGraph.edge[vID].end()) {
            loopCount ++;
            loopLen[0] ++;
        }

        if (vID & 1) {
            tmp.to = vID + 1;
            if (biedgedGraph.edge[vID].find(tmp) != biedgedGraph.edge[vID].end()) {
                loopCount ++;
                if (biedgedGraph.edge[vID].begin() -> value) {
                    loopLen[biedgedGraph.edge[vID].begin() -> value] ++;
                } else {
                    std::cerr << "There were some problems with counting the loops in the biedged graph !!!" << std::endl;
                }
            }
        }
    }
    linkEdgeCount /= 2;
}


void Edge::print2File(const std::string& outputFile) {
    // T. B. C.
    std::cout << "Edges:\t" << std::max(edgeCount, linkEdgeCount) << "\n" 
        << "Loops:\t" << loopCount << "\n";
    for (auto loop: loopLen) {
        std::cout << loop.first << " " << loop.second << std::endl;
    }

    loopLen = std::unordered_map <long long, int>();
}


Cycle::Cycle() {
    cycleCount = 0;
    minCycleLen = 0x7FFFFFFFFFFFFFFF;
    cycleLen = std::unordered_map <long long, int> ();
}


Cycle::~Cycle() {}


void Cycle::statCycle(const DiGraph& diGraph) {
    // T. B. C.

}


void Cycle::print2File(const std::string&) {
    // T. B. C.
    if (minCycleLen == 0x7FFFFFFFFFFFFFFF) {
        minCycleLen = -1;
    }
    std::cout << "Cycles:\t" << cycleCount << "\n" 
        << "Minimum Cycle:\t" << minCycleLen << "\n";
    for (auto cycle: cycleLen) {
        std::cout << cycle.first << " " << cycle.second << std::endl;
    }
    cycleLen = std::unordered_map <long long, int> ();
}