#include "bubble.h"


Bubble::Bubble() {
    snarl = std::vector < std::vector <int> > ();
    supperBubble = std::vector < std::vector <int> > ();
    simpleBubble = std::vector < std::vector <int> > ();
    allele = std::unordered_map <long long, int> ();
    chainLen = std::unordered_map <long long, int> ();
    nestedBubble = std::unordered_map <int, int> ();
}


Bubble::~Bubble() {}


void Bubble::statSnarl(const BiedgedGraph& biedgedGraph) {
    // 统计snarl和allele
    // T. B. C.
}


void Bubble::findBubble(const DiGraph& diGraph) {
    // 统计supperBubble，simpleBubble，nestedBubble和chainLen
    // T. B. C.
}


void Bubble::findBubble(const BiedgedGraph& biedgedGraph) {
    // 统计supperBubble，simpleBubble，nestedBubble和chainLen
    // T. B. C.
    statSnarl(biedgedGraph);
}


void Bubble::print2File(const std::string& outputFile) {
    // T. B. C.
}