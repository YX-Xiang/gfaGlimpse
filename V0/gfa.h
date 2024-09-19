#ifndef __GFA_H__
#define __GFA_H__


#include <string>
#include <utility>
#include <unordered_map>
#include <fstream>
#include <ostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <chrono>

#include "graph.h"
#include "vertex.h"
#include "edge.h"
#include "path.h"
#include "connectivity.h"
#include "bubble.h"


class Gfa {
    public:
        size_t fileSize;
        int numThreads;
        long long segmentNumber, linkNumber;
        long long singleDirectionSegmentCount, biDirectionalSegmentCount;
        std::vector <std::vector <std::pair <long long, char> > > path;

        Gfa(int);
        ~Gfa();

        void gfa2Graph(const std::string&, const std::string&, DiGraph&, BiedgedGraph&, BiedgedGraph&);
        void printDigraphInfo(const std::string&, DiGraph&);
        void printBigraphInfo(const std::string&, BiedgedGraph&);
        void printDibigraphInfo(const std::string&, BiedgedGraph&);
};


#endif