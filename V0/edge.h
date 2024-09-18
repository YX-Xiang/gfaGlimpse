#ifndef __EDGE_H__
#define __EDGE_H__


#include <unordered_map>
#include <iostream>
#include <utility>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <functional>
#include <atomic>


#include "graph.h"


class Edge {
    public:
        long long edgeCount, linkEdgeCount;
        int loopCount;
        std::unordered_map <long long, int> loopLen;

        Edge();
        ~Edge();

        void stat(const DiGraph&);
        void stat(const BiedgedGraph&);
        void print2File(const std::string&, int);
};


class Cycle {
    public:
        int numThreads;
        int cycleCount;
        long long minCycleLen;
        std::unordered_map <long long, int> cycleLen;

        Cycle();
        ~Cycle();

        void work(std::vector <DiGraph>&, std::unordered_map <long long, int>&);
        void work(std::vector <BiedgedGraph>&, std::unordered_map <long long, int>&);
        void print2File(const std::string&, int);

    private:
        std::mutex mtx;  // 互斥锁保护共享资源

        void statCycle(const DiGraph&);
        void statCycle(const BiedgedGraph&);
        void findCycleInDirected(int, const std::vector <int>&, const std::vector <std::set <int> >&);
        void findCycleInBiedged(int, const std::vector <int>&, const std::vector <std::set <std::pair<int, int> > >&);
};

#endif