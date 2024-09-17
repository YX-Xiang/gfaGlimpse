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
        int N50, L50, U50, deadEnd, startEnd;
        
        Vertex();
        ~Vertex();

        void statVertex(const DiGraph&);
        void statVertex(const BiedgedGraph&);
        void printInDegree2File(const std::string&);
        void printOutDegree2File(const std::string&);
        void printDegree2File(const std::string&);
        void printLinkDegree2File(const std::string&);
        void printDibiDegree2File(const std::string&);

    private:
        std::unordered_map <int, int> inDegreeDistribution, outDegreeDistribution;
        std::unordered_map <int, int> bidirectedDegreeDistribution, linkDegreeDistribution;
        std::unordered_map <int, int> segmentInDegreeDistribution, segmentOutDegreeDistribution, linkInDegreeDistribution, linkOutDegreeDistribution;

        std::pair<int, int> calN50(std::vector <int>&);
};


#endif