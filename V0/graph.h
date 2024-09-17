#ifndef __GRAPH_H__
#define __GRAPH_H__


#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>


class Graph {
    public:
        long long vertexNumber;
        std::map <long long, int> vertexVal;
        std::map <int, int> map2RawGraph;

        Graph();
        ~Graph();
};


class DiGraph: public Graph {
    public:
        std::vector <std::set <long long> > edge;
        std::map <int, int> inDegree, outDegree;

        DiGraph();
        ~DiGraph();

        void vertexvalStat(const std::string&);
        void print();
};


class Biedge {
    public:
        int to, value;

        Biedge();
        Biedge(int, int);
        ~Biedge();

        bool operator<(const Biedge&) const;
};


class BiedgedGraph: public Graph {
    public:
        std::vector <std::set <Biedge> > edge;
        std::vector <std::set <Biedge> > redge; //reverse edge
        // 区分biedged和directed biedged
        bool directed;
        
        BiedgedGraph();
        ~BiedgedGraph();
        void edgevalStat(const std::string&);
        void print();
};

#endif
