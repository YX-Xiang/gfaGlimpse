#ifndef __BUBBLE_H__
#define __BUBBLE_H__


#include <unordered_map>


#include "graph.h"


class Bubble {
    public:
        Bubble();
        ~Bubble();

        void statSnarl(const BiedgedGraph&);
        void findBubble(const DiGraph&);
        void findBubble(const BiedgedGraph&);
        void print2File(const std::string&);

    private:
        std::vector < std::vector <int> > snarl, supperBubble, simpleBubble;
        std::unordered_map <long long, int> allele, chainLen;
        std::unordered_map <int, int> nestedBubble;
};

#endif