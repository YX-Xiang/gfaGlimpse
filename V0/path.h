#ifndef __PATH_H__
#define __PATH_H__


#include <cstdlib>
#include <iterator>
#include <sstream>

#include "graph.h"


class Coverage {
    public:
        const std::vector <std::vector <std::pair <int, char> > >& path;

        Coverage(const std::vector <std::vector <std::pair <int, char> > >&);
        ~Coverage();

        void statCoverage(const DiGraph&);
        void statCoverage(const BiedgedGraph&);
        void print2File(const std::string&, int);

    private:
        std::vector <int> vertexCoverage, edgeCoverage;
        std::vector <long long> bpCoverage;
};


class Growth {
    public:
        //const std::vector <std::vector <std::pair <int, char> > >& path;
        std::string outputPath;

        Growth(const std::string&);
        ~Growth();

        void statGrowth(const std::string&);
        // void statGrowth(const DiGraph&);
        // void statGrowth(const BiedgedGraph&);
        void print2File(const std::string&);

    /*
    private:
        // common (≥5% of all non-reference haplotypes) and core (≥95% of all non-reference haplotypes)
        std::vector <int> vertexGrowth, vertexCommonGrowth, vertexCoreGrowth;
        std::vector <int> edgeGrowth, edgeCommonGrowth, edgeCoreGrowth;
        std::vector <long long> bpGrowth, bpCommonGrowth, bpCoreGrowth;
    */
};

#endif