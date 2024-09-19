#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include <algorithm>
#include <queue>
#include <unordered_map>

#include "graph.h"

class Bubble {
public:
    int simpleBubbleCount, superBubbleCount;
    double seqCoverage, nodeCoverage;
    long long maxChainBp, maxChainBubbleCount;

    Bubble();
    ~Bubble();

    void bfs(int, int, const BiedgedGraph &, int);
    void findBubble(const BiedgedGraph &, int);
    void print2File(const std::string &);

private:
    struct bubble {
        int s, t;
        int type; // 0 simple 1 super 2 insertion
        std::vector<int> inside;
        bool operator<(const bubble &ts) const { return s < ts.s; }
    };
    std::vector <std::vector<int> > supperBubble, simpleBubble, insertion;
    std::unordered_map<long long, int> chainLength;
    std::vector <bubble> Bubbles;
    std::map<int, int> bubble_siz;
};

#endif