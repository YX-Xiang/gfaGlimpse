#ifndef __GRAPH_H__
#define __GRAPH_H__


#include <map>
#include <vector>
#include <set>


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
};


class Biedge {
    public:
        int to, value;

        Biedge();
        ~Biedge();

        bool operator<(const Biedge&) const;
};


class BiedgedGraph: public Graph {
    public:
        std::vector <std::set <Biedge> > edge;
        // ！！！后期需要加度数，注意区分bidirected和biedged

        BiedgedGraph();
        ~BiedgedGraph();
};

#endif