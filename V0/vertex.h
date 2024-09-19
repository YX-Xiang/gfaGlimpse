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
        long long N50, L50, U50;
        int deadEnd, startEnd;
        
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
        std::unordered_map <long long, long long> inDegreeDistribution, outDegreeDistribution;
        std::unordered_map <long long, long long> bidirectedDegreeDistribution, linkDegreeDistribution;
        std::unordered_map <long long, long long> segmentInDegreeDistribution, segmentOutDegreeDistribution, linkInDegreeDistribution, linkOutDegreeDistribution;

        std::pair<long long, long long> calN50(std::vector <long long>&, long long);
};


#endif