#include "gfa.h"
#include <iostream>


Gfa::Gfa() {
    segmentNumber = 0;
    linkNumber = 0;
    path = std::vector <std::pair <int, char> > ();
}


Gfa::~Gfa() {}


void Gfa::gfa2Graph (const std::string& gfaFile, DiGraph& diGraph, BiedgedGraph& biedgedGraph) {
    std::cerr << "+++ Load gfa file from " << gfaFile << std::endl;

    // 获取文件大小
    struct stat info;
    stat(gfaFile.c_str(), &info);
    fileSize = info.st_size;

    std::ifstream ifs(gfaFile);
    std::vector <std::string> tempLine = std::vector <std::string> ();
    std::vector <std::string> tempPath = std::vector <std::string> ();
    std::vector <std::string> tempWalk = std::vector <std::string> ();
    std::unordered_map <std::string, long long> segmentID;

    if(!ifs.is_open()) {
        std::cerr << "!!! Error: failed to open " << gfaFile << std::endl;
    }

    while (!ifs.eof()) {
        std::string line;
        std::getline(ifs, line);
        // std::cout << line << std::endl;

        if(line[0] == 'S') {
            segmentNumber ++;

            std::istringstream iss(line);
            std::vector <std::string> tokens;
            std::string token;

            while (std::getline(iss, token, '\t')) {
                tokens.push_back(token);
            }

            segmentID[tokens[1]] = segmentNumber;
            
            // biedgedGraph加segment型边
            long long rnode = (segmentNumber << 1), lnode = rnode - 1;
            Biedge newBiedge;
            newBiedge.to = rnode;
            newBiedge.value = tokens[2].size();

            biedgedGraph.edge.push_back(std::set <Biedge> ());
            biedgedGraph.edge.push_back(std::set <Biedge> ());
            biedgedGraph.edge[lnode].insert(newBiedge);

            newBiedge.to = lnode;
            biedgedGraph.edge[rnode].insert(newBiedge);
            // std::cout << lnode << " " << rnode << " " << newBiedge.value << std::endl;

        } else if(line[0] == 'L') {
            linkNumber ++;
            tempLine.push_back(line);

        } else if(line[0] == 'P') {
            tempPath.push_back(line);

        } else if(line[0] == 'W') {
            tempWalk.push_back(line);
        }
    }
    // std::cout << segmentNumber << " " << linkNumber << std::endl;
    diGraph.edge = std::vector <std::set <long long> > ((segmentNumber << 1) + 1, std::set <long long>());

    for(auto line: tempLine) {
        std::istringstream iss(line);
        std::vector <std::string> tokens;
        std::string token;

        while (std::getline(iss, token, '\t')) {
            tokens.push_back(token);
        }

        long long lseg, rseg, lnode, rnode, lvertex, rvertex;
        if (segmentID.count(tokens[1])) {
            lseg = segmentID[tokens[1]];
        } else {
            std::cerr << "!!! Error: failed to find the segment " << tokens[1] << std::endl;
            continue;
        }
        
        if (segmentID.count(tokens[3])) {
            rseg = segmentID[tokens[3]];
        } else {
            std::cerr << "!!! Error: failed to find the segment " << tokens[3] << std::endl;
            continue;
        }

        if (tokens[2] == "+") {
            lnode = (lseg << 1);
            lvertex = (lseg << 1) - 1;
        } else {
            lnode = (lseg << 1) - 1;
            lvertex = (lseg << 1);
        }

        if(tokens[4] == "+") {
            rnode = (rseg << 1) - 1;
            rvertex = (rseg << 1) - 1;
        } else {
            rnode = (rseg << 1);
            rvertex = (rseg << 1);
        }
        // std::cout << line << std::endl;
        // std::cout << lnode << " " << rnode << std::endl;

        // biedgedGraph加link型边
        Biedge newBiedge;
        newBiedge.to = rnode;
        biedgedGraph.edge[lnode].insert(newBiedge);

        newBiedge.to = lnode;
        biedgedGraph.edge[rnode].insert(newBiedge);

        // diGraph赋点权，加边
        diGraph.vertexVal[lvertex] = biedgedGraph.edge[lvertex].begin() -> value;
        diGraph.vertexVal[rvertex] = biedgedGraph.edge[rvertex].begin() -> value;
        diGraph.edge[lvertex].insert(rvertex);
        diGraph.inDegree[rvertex] ++;
        diGraph.outDegree[lvertex] ++;
    }
    diGraph.vertexNumber = diGraph.vertexVal.size();
    biedgedGraph.vertexNumber = (segmentNumber << 1);
    // std::cout << diGraph.vertexNumber << " " << biedgedGraph.vertexNumber <<std::endl;

    for (auto line: tempPath) {
        std::istringstream iss(line);
        std::vector <std::string> tokens;
        std::string token;

        while (std::getline(iss, token, '\t')) {
            tokens.push_back(token);
        }

        std::string segmentName;
        for (auto c: tokens[2]) {
            if (c == ',') {
                segmentName = "";
            } else if (c == '+' || c == '-') {
                if (segmentID.count(segmentName)) {
                    path.push_back({segmentID[segmentName], c});
                } else {
                    std::cerr << "!!! Error: failed to find the segment " << segmentName 
                        << " in the path " << tokens[1] << std::endl;
                }
                
            } else {
                segmentName.push_back(c);
            }
        }
    }

    for (auto line: tempWalk) {
        std::istringstream iss(line);
        std::vector <std::string> tokens;
        std::string token;

        while (std::getline(iss, token, '\t')) {
            tokens.push_back(token);
        }

        std::string segmentName;
        for (auto c: tokens[6]) {
            if (c == '>') {
                path.push_back({segmentID[segmentName], '+'});
                segmentName = "";
            } else if (c == '<') {
                path.push_back({segmentID[segmentName], '-'});
                segmentName = "";
            } else {
                segmentName.push_back(c);
            }
        }
    }

    std::cerr << "+++ Loading complete! The gfa file has " << segmentNumber 
        << " segments and " << linkNumber << " links, with a total size of "
        << fileSize << "B. +++\n";
}