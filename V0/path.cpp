#include "path.h"
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


void Coverage::print2File(const std::string&) {
    // T. B. C.
}


Growth::Growth(const std::string& outputPath) {
    if (!outputPath.empty() && outputPath.back() == '/') {
        tmpPath = outputPath + "tmp";
    } else {
        tmpPath = outputPath + "/tmp";
    }

    std::string command = "mkdir -p " + tmpPath;
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Command failed with return code " << result << std::endl;
    }
}


Growth::~Growth() {}


void Growth::statGrowth(const std::string& gfaFile) {
    std::string path_file = tmpPath + "/paths.haplotypes.txt";
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

    std::string html_file = tmpPath + "/histgrowth.html";
    std::string command = "panacus histgrowth -t4 -l 1,2,1,1,1 -q 0,0,1,0.5,0.1 -S -s " 
        + path_file + " -c all -a -o html " + gfaFile + " > " + html_file;
    FILE* pipe = popen(command.c_str(), "r");
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