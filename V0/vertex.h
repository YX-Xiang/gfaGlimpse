#ifndef __VERTEX_H__
#define __VERTEX_H__


#include <iostream>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>


#include "graph.h"


class Vertex {
    public:
        long long totalLen;
        
        Vertex();
        ~Vertex();

        void statVertex(const DiGraph&);
        void statVertex(const BiedgedGraph&);
        void print2File(const std::string&);

    private:
        int N50, L50, U50;
        std::unordered_map <int, int> inDegreeDistribution, outDegreeDistribution;
        std::unordered_map <int, int> bidirectedDegreeDistribution, linkDegreeDistribution;
        std::unordered_map <int, int> segmentInDegreeDistribution, segmentOutDegreeDistribution, linkInDegreeDistribution, linkOutDegreeDistribution;

        int calN50(std::vector <int>&);
};


#endif