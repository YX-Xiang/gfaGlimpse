#ifndef __CONNECTIVITY_H__
#define __CONNECTIVITY_H__


#include <cstdint>
#include <set>
#include <stack>
#include <unordered_map>
#include <vector>


#include "graph.h"


class Connectivity {
    public:
        int ccCount, sccNumber, nodeNumber, bridgeCount, cutPointCount, sccCount, wccCount;
        std::vector < std::vector <long long> > sccList;

        Connectivity(long long);
        ~Connectivity();

        void edgeCompress(const DiGraph&);
        void findComponent(const BiedgedGraph&);
        void findWCC(const DiGraph&);
        void findSCC(const DiGraph&);
        void SCC2Subgraph(const DiGraph&, std::vector <DiGraph>&);
        void SCC2Subgraph(const BiedgedGraph&, std::vector <BiedgedGraph>&);
        void print2File(const std::string&);

    private:
        std::vector < std::set <long long> > graph;
        std::vector <long long> vertexMap, reVertexMap;
        long long dfsNumber;
        std::vector <bool> isCut;
        std::set <std::pair<long long, long long>> bridge;
        std::vector <long long> visNum, low, sccNum;
        std::stack <long long> scc;

        void initGraph();
        void tarjan(long long);
};

#endif