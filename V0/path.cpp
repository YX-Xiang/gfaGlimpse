#include "path.h"
#include <cstdlib>
#include <iostream>
#include <string>


Coverage::Coverage(const std::vector <std::vector <std::pair <int, char> > >& gfaPath): 
    path(gfaPath) {
        vertexCoverage = std::vector <int> (path.size() + 1, 0);
        edgeCoverage = std::vector <int> (path.size() + 1, 0);
        bpCoverage = std::vector <long long> (path.size() + 1, 0);
}


Coverage::~Coverage() {}


void Coverage::statCoverage(const DiGraph& diGraph) {
    int vertexMaxNumber = diGraph.edge.size();
    std::vector <std::set <int> > vertexTravelledPathList = std::vector <std::set <int> > (vertexMaxNumber, std::set <int> ());
    std::map <std::pair <std::pair <int, char>, std::pair <int, char> >, std::set <int> > edgeTravelledPathList;

    int pathCount = path.size();
    for (int pathID = 0; pathID < pathCount; pathID ++) {
        int pathSize = path[pathID].size();
        for (int p = 0; p < pathSize; p ++) {
            int id = path[pathID][p].first;
            char direction = path[pathID][p].second;
            if (direction == '+') {
                vertexTravelledPathList[(id << 1) - 1].insert(pathID);
            } else {
                vertexTravelledPathList[(id << 1)].insert(pathID);
            }

            if (p) {
                if (! edgeTravelledPathList.count({path[pathID][p - 1], path[pathID][p]})) {
                    edgeTravelledPathList[{path[pathID][p - 1], path[pathID][p]}] = std::set <int> ();
                }
                edgeTravelledPathList[{path[pathID][p - 1], path[pathID][p]}].insert(pathID);
            }
        }
    }

    for (int vertexID = 1; vertexID < vertexMaxNumber; vertexID ++) {
        //std::cout << vertexID << " " << vertexTravelledPathList[vertexID].size() << std::endl;
        if (diGraph.vertexVal.count(vertexID)) {
            vertexCoverage[vertexTravelledPathList[vertexID].size()] ++;
            bpCoverage[vertexTravelledPathList[vertexID].size()] += diGraph.vertexVal.at(vertexID);
        }
    }

    for (auto e: edgeTravelledPathList) {
        // std::cout << e.first.first.first << e.first.first.second << " " << e.first.second.first << e.first.second.second << ": ";
        // for (auto x: e.second) {
        //     std::cout << x << " ";
        // }
        // std::cout << std::endl;
        edgeCoverage[e.second.size()] ++;
    }
}


void Coverage::statCoverage(const BiedgedGraph& biedgedGraph) {
    int vertexMaxNumber = (biedgedGraph.vertexNumber >> 1) + 1;
    std::vector <std::set <int> > vertexTravelledPathList = std::vector <std::set <int> > (vertexMaxNumber, std::set <int> ());
    std::map <std::pair <std::pair <int, char>, std::pair <int, char> >, std::set <int> > edgeTravelledPathList;

    int pathCount = path.size();
    for (int pathID = 0; pathID < pathCount; pathID ++) {
        int pathSize = path[pathID].size();
        for (int p = 0; p < pathSize; p++) {
            int id = path[pathID][p].first;
            vertexTravelledPathList[id].insert(pathID);

            if (p) {
                // 无向边统一转化为编号较小的节点在前的表示
                std::pair <int, char> fi, se;
                if (path[pathID][p - 1].first == path[pathID][p].first) {
                    if (path[pathID][p - 1].second == path[pathID][p].second) {
                        fi.first = se.first = path[pathID][p - 1].first;
                        fi.second = se.second = '+';
                    } else {
                        fi = path[pathID][p - 1];
                        se = path[pathID][p];
                    }
                } else if (path[pathID][p - 1].first < path[pathID][p].first) {
                    fi = path[pathID][p - 1];
                    se = path[pathID][p];
                } else {
                    fi.first = path[pathID][p].first;
                    if (path[pathID][p].second == '-') {
                        fi.second = '+';
                    } else {
                        fi.second = '-';
                    }

                    se.first = path[pathID][p - 1].first;
                    if (path[pathID][p - 1].second == '-') {
                        se.second = '+';
                    } else {
                        se.second = '-';
                    }
                }

                if (! edgeTravelledPathList.count({fi, se})) {
                    edgeTravelledPathList[{fi, se}] = std::set <int> ();
                }
                edgeTravelledPathList[{fi, se}].insert(pathID);
            }
        }
    }

    for (int vertexID = 1; vertexID < vertexMaxNumber; vertexID ++) {
        // std::cout << vertexID << " " << vertexTravelledPathList[vertexID].size() << std::endl;
        vertexCoverage[vertexTravelledPathList[vertexID].size()] ++;
        bpCoverage[vertexTravelledPathList[vertexID].size()] += (biedgedGraph.edge[(vertexID << 1)].begin() -> value);
    }

    for (auto e: edgeTravelledPathList) {
        // std::cout << e.first.first.first << e.first.first.second << " " << e.first.second.first << e.first.second.second << ": ";
        // for (auto x: e.second) {
        //     std::cout << x << " ";
        // }
        // std::cout << std::endl;
        edgeCoverage[e.second.size()] ++;
    }
}


void Coverage::print2File(const std::string& outputFile, int mode) {
    std::ofstream output(outputFile);
    if (!output.is_open()) {
        std::cerr << "Error opening file: " << outputFile << std::endl;
        return;
    }

    int haplotypeCount = path.size();
    output << "Count\tbp\t\tNode\t\tEdge\n";
    for (int haplotype = 1; haplotype <= haplotypeCount; haplotype ++) {
        output << haplotype << "\t\t" << bpCoverage[haplotype] << "\t\t"
            << vertexCoverage[haplotype] << "\t\t" << edgeCoverage[haplotype] << '\n';
    }
    output.close();

    if (mode == 1) {
        std::cerr << "--- The coverage of the digraph has been successfully exported to the file: " << outputFile << std::endl;
    } else if (mode == 2) {
        std::cerr << "--- The coverage of the bidirected graph has been successfully exported to the file: " << outputFile << std::endl;
    }
    
}


Growth::Growth(const std::string& outputFolderPath) {
    if (!outputFolderPath.empty() && outputFolderPath.back() == '/') {
        outputPath = outputFolderPath + "gfa";
    } else {
        outputPath = outputFolderPath + "/gfa";
    }
}


Growth::~Growth() {}


void Growth::statGrowth(const std::string& gfaFile) {
    std::string path_file = outputPath + "/paths.haplotypes.txt";
    std::ifstream infile(gfaFile);
    std::ofstream outfile(path_file);
    std::string line;

    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    while (std::getline(infile, line)) {
        if (line[0] == 'W') {
            std::istringstream iss(line);
            std::string fields[5];
            for (int i = 0; i < 5; ++i) {
                if (!std::getline(iss, fields[i], '\t')) break;
            }
            outfile << fields[0] << "#" << fields[1] << "#" << fields[2] << ":"
                    << fields[3] << "-" << fields[4] << '\n';
        } else if (line[0] == 'P') {
            std::istringstream iss(line);
            std::string field;
            if (std::getline(iss, field, '\t') && std::getline(iss, field, '\t')) {
                outfile << field << '\n';
            }
        }
    }

    std::string htmlFile = outputPath + "/histgrowth.html";
    std::string csvFile = outputPath + "/histgrowth.csv";
    std::string logFile = outputPath + "/histgrowth.log";
    std::string commandHtml = "RUST_LOG=info panacus histgrowth -t"
        + std::to_string(numThreads)
        + " -l 1,2,1,1,1 -q 0,0,1,0.5,0.1 -S -s " 
        + path_file + " -c all -a -o html " + gfaFile + " > " + htmlFile + " 2> " + logFile;
    std::string commandCsv = "RUST_LOG=info panacus histgrowth -t"
        + std::to_string(numThreads)
        + " -l 1,2,1,1,1 -q 0,0,1,0.5,0.1 -S -s " 
        + path_file + " -c all -a " + gfaFile + " > " + csvFile + " 2>> " + logFile;
    FILE* pipe1 = popen(commandHtml.c_str(), "r");
    FILE* pipe2 = popen(commandCsv.c_str(), "r");

    std::ifstream file(csvFile);
    if (pipe2 == NULL) {
        std::cerr << "!!! Error: Unexpected growth calculation. Please check the log file for more details: " << logFile << std::endl;
    } else {
        std::cout << "+++ Growth calculation successful." << std::endl << std::endl;
    }
}   


/*Growth::Growth(const std::vector <std::vector <std::pair <int, char> > >& gfaPath): 
    path(gfaPath) {
        vertexGrowth = std::vector <int> (path.size() + 1, 0);
        vertexCommonGrowth = std::vector <int> (path.size() + 1, 0);
        vertexCoreGrowth = std::vector <int> (path.size() + 1, 0);
        edgeGrowth = std::vector <int> (path.size() + 1, 0);
        edgeCommonGrowth = std::vector <int> (path.size() + 1, 0);
        edgeCoreGrowth = std::vector <int> (path.size() + 1, 0);
        bpGrowth = std::vector <long long> (path.size() + 1, 0);
        bpCommonGrowth = std::vector <long long> (path.size() + 1, 0);
        bpCoreGrowth = std::vector <long long> (path.size() + 1, 0);
}


Growth::~Growth() {}


void Growth::statGrowth(const std::string& gfaFile) {
    // T. B. C.
}


void Growth::statGrowth(const BiedgedGraph& biedgedGraph) {
    // T. B. C.
}


void Growth::print2File(const std::string&) {
    // T. B. C.
}*/