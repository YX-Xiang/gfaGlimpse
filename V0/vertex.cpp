#include "vertex.h"
#include <iostream>
#include <vector>


Vertex::Vertex() {
    totalLen = 0;
    N50 = 0;
    L50 = 0;
    U50 = 0;
}


Vertex::~Vertex() {}


int Vertex::calN50(std::vector<int>& vec) {
    long long sum = 0;
    for (auto val: vec) {
        sum += val;
        L50 ++;
        if ((sum << 1) >= totalLen) {
            return val;
        }
    }
    return 0;
}


void Vertex::statVertex(const DiGraph& diGraph) {
    std::vector<int> tmpN, tmpU;
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

    std::sort(tmpN.begin(), tmpN.end(), std::greater <int> () );
    tmpU = tmpN;
    auto last = std::unique(tmpU.begin(), tmpU.end());
    tmpU.erase(last, tmpU.end());

    N50 = calN50(tmpN);
    U50 = calN50(tmpU);
}


void Vertex::statVertex(const BiedgedGraph& biedgedGraph) {
    std::vector<int> tmpN, tmpU;
    // bidirected graph和biedged graph
    if (biedgedGraph.directed == 0) {
        for (int vID = 1; vID <= biedgedGraph.vertexNumber; vID ++) {
            if (vID & 1) {
                totalLen += biedgedGraph.edge[vID].begin() -> value;
                tmpN.push_back(biedgedGraph.edge[vID].begin() -> value);

                bidirectedDegreeDistribution[biedgedGraph.edge[vID].size() + biedgedGraph.edge[vID + 1].size() - 2] ++;
            }
            linkDegreeDistribution[biedgedGraph.edge[vID].size() - 1] ++;
        }

        std::sort(tmpN.begin(), tmpN.end(), std::greater <int> () );
        tmpU = tmpN;
        auto last = std::unique(tmpU.begin(), tmpU.end());
        tmpU.erase(last, tmpU.end());

        N50 = calN50(tmpN);
        U50 = calN50(tmpU);

    } else { //directed biedged graph
        std::vector <int> segmentInDegree = std::vector <int> (biedgedGraph.vertexNumber + 1, 0);
        std::vector <int> linkInDegree = std::vector <int> (biedgedGraph.vertexNumber + 1, 0);
    
        for (int vID = 1; vID <= biedgedGraph.vertexNumber; vID ++) {
            int segmentOutDegree = 0, linkOutDegree = biedgedGraph.edge[vID].size();
            for (auto e: biedgedGraph.edge[vID]) {
                if (e.value) {
                    segmentInDegree[e.to] ++;
                    segmentOutDegree ++;
                    linkOutDegree --;
                } else {
                    linkInDegree[e.to] ++;
                }
            }
            segmentOutDegreeDistribution[segmentOutDegree] ++;
            linkOutDegreeDistribution[linkOutDegree] ++;
        }

        for (int vID = 1; vID <= biedgedGraph.vertexNumber; vID ++) {
            segmentInDegreeDistribution[segmentInDegree[vID]] ++;
            linkInDegreeDistribution[linkInDegree[vID]] ++;
        }
    }
}


void Vertex::print2File(const std::string& outputFile) {
    std::cout << "Total Length:\t" << totalLen << " bp\n" 
        << "N50:\t" << N50 << " bp\n" 
        << "L50:\t" << L50 << " bp\n"
        << "U50:\t" << U50 << " bp\n";
        
    // 输出度数分布（每一行第一个数字代表具体度数，第二个数字代表有几个点刚好是这个度数）
    if (! inDegreeDistribution.empty()) {
        std::cout << "The distribution of in-degree\n";
        for (auto in: inDegreeDistribution) {
            std::cout << in.first << " " << in.second << "\n";
        }
        std::cout << "The distribution of out-degree\n";
        for (auto out: outDegreeDistribution) {
            std::cout << out.first << " " << out.second << "\n";
        }
    }
    if (! bidirectedDegreeDistribution.empty()) {
        std::cout << "The degree distribution in the bidirected graph\n";
        for (auto degree: bidirectedDegreeDistribution) {
            std::cout << degree.first << " " << degree.second << "\n";
        }
    }
    if (! linkDegreeDistribution.empty()) {
        std::cout << "The degree distribution of links in the biedged graph\n";
        for (auto degree: linkDegreeDistribution) {
            std::cout << degree.first << " " << degree.second << "\n";
        }
    }
    if (! segmentInDegreeDistribution.empty()) {
        std::cout << "The distribution of in-degree\n";
        std::cout << "Segment:\n";
        for (auto in: segmentInDegreeDistribution) {
            std::cout << in.first << " " << in.second << "\n";
        }
        std::cout << "Link:\n";
        for (auto in: linkInDegreeDistribution) {
            std::cout << in.first << " " << in.second << "\n";
        }

        std::cout << "The distribution of out-degree\n";
        std::cout << "Segment:\n";
        for (auto out: segmentOutDegreeDistribution) {
            std::cout << out.first << " " << out.second << "\n";
        }
        std::cout << "Link:\n";
        for (auto out: linkOutDegreeDistribution) {
            std::cout << out.first << " " << out.second << "\n";
        }
    }
}