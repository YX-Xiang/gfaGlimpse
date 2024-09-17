#ifndef __CONNECTIVITY_H__
#define __CONNECTIVITY_H__


#include <set>
#include <stack>
#include <unordered_map>
#include <vector>


#include "graph.h"


class Connectivity {
    public:
        int sccNumber, nodeNumber, bridgeCount, cutPointCount, sccCount, wccCount;
        std::vector < std::vector <int> > sccList;

        Connectivity(int);
        ~Connectivity();

        void edgeCompress(const DiGraph&);
        void findComponent(const BiedgedGraph&);
        void findWCC(const DiGraph&);
        void findSCC(const DiGraph&);
        void SCC2Subgraph(const DiGraph&, std::vector <DiGraph>&);
        void SCC2Subgraph(const BiedgedGraph&, std::vector <BiedgedGraph>&);
        void print2File(const std::string&);

    private:
        std::vector < std::set <int> > graph;
        std::vector <long long> vertexMap, reVertexMap;
        int dfsNumber;
        std::vector <bool> isCut;
        std::set <std::pair<int, int>> bridge;
        std::vector <long long> visNum, low, sccNum;
        std::stack <long long> scc;

        void initGraph();
        void tarjan(int, int);
};

#endif