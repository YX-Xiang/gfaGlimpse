#include "path.h"


Coverage::Coverage(const std::vector <std::pair <int, char> >& gfaPath): 
    path(gfaPath) {
        vertexCoverage = std::vector <int> (path.size() + 1, 0);
        edgeCoverage = std::vector <int> (path.size() + 1, 0);
        bpCoverage = std::vector <long long> (path.size() + 1, 0);
}


Coverage::~Coverage() {}


void Coverage::statCoverage(const DiGraph& diGraph) {
    // T. B. C.
}


void Coverage::statCoverage(const BiedgedGraph& biedgedGraph) {
    // T. B. C.
}


void Coverage::print2File(const std::string&) {
    // T. B. C.
}


Growth::Growth(const std::vector <std::pair <int, char> >& gfaPath): 
    path(gfaPath) {
        vertexGrowth = std::vector <int> (path.size() + 1, 0);
        vertexCommonGrowth = std::vector <int> (path.size() + 1, 0);
        vertexCoreGrowth = std::vector <int> (path.size() + 1, 0);
        edgeGrowth = std::vector <int> (path.size() + 1, 0);
        edgeCommonGrowth = std::vector <int> (path.size() + 1, 0);
        edgeCoreGrowth = std::vector <int> (path.size() + 1, 0);
        bpGrowth = std::vector <long long> (path.size() + 1, 0);
        bpCommonGrowth = std::vector <long long> (path.size() + 1, 0);
        bpCoreGrowth = std::vector <long long> (path.size() + 1, 0);
}


Growth::~Growth() {}


void Growth::statGrowth(const DiGraph& diGraph) {
    // T. B. C.
}


void Growth::statGrowth(const BiedgedGraph& biedgedGraph) {
    // T. B. C.
}


void Growth::print2File(const std::string&) {
    // T. B. C.
}