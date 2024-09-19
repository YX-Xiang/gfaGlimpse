#include "vertex.h"


Vertex::Vertex() {
    totalLen = 0;
    N50 = 0;
    L50 = 0;
    U50 = 0;
    deadEnd = 0;
}


Vertex::~Vertex() {}


std::pair<long long, long long> Vertex::calN50(std::vector<long long>& vec, long long total) {
    long long sum = 0, num  = 0;
    for (auto val: vec) {
        sum += val;
        num ++;
        if ((sum << 1) >= total) {
            return {val, num};
        }
    }
    return {0, 0};
}


void Vertex::statVertex(const DiGraph& diGraph) {
    std::vector<long long> tmpN, tmpU;
    for (auto v: diGraph.vertexVal) {
        //统计segment长度
        totalLen += v.second;
        tmpN.push_back(v.second);

        //统计度数分布
        if (diGraph.inDegree.count(v.first)) {
            inDegreeDistribution[diGraph.inDegree.at(v.first)] ++;
        } else {
            inDegreeDistribution[0] ++;
        }
        if (diGraph.outDegree.count(v.first)) {
            outDegreeDistribution[diGraph.outDegree.at(v.first)] ++;
        } else {
            outDegreeDistribution[0] ++;
        }
    }
    deadEnd = outDegreeDistribution[0];
    startEnd = inDegreeDistribution[0];

    std::sort(tmpN.begin(), tmpN.end(), std::greater <long long> () );
    tmpU = tmpN;

    long long total = 0;
    auto last = std::unique(tmpU.begin(), tmpU.end());
    tmpU.erase(last, tmpU.end());
    for (auto x: tmpU) {
        total += x;
    }

    std::tie(N50, L50) = calN50(tmpN, totalLen);
    U50 = calN50(tmpU, total).first;
}


void Vertex::statVertex(const BiedgedGraph& biedgedGraph) {
    std::vector<long long> tmpN, tmpU;
    // bidirected graph和biedged graph
    if (biedgedGraph.directed == 0) {
        for (long long vID = 1; vID <= biedgedGraph.vertexNumber; vID ++) {
            if (vID & 1) {
                totalLen += biedgedGraph.edge[vID].begin() -> value;
                tmpN.push_back(biedgedGraph.edge[vID].begin() -> value);

                bidirectedDegreeDistribution[biedgedGraph.edge[vID].size() + biedgedGraph.edge[vID + 1].size() - 2] ++;
            }
            if (biedgedGraph.edge[vID].size() == 1) {
                deadEnd ++;
            }
            linkDegreeDistribution[biedgedGraph.edge[vID].size() - 1] ++;
        }

        std::sort(tmpN.begin(), tmpN.end(), std::greater <long long> () );
        tmpU = tmpN;
        long long total = 0;
        auto last = std::unique(tmpU.begin(), tmpU.end());
        tmpU.erase(last, tmpU.end());
        for (auto x: tmpU) {
            total += x;
        }

        std::tie(N50, L50) = calN50(tmpN, totalLen);
        U50 = calN50(tmpU, total).first;

    } else { //directed biedged graph
        std::vector <long long> segmentInDegree = std::vector <long long> (biedgedGraph.vertexNumber + 1, 0);
        std::vector <long long> linkInDegree = std::vector <long long> (biedgedGraph.vertexNumber + 1, 0);
    
        for (long long vID = 1; vID <= biedgedGraph.vertexNumber; vID ++) {
            long long segmentOutDegree = 0, linkOutDegree = biedgedGraph.edge[vID].size();
            for (auto e: biedgedGraph.edge[vID]) {
                if (e.value) {
                    segmentInDegree[e.to] ++;
                    segmentOutDegree ++;
                    linkOutDegree --;
                } else {
                    linkInDegree[e.to] ++;
                }
            }
            if (segmentOutDegree + linkOutDegree == 0) {
                deadEnd ++;
            }
            segmentOutDegreeDistribution[segmentOutDegree] ++;
            linkOutDegreeDistribution[linkOutDegree] ++;
        }

        for (long long vID = 1; vID <= biedgedGraph.vertexNumber; vID ++) {
            if (segmentInDegree[vID] + linkInDegree[vID] == 0) {
                startEnd ++;
            }
            segmentInDegreeDistribution[segmentInDegree[vID]] ++;
            linkInDegreeDistribution[linkInDegree[vID]] ++;
        }
    }
}


// 输出度数分布（每一行第一个数字代表具体度数，第二个数字代表有几个点刚好是这个度数）
void Vertex::printInDegree2File(const std::string& outputFile) {
    std::ofstream output(outputFile);
    if (!output.is_open()) {
        std::cerr << "Error opening file: " << outputFile << std::endl;
        return;
    }

    for (auto in: inDegreeDistribution) {
        output << in.first << "\t" << in.second << "\n";
    }
    output.close();

    std::cerr << "--- The distribution of in-degree in the digraph has been successfully exported to the file: " << outputFile << std::endl;
}


void Vertex::printOutDegree2File(const std::string& outputFile) {
    std::ofstream output(outputFile);
    if (!output.is_open()) {
        std::cerr << "Error opening file: " << outputFile << std::endl;
        return;
    }

    for (auto out: outDegreeDistribution) {
        output << out.first << "\t" << out.second << "\n";
    }
    output.close();

    std::cerr << "--- The distribution of out-degree in the digraph has been successfully exported to the file: " << outputFile << std::endl;
}


void Vertex::printDegree2File(const std::string& outputFile) {
    std::ofstream output(outputFile);
    if (!output.is_open()) {
        std::cerr << "Error opening file: " << outputFile << std::endl;
        return;
    }

    for (auto degree: bidirectedDegreeDistribution) {
        output << degree.first << "\t" << degree.second << "\n";
    }
    output.close();

    std::cerr << "--- The distribution of degree in the bidirected graph has been successfully exported to the file: " << outputFile << std::endl;
}


void Vertex::printLinkDegree2File(const std::string& outputFile) {
    std::ofstream output(outputFile);
    if (!output.is_open()) {
        std::cerr << "Error opening file: " << outputFile << std::endl;
        return;
    }

    for (auto degree: linkDegreeDistribution) {
        output << degree.first << "\t" << degree.second << "\n";
    }
    output.close();

    std::cerr << "--- The distribution of degree in the biedged graph (only link edges) has been successfully exported to the file: " << outputFile << std::endl;
}


void Vertex::printDibiDegree2File(const std::string& outputFile) {
    std::ofstream output(outputFile);
    if (!output.is_open()) {
        std::cerr << "Error opening file: " << outputFile << std::endl;
        return;
    }

    output << "The distribution of in-degree\n";
    output << "Segment:\n";
    for (auto in: segmentInDegreeDistribution) {
        output << in.first << " " << in.second << "\n";
    }
    output << "Link:\n";
    for (auto in: linkInDegreeDistribution) {
        output << in.first << " " << in.second << "\n";
    }

    output << "The distribution of out-degree\n";
    output << "Segment:\n";
    for (auto out: segmentOutDegreeDistribution) {
        output << out.first << " " << out.second << "\n";
    }
    output << "Link:\n";
    for (auto out: linkOutDegreeDistribution) {
        output << out.first << " " << out.second << "\n";
    }
    output.close();

    std::cerr << "--- The distribution of degree in the directed biedged graph has been successfully exported to the file: " << outputFile << std::endl;
}