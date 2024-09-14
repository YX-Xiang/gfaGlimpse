#include "edge.h"
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>


Edge::Edge() {
    edgeCount = 0;
    linkEdgeCount = 0;
    loopCount = 0;
    loopLen = std::unordered_map <long long, int> ();
}


Edge::~Edge() {}


void Edge::stat(const DiGraph& diGraph) {
    int maxVertexNumber = diGraph.edge.size();
    for (int vID = 1; vID < maxVertexNumber; vID ++) {
        edgeCount += diGraph.edge[vID].size();
        if (diGraph.edge[vID].find(vID) != diGraph.edge[vID].end()) {
            loopCount ++;
            if (diGraph.vertexVal.count(vID)) {
                loopLen[diGraph.vertexVal.at(vID)] ++;
            } else {
                std::cerr << "There were some problems with counting the loops in the digraph !!!" << std::endl;
            }
        }
    }
}


void Edge::stat(const BiedgedGraph& biedgedGraph) {
    int maxVertexNumber = biedgedGraph.edge.size();
    for (int vID = 1; vID < maxVertexNumber; vID ++) {
        linkEdgeCount += biedgedGraph.edge[vID].size() - 1;
        
        Biedge tmp;
        tmp.value = 0;
        tmp.to = vID;
        if (biedgedGraph.edge[vID].find(tmp) != biedgedGraph.edge[vID].end()) {
            loopCount ++;
            loopLen[0] ++;
        }

        if (vID & 1) {
            tmp.to = vID + 1;
            if (biedgedGraph.edge[vID].find(tmp) != biedgedGraph.edge[vID].end()) {
                loopCount ++;
                if (biedgedGraph.edge[vID].begin() -> value) {
                    loopLen[biedgedGraph.edge[vID].begin() -> value] ++;
                } else {
                    std::cerr << "There were some problems with counting the loops in the biedged graph !!!" << std::endl;
                }
            }
        }
    }
    linkEdgeCount /= 2;
}


void Edge::print2File(const std::string& outputFile) {
    // T. B. C.
    std::cout << "Edges:\t" << std::max(edgeCount, linkEdgeCount) << "\n" 
        << "Loops:\t" << loopCount << "\n";
    for (auto loop: loopLen) {
        std::cout << loop.first << " " << loop.second << std::endl;
    }

    loopLen = std::unordered_map <long long, int>();
}


Cycle::Cycle() {
    cycleCount = 0;
    minCycleLen = 0x7FFFFFFFFFFFFFFF;
    cycleLen = std::unordered_map <long long, int> ();
}


Cycle::~Cycle() {}


void Cycle::work(std::vector <DiGraph>& subgraphList) {
    for (auto subgraph: subgraphList) {
		Cycle diCycle;
		diCycle.statCycle(subgraph);
		cycleCount += diCycle.cycleCount;
		if (diCycle.cycleCount) {
			minCycleLen = std::min(minCycleLen, diCycle.minCycleLen);
		}
		for (auto item: diCycle.cycleLen) {
			cycleLen[item.first] += item.second;
		}
	}
}


void Cycle::work(std::vector <BiedgedGraph>& subgraphList) {
    for (auto subgraph: subgraphList) {
		Cycle diBiCycle;
		diBiCycle.statCycle(subgraph);
		cycleCount += diBiCycle.cycleCount;
		if (diBiCycle.cycleCount) {
			minCycleLen = std::min(minCycleLen, diBiCycle.minCycleLen);
		}
		for (auto item: diBiCycle.cycleLen) {
			cycleLen[item.first] += item.second;
		}
	}
}


void Cycle::statCycle(const DiGraph& diGraph) {
    int vertexMaxCount = diGraph.edge.size();
    std::vector <int> vertexVal = std::vector <int> (vertexMaxCount, 0);
    for (int v = 1; v < vertexMaxCount; v ++) {
        if (diGraph.vertexVal.count(v)) {
            vertexVal[v] = diGraph.vertexVal.at(v);
        }
    }

    std::vector <std::set <int> > edge = std::vector <std::set <int> > (vertexMaxCount, std::set <int>());
    for (int v = 1; v < vertexMaxCount; v ++) {
        if (! diGraph.edge[v].empty()) {
            for (auto e: diGraph.edge[v]) {
                if (e != v) {
                    edge[v].insert(e);
                }
            }
        }
    }

    // 可并行
    for (auto v: diGraph.vertexVal) {
        int beginVertex = v.first;
        findCycleInDirected(beginVertex, vertexVal, edge);
    }
}


void Cycle::statCycle(const BiedgedGraph& biedgedGraph) {
    int vertexMaxCount = biedgedGraph.edge.size();
    std::vector <int> vertexVal = std::vector <int> (vertexMaxCount, 0);
    std::vector <std::set <std::pair<int, int> > > edgeVal = std::vector <std::set <std::pair<int, int> > > (vertexMaxCount, std::set <std::pair<int, int> >());

    for (int v = 1; v < vertexMaxCount; v ++) {
        if (! biedgedGraph.edge[v].empty()) {
            for (auto e: biedgedGraph.edge[v]) {
                if (e.to != v) {
                    edgeVal[v].insert({e.to, e.value});
                }
            }
        }
    }

    // 可并行
    for (int v = 1; v < vertexMaxCount; v ++) {
        findCycleInBiedged(v, vertexVal, edgeVal);
    }
}


// 写法可能跟理论时间复杂度对不上，后期重点修改！！！
void Cycle::findCycleInDirected(int startVertex, const std::vector <int>& vertexVal, 
    const std::vector <std::set <int> >& edge) {

    int vertexNumber = vertexVal.size();
    std::vector <bool> blocked = std::vector <bool> (vertexNumber, 0);

    // 手动模拟dfs栈
    std::stack <int> dfsStack = std::stack <int> ();
    dfsStack.push(startVertex);

    std::vector <int> path;
    path.push_back(startVertex);
    blocked[startVertex] = 1;
    long long totalLength = vertexVal[startVertex];

    // 存储每个点访问邻接边的断点
    std::vector <int> visited = std::vector <int> (vertexNumber, startVertex);
    // 标记每个点是否被至少一个cycle包含
    std::vector <int> boolCycle = std::vector <int> (vertexNumber, 0);
    // 标记每个点后继会出现的死胡同
    std::vector <std::set <int> > blockList = std::vector <std::set <int> > (vertexNumber, std::set <int> ());

    while (! dfsStack.empty()) {
        int nowNode = dfsStack.top();
        bool found = 1;

        // std::cout << "Path: ";
        // for (auto p: path) {
        //     std::cout << p << " ";
        // }
        // std::cout << "\n";

        auto it = edge[nowNode].lower_bound(visited[nowNode]);
        if (it != edge[nowNode].end()) {
            int nxtNode = *it;
            visited[nowNode] = nxtNode + 1;
            if (nxtNode == startVertex) {
                cycleCount ++;
                cycleLen[totalLength] ++;
                minCycleLen = std::min(minCycleLen, totalLength);

                for(auto p: path) {
                    boolCycle[p] = 1;
                }

            } else if (! blocked[nxtNode]) {
                dfsStack.push(nxtNode);
                path.push_back(nxtNode);

                totalLength += vertexVal[nxtNode];

                blocked[nxtNode] = 1;
                found = 0;
            }
        }

        // When no extension from vertex nowNode is possible
        if (found) {
            path.pop_back();
            dfsStack.pop();

            // If no cycle containing nowNode has been found, 
            // then nowNode stays blocked and is added to the list blockList[adjacentNode] for each adjacentNode for which an edge (nowNode, adjacentNode) exists.
            if (! boolCycle[nowNode]) {
                for(auto adjacentNode: edge[nowNode]) {
                    blockList[adjacentNode].insert(nowNode);
                }
            } else {
            // If a cycle containing nowNode has been found,
            // then a procedure UNBLOCK(nowNode) is called, which 'unblocks' nowNode and recursively calls UNBLOCK(adjacentNode) for each adjacentNode \in blockList[adjacentNode].
                std::queue <int> unblockList;
                std::unordered_map <int, int> queueVisit;
                unblockList.push(nowNode);
                queueVisit[nowNode] = 1;
                while (! unblockList.empty()) {
                    int unblockElement = unblockList.front();
                    unblockList.pop();

                    blocked[unblockElement] = 0;
                    for (auto adjacentNode: blockList[unblockElement]) {
                        if (! queueVisit[adjacentNode]) {
                            queueVisit[adjacentNode] = 1;
                            unblockList.push(adjacentNode);
                        }
                    }
                }
                boolCycle[nowNode] = 0;
            }

            if (! path.empty()) {
                int preFinished = path.back();

                totalLength -= vertexVal[nowNode];
                visited[nowNode] = startVertex + 1;
            }
        }
    }
}


// walk有特殊要求，要求必须以segment edge起始，以segment edge终止，且segment edge和link edge首尾相接
// 为了加速，因为cycle的性质，以segment edge起始且以segment edge终止，可以等价看成以link edge起始，以link edge终止，因此这两种都看作有效的cycle。这样可以避免由于先遍历的点在link edge上而无法找到cycle。
void Cycle::findCycleInBiedged(int startVertex, const std::vector <int>& vertexVal, 
    const std::vector <std::set <std::pair<int, int> > >& edgeVal) {

    int vertexNumber = vertexVal.size();
    std::vector <bool> blocked = std::vector <bool> (vertexNumber, 0);

    std::stack <int> dfsStack = std::stack <int> ();
    dfsStack.push(startVertex);

    std::vector <int> path;
    path.push_back(startVertex);
    blocked[startVertex] = 1;
    long long totalLength = vertexVal[startVertex];
    bool preEdge = 0;

    std::vector <int> visited = std::vector <int> (vertexNumber, startVertex);

    while (! dfsStack.empty()) {
        int nowNode = dfsStack.top();
        bool found = 1;

        auto it = edgeVal[nowNode].lower_bound({visited[nowNode], 0});
        if (it != edgeVal[nowNode].end()) {
            int nxtNode = it->first;
            visited[nowNode] = nxtNode + 1;
            if (nxtNode == startVertex  && (((it->second) > 0 ? 1 : 0) ^ preEdge) ) {
                cycleCount ++;
                cycleLen[totalLength + (it->second)] ++;
                minCycleLen = std::min(minCycleLen, totalLength);

            } else if (! blocked[nxtNode] && (path.size() == 1 || (((it->second) > 0 ? 1 : 0) ^ preEdge) ) ) {
                if (((it->second) > 0 ? 1 : 0) ^ preEdge) {
                    preEdge = 1 - preEdge;
                }
                
                dfsStack.push(nxtNode);
                path.push_back(nxtNode);

                totalLength += vertexVal[nxtNode];
                totalLength += it->second;

                blocked[nxtNode] = 1;
                found = 0;
            }
        }

        if (found) {
            int finished = path.back();
            path.pop_back();
            dfsStack.pop();
            preEdge = 1 - preEdge;
            blocked[finished] = 0;

            if (! path.empty()) {
                int preFinished = path.back();
                int edgeLength = edgeVal[preFinished].lower_bound({finished, 0})->second;

                totalLength -= vertexVal[finished];
                totalLength -= edgeLength;
                visited[finished] = startVertex;
            }
        }
    }
}


void Cycle::print2File(const std::string&) {
    // T. B. C.
    if (minCycleLen == 0x7FFFFFFFFFFFFFFF) {
        minCycleLen = -1;
    }
    std::cout << "Cycles:\t" << cycleCount << "\n" 
        << "Minimum Cycle:\t" << minCycleLen << "\n";
    for (auto cycle: cycleLen) {
        std::cout << cycle.first << " " << cycle.second << std::endl;
    }
    cycleLen = std::unordered_map <long long, int> ();
}