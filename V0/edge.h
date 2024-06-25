#ifndef __EDGE_H__
#define __EDGE_H__


#include <unordered_map>
#include <iostream>
#include <vector>


#include "graph.h"


class Edge {
    public:
        long long edgeCount, linkEdgeCount;

        Edge();
        ~Edge();

        void stat(const DiGraph&);
        void stat(const BiedgedGraph&);
        void print2File(const std::string&);

    private:
        int loopCount;
        std::unordered_map <long long, int> loopLen;
        std::vector <int> inDegree, outDegree, degree;
        std::vector <int> deadEndList;
};


class Cycle {
    public:
        int cycleCount;
        long long minCycleLen;
        std::unordered_map <long long, int> cycleLen;

        Cycle();
        ~Cycle();

        void statCycle(const DiGraph&);
        void print2File(const std::string&);
};

#endif