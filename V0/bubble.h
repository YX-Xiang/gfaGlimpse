#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include <algorithm>
#include <queue>
#include <unordered_map>

#include "graph.h"

class Bubble {
public:
    long long simpleBubbleCount, superBubbleCount;
    double seqCoverage, nodeCoverage;
    long long maxChainBp, maxChainBubbleCount;

    Bubble();
    ~Bubble();

    void findBubble(const BiedgedGraph &, int);
    void print2File(const std::string &);

private:
    struct bubble {
        long long s, t;
        int type; // 0 simple 1 super 2 insertion
        std::vector<long long> inside;
        bool operator<(const bubble &ts) const { return s < ts.s; }
    };
    std::vector <std::vector<long long> > supperBubble, simpleBubble, insertion;
    std::unordered_map<long long, long long> chainLength;
    std::vector <bubble> Bubbles;
    std::map<long long, long long> bubble_siz;

    long long id(long long, int);
    bool is_simple(int, std::vector<long long>&, const BiedgedGraph&, int);
    void bfs(long long, int, const BiedgedGraph &, int);
};

#endif