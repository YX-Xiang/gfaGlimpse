#include <algorithm>
#include <cstring>
#include <iostream>

#include "graph.h"
#include "gfa.h"
#include "vertex.h"
#include "edge.h"
#include "connectivity.h"
#include "path.h"
#include "bubble.h"


void showUsage(std::string name) {
	std::cerr << "Usage: " << name << " <option(s)> input_gfa_file\n"
		<< "Options:\n"
		<< "\t-h,\t\tShow this help message\n"
		<< "\t-m,\t\tChoose the different mode (cycle(default)/path/scc/dominator/check)"
		<< "\t-t,\t\tThe number of threads (default: 1)\n"
		<< "\t-o,\t\tPath to output file (containing strongly connected component point distributions and cycle length distributions)\n"
		<< "\t-f,\t\tFasta files to be compared\n"
		<< std::endl;
}


int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);
	std::cout.tie(NULL);
	std::cerr.tie(NULL);

    if(argc < 1) {
		showUsage(argv[0]);
		return 1;
	}

    std::string inputThread = "";
    std::string inputMode = "cycle";
    std::string inputGfaFile = "";
	std::string outputFile = "";
    int threads = 1;

    for (int i = 1; i < argc; ++ i) {
        if(strcmp(argv[i], "-h") == 0) {
			showUsage(argv[0]);
			return 0;
		} else if (strcmp(argv[i], "-m") == 0) {
			if (i + 1 < argc) {
				inputMode = argv[++i];
				if (inputMode != "cycle" && inputMode != "path" && inputMode != "scc" && inputMode != "dominator" && inputMode != "check") {
					std::cerr << "Errors concerning the argument of the -m option." << std::endl;
					return 1;
				}
			} else {
				std::cerr << "-m option requires one argument." << std::endl;
				return 1;
			}  
		} else if (strcmp(argv[i], "-t") == 0) {
			if (i + 1 < argc) {
				inputThread = argv[++i];
				threads = std::stoi(inputThread);
			} else {
				std::cerr << "-t option requires one argument." << std::endl;
				return 1;
			}  
		} else if (strcmp(argv[i], "-o") == 0) {
			if (i + 1 < argc) {
				outputFile = argv[++i];
			} else {
				std::cerr << "-o option requires one argument." << std::endl;
				return 1;
			}  
		} else {
			inputGfaFile = argv[i];
		}
    }

    DiGraph diGraph;
    BiedgedGraph biedgedGraph;
    Gfa gfa;
    gfa.gfa2Graph(inputGfaFile, diGraph, biedgedGraph);

	Vertex diVertex, biVertex;
	diVertex.statVertex(diGraph);
	biVertex.statVertex(biedgedGraph);
	diVertex.print2File(outputFile);
	biVertex.print2File(outputFile);

	Edge diEdge, biEdge;
	diEdge.stat(diGraph);
	biEdge.stat(biedgedGraph);
	// diEdge.print2File(outputFile);
	// biEdge.print2File(outputFile);

	Connectivity diConnectivity = Connectivity(diGraph.vertexNumber);
	diConnectivity.edgeCompress(diGraph);
	diConnectivity.findSCC(diGraph);

	std::vector <DiGraph> subgraphList = std::vector <DiGraph>();
	diConnectivity.SCC2Subgraph(diGraph, subgraphList);
	/*for (auto subG: subgraphList) {
		for (auto v: subG.vertexVal) {
			std::cout << "Vertexs:\t";
			std::cout << v.first << ": " << v.second << std::endl;
			std::cout << "Edges:\t";
			for (auto e: subG.edge[v.first]) {
				std::cout << v.first << "->" <<  e << "; ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}*/


	// 只需要在上一步分离出的SCC中统计cycle相关指标，大大降低时间复杂度
	Cycle total;
	for (auto subgraph: subgraphList) {
		Cycle diCycle;
		diCycle.statCycle(subgraph);
		total.cycleCount += diCycle.cycleCount;
		if (diCycle.cycleCount) {
			total.minCycleLen = std::min(total.minCycleLen, diCycle.minCycleLen);
		}
		for (auto item: diCycle.cycleLen) {
			total.cycleLen[item.first] += item.second;
		}
	}
	// total.print2File(outputFile);

	Coverage diCoverage = Coverage(gfa.path);
	Coverage biCoverage = Coverage(gfa.path);
	diCoverage.statCoverage(diGraph);
	biCoverage.statCoverage(biedgedGraph);

	Growth diGrowth = Growth(gfa.path);
	Growth biGrowth = Growth(gfa.path);
	diGrowth.statGrowth(diGraph);
	biGrowth.statGrowth(biedgedGraph);

	Bubble diBubble, biBubble;
	diBubble.findBubble(diGraph);
	biBubble.findBubble(biedgedGraph);

    return 0;
}