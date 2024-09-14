#include <cstring>
#include <ostream>

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
		<< "\t-o,\t\tPath to output file (containing temporary file)\n"
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
	std::string outputPath = "";
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
				outputPath = argv[++i];
			} else {
				std::cerr << "-o option requires one argument." << std::endl;
				return 1;
			}  
		} else {
			inputGfaFile = argv[i];
		}
    }

    DiGraph diGraph;
    BiedgedGraph biedgedGraph, diBiedgedGraph;
    Gfa gfa;
    gfa.gfa2Graph(inputGfaFile, diGraph, biedgedGraph, diBiedgedGraph);
	// diGraph.print();
	// biedgedGraph.print();
	// diBiedgedGraph.print();

	Vertex diVertex, biVertex, diBiVertex;
	diVertex.statVertex(diGraph);
	biVertex.statVertex(biedgedGraph);
	diBiVertex.statVertex(diBiedgedGraph);
	// diVertex.print2File(outputPath);
	// biVertex.print2File(outputPath);
	// diBiVertex.print2File(outputPath);

	Edge diEdge, biEdge, diBiEdge;
	diEdge.stat(diGraph);
	biEdge.stat(biedgedGraph);
	diBiEdge.stat(diBiedgedGraph);
	// diEdge.print2File(outputPath);
	// biEdge.print2File(outputPath);
	// diBiEdge.print2File(outputPath);


	Connectivity diConnectivity = Connectivity(diGraph.vertexNumber);
	std::vector <DiGraph> diSubgraphList = std::vector <DiGraph>();
	diConnectivity.edgeCompress(diGraph);
	diConnectivity.findSCC(diGraph);
	diConnectivity.SCC2Subgraph(diGraph, diSubgraphList);

	// 只需要在上一步分离出的SCC中统计cycle相关指标，大大降低时间复杂度
	Cycle diTotal;
	diTotal.work(diSubgraphList);
	diTotal.print2File(outputPath);
	
	Connectivity diBiConnectivity = Connectivity(diBiedgedGraph.vertexNumber);
	std::vector <BiedgedGraph> diBiSubgraphList = std::vector <BiedgedGraph>();
	// diBiConnectivity.findComponent(diBiedgedGraph);
	// diBiConnectivity.SCC2Subgraph(diBiedgedGraph, diBiSubgraphList);

	Cycle diBiTotal;
	// diBiTotal.work(diBiSubgraphList);
	// diBiTotal.print2File(outputPath);

	Coverage diCoverage = Coverage(gfa.path);
	Coverage biCoverage = Coverage(gfa.path);
	// diCoverage.statCoverage(diGraph);
	// biCoverage.statCoverage(biedgedGraph);

	// ···TODO··· 先暂时放弃了，当个快乐的调包侠o(*￣▽￣*)ブ
	// Growth diGrowth = Growth(gfa.path);
	// Growth biGrowth = Growth(gfa.path);
	// diGrowth.statGrowth(diGraph);
	// biGrowth.statGrowth(biedgedGraph);
	Growth growth = Growth(outputPath);
	// growth.statGrowth(inputGfaFile);

	Bubble diBubble, biBubble, dibiBubble;
	// biBubble.findBubble(biedgedGraph);
	// dibiBubble.findBubble(diBiedgedGraph,1);
    return 0;
}
