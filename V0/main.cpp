#include <cstring>
#include <ostream>

#include "graph.h"
#include "gfa.h"
#include "path.h"


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
	std::string outputFolderPath = "";
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
				outputFolderPath = argv[++i];
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
    Gfa gfa(threads);
    gfa.gfa2Graph(inputGfaFile, outputFolderPath, diGraph, biedgedGraph, diBiedgedGraph);
	// diGraph.print();
	// biedgedGraph.print();
	// diBiedgedGraph.print();

	Growth growth = Growth(outputFolderPath);
	growth.numThreads = threads;
	growth.statGrowth(inputGfaFile);

	gfa.printDigraphInfo(outputFolderPath, diGraph);
	gfa.printBigraphInfo(outputFolderPath, biedgedGraph);
	gfa.printDibigraphInfo(outputFolderPath, diBiedgedGraph);

    return 0;
}
