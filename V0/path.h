#ifndef __PATH_H__
#define __PATH_H__


#include "gfa.h"
#include "graph.h"


class Coverage {
    public:
        const std::vector <std::pair <int, char> >& path;

        Coverage(const std::vector <std::pair <int, char> >&);
        ~Coverage();

        void statCoverage(const DiGraph&);
        void statCoverage(const BiedgedGraph&);
        void print2File(const std::string&);

    private:
        std::vector <int> vertexCoverage, edgeCoverage;
        std::vector <long long> bpCoverage;
};


class Growth {
    public:
        const std::vector <std::pair <int, char> >& path;

        Growth(const std::vector <std::pair <int, char> >&);
        ~Growth();

        void statGrowth(const DiGraph&);
        void statGrowth(const BiedgedGraph&);
        void print2File(const std::string&);

    private:
        std::vector <int> vertexGrowth, vertexCommonGrowth, vertexCoreGrowth;
        std::vector <int> edgeGrowth, edgeCommonGrowth, edgeCoreGrowth;
        std::vector <long long> bpGrowth, bpCommonGrowth, bpCoreGrowth;
};

#endif