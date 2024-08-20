#include "graph.h"
#include <iostream>


Graph::Graph() {
    vertexNumber = 0;
    vertexVal = std::map <long long, int>();
    map2RawGraph = std::map <int, int> ();
}


Graph::~Graph() {}


DiGraph::DiGraph() {
    edge = std::vector <std::set <long long> > ();
    inDegree = std::map <int, int> ();
    outDegree = std::map <int, int> ();
}


DiGraph::~DiGraph() {}


void DiGraph::print() {
    std::cout << "Edges in the digraph:\n";
    int vertexMaxNumber = edge.size();
    for(int bid = 1; bid < vertexMaxNumber; bid ++) {
        for(auto eid: edge[bid]) {
            std::cout << bid << ":" << vertexVal.at(bid) << " -> " 
                << eid << ":" << vertexVal.at(eid) << '\n';
        }
    }
}


Biedge::Biedge() {
    value = 0;
}


Biedge::Biedge(int nTo, int nValue) {
    to = nTo;
    value = nValue;
}


Biedge::~Biedge() {}


bool Biedge::operator<(const Biedge& other) const {
    if (value != other.value) {
        return value > other.value;
    }
    return to < other.to; 
}


BiedgedGraph::BiedgedGraph() {
    edge = std::vector <std::set <Biedge> > (1, std::set <Biedge>());
}


BiedgedGraph::~BiedgedGraph() {}


void BiedgedGraph::print() {
    std::cout << "Edges in the bigraph:\n";
    int vertexMaxNumber = edge.size();
    for(int bid = 1; bid < vertexMaxNumber; bid ++) {
        for(auto eid: edge[bid]) {
            std::cout << bid << " -> " << eid.to << ": " << eid.value << '\n';
        }
    }
}