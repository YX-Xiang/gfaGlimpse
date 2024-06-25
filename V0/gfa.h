#ifndef __GFA_H__
#define __GFA_H__


#include <string>
#include <utility>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "graph.h"


class Gfa {
    public:
        size_t fileSize;
        int segmentNumber, linkNumber;
        std::vector <std::pair <int, char> > path;

        Gfa();
        ~Gfa();

        void gfa2Graph(const std::string&, DiGraph&, BiedgedGraph&);
};


#endif