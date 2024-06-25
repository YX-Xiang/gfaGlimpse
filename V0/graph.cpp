#include "graph.h"


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


Biedge::Biedge() {
    value = 0;
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