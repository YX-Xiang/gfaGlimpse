#include "edge.h"
#include <iostream>
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
        edgeCount += biedgedGraph.edge[vID].size();
        
        // 一个口进出的不应该计入loop中
        // if (biedgedGraph.edge[vID].find(tmp) != biedgedGraph.edge[vID].end()) {
        //     loopCount ++;
        //     loopLen[0] ++;
        // }

        if (vID & 1) {
            Biedge tmp1, tmp2;
            tmp1.value = 0;
            tmp1.to = vID + 1;

            tmp2.value = 0;
            tmp2.to = vID;
            // 存在二元环当且仅当以下两种情况之一
            // vID有一条指向 vID + 1 的link边，vID + 1有一条指向 vID 的segment边
            // vID有一条指向 vID + 1 的segment边，vID + 1有一条指向 vID 的link边
            if (biedgedGraph.edge[vID].find(tmp1) != biedgedGraph.edge[vID].end() 
                && biedgedGraph.edge[vID + 1].begin()->to == vID && biedgedGraph.edge[vID + 1].begin()->value > 0) {
                loopCount ++;
                loopLen[biedgedGraph.edge[vID + 1].begin()->value] ++;
                
            } else if (biedgedGraph.edge[vID].begin()->to == vID + 1 && biedgedGraph.edge[vID].begin()->value > 0 
                && biedgedGraph.edge[vID + 1].find(tmp2) != biedgedGraph.edge[vID + 1].end()) {
                loopCount ++;
                loopLen[biedgedGraph.edge[vID].begin()->value] ++;
            }
        }
    }
    linkEdgeCount /= 2;
}


void Edge::print2File(const std::string& outputFile, int mode) {
    std::ofstream output(outputFile);
    if (!output.is_open()) {
        std::cerr << "Error opening file: " << outputFile << std::endl;
        return;
    }

    for (auto loop: loopLen) {
        output << loop.first << "\t" << loop.second << std::endl;
    }
    output.close();
    if (mode == 1) {
        std::cerr << "--- The " << loopCount << " loop(s) in the digraph has been successfully exported to the file: " << outputFile << std::endl;
    } else if (mode == 2) {
        std::cerr << "--- The " << loopCount << " loop(s) in the bidirected graph has been successfully exported to the file: " << outputFile << std::endl;
    } else if (mode == 3) {
        std::cerr << "--- The " << loopCount << " loop(s) in the biedged graph has been successfully exported to the file: " << outputFile << std::endl;
    } else if (mode == 4) {
        std::cerr << "--- The " << loopCount << " loop(s) in the directed biedged graph has been successfully exported to the file: " << outputFile << std::endl;
    }
    
    loopLen = std::unordered_map <long long, int>();
}


Cycle::Cycle() {
    cycleCount = 0;
    minCycleLen = 0x7FFFFFFFFFFFFFFF;
    cycleLen = std::unordered_map <long long, int> ();
}


Cycle::~Cycle() {}


void Cycle::work(std::vector <DiGraph>& subgraphList, std::unordered_map <long long, int>& loopLen) {
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

    if (minCycleLen == 0x7FFFFFFFFFFFFFFF) {
        minCycleLen = -1;
    }

    for (auto item: loopLen) {
        cycleLen[item.first] += item.second;
    }
}


void Cycle::work(std::vector <BiedgedGraph>& subgraphList, std::unordered_map <long long, int>& loopLen) {
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

    if (minCycleLen == 0x7FFFFFFFFFFFFFFF) {
        minCycleLen = -1;
    }

    for (auto item: loopLen) {
        cycleLen[item.first] += item.second;
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


void Cycle::findCycleInDirected(int startVertex, const std::vector <int>& vertexVal, 
    const std::vector <std::set <int> >& edge) {
    int vertexNumber = vertexVal.size();

    // 标记这个点当前是否是死胡同
    std::vector <bool> blocked = std::vector <bool> (vertexNumber, 0);
    // 存储每个点访问邻接边的断点
    std::vector <int> visited = std::vector <int> (vertexNumber, startVertex);
    // 标记每个点是否被至少一个cycle包含
    std::vector <int> boolCycle = std::vector <int> (vertexNumber, 0);
    // 标记每个点后继会出现的死胡同
    std::vector <std::set <int> > blockList = std::vector <std::set <int> > (vertexNumber, std::set <int> ());

    // 手动模拟dfs栈
    std::stack <int> dfsStack = std::stack <int> ();
    dfsStack.push(startVertex);

    std::vector <int> path;
    path.push_back(startVertex);

    blocked[startVertex] = 1;
    long long totalLength = vertexVal[startVertex];

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
        } else {
        // When no extension from vertex nowNode is possible
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
                    blockList[unblockElement] = std::set<int> ();
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
// 因为路径存在不同性质，因此在block时也要分为两个记号
void Cycle::findCycleInBiedged(int startVertex, const std::vector <int>& vertexVal, 
    const std::vector <std::set <std::pair<int, int> > >& edgeVal) {
    int vertexNumber = vertexVal.size();

    std::vector <int> visited = std::vector <int> (vertexNumber, startVertex);
    std::vector <bool> boolInStack = std::vector <bool> (vertexNumber, 0);
    std::vector <bool> linkBlocked = std::vector <bool> (vertexNumber, 0);
    std::vector <bool> edgeBlocked = std::vector <bool> (vertexNumber, 0);
    std::vector <int> boolLinkCycle = std::vector <int> (vertexNumber, 0);
    std::vector <int> boolEdgeCycle = std::vector <int> (vertexNumber, 0);
    std::vector <std::set <int> > linkBlockList = std::vector <std::set <int> > (vertexNumber, std::set <int> ());
    std::vector <std::set <int> > edgeBlockList = std::vector <std::set <int> > (vertexNumber, std::set <int> ());

    std::stack <int> dfsStack = std::stack <int> ();
    dfsStack.push(startVertex);
    boolInStack[startVertex] = 1;

    std::vector <int> path;
    path.push_back(startVertex);

    long long totalLength = vertexVal[startVertex];

    while (! dfsStack.empty()) {
        int nowNode = dfsStack.top();

        // std::cout << "Path: ";
        // for (auto p: path) {
        //     std::cout << p << " ";
        // }
        // std::cout << "\n";

        auto it = edgeVal[nowNode].lower_bound({visited[nowNode], 0});
        if (it != edgeVal[nowNode].end()) {
            int nxtNode = it->first, weight = it->second;
            bool boolExtent = 0;

            visited[nowNode] = nxtNode + 1;
            if (! boolInStack[nxtNode] && ((weight && ! linkBlocked[nxtNode]) || (! weight && ! edgeBlocked[nxtNode]))) {
                boolExtent = 1;
            }

            if (nxtNode == startVertex) {
                // 判断找到的cycle是否合法，首先要满足点数为偶数
                bool legallyCycle = 1 - path.size() % 2;

                // 反向遍历path，更新各个点是不是死胡同。
                // nxtEdgeCharacter记录后继边的性质，判断是否与当前边矛盾。
                path.push_back(startVertex);
                int pathLength = path.size();
                bool nxtEdgeCharacter = ((weight > 0) ? 0 : 1);
                for (int p = pathLength - 2; p >= 0; p --) {
                    bool nowEdgeCharacter;
                    if ((edgeVal[path[p]].lower_bound({path[p + 1], 0})->second) > 0) {
                        nowEdgeCharacter = 1;
                    } else {
                        nowEdgeCharacter = 0;
                    }
                    
                    if (nowEdgeCharacter != nxtEdgeCharacter) {
                        if (nowEdgeCharacter) {
                            boolEdgeCycle[path[p]] = 1;
                        } else {
                            boolLinkCycle[path[p]] = 1;
                        }
                        nxtEdgeCharacter = nowEdgeCharacter;
                    } else {
                        legallyCycle = 0;
                        break;
                    }
                }
                path.pop_back();

                if (legallyCycle) {
                    cycleCount ++;
                    cycleLen[totalLength + weight] ++;
                    minCycleLen = std::min(minCycleLen, totalLength);
                }

            } else if (boolExtent) {
                // 如果 (nowNode, nxtNode) 是edge边，则nxtNode走link边权限被锁，除非后续能找到cycle解锁；
                // 如果 (nowNode, nxtNode) 是link边，则nxtNode走edge边权限被锁，除非后续能找到cycle解锁
                if(weight) {
                    linkBlocked[nxtNode] = 1;
                } else {
                    edgeBlocked[nxtNode] = 1;
                }
                
                dfsStack.push(nxtNode);
                boolInStack[nxtNode] = 1;
                path.push_back(nxtNode);

                totalLength += vertexVal[nxtNode];
                totalLength += it->second;
            }
        } else {
            path.pop_back();
            dfsStack.pop();
            boolInStack[nowNode] = 0;
            
            // 若当前节点nowNode不能通过拓展后继edge边找到cycle，则将与nowNode放入与nowNode有edge边相连的adjacentNode的edgeBlockList中，
            // 表示除非有adjacentNode可以走link边找到cycle，否则nowNode就别走edge边了
            if (! boolEdgeCycle[nowNode]) {
                for(auto adjacentNode: edgeVal[nowNode]) {
                    if (adjacentNode.second) {
                        edgeBlockList[adjacentNode.first].insert(nowNode);
                    }
                }
            }

            // 若当前节点nowNode不能通过拓展后继link边找到cycle，则将与nowNode放入与nowNode有link边相连的adjacentNode的linkBlockList中，
            // 表示除非有adjacentNode可以走edge边找到cycle，否则nowNode就别走link边了
            if (! boolLinkCycle[nowNode]) {
                for(auto adjacentNode: edgeVal[nowNode]) {
                    if (! adjacentNode.second) {
                        linkBlockList[adjacentNode.first].insert(nowNode);
                    }
                }
            }

            // 若nowNode能通过拓展后继link边或edge边找到cycle，则需要递归解锁一系列节点，用BFS实现
            // 具体来说，若BFS队首元素unblockElement可以通过后继link边找到cycle，则将edgeBlockList[unblockElement]中的元素入队；
            // 若unblockElement可以通过后继edge边找到cycle，则将linkBlockList[unblockElement]中的元素入队
            if (boolLinkCycle[nowNode] || boolEdgeCycle[nowNode]) {
                std::queue <std::pair<int, int> > unblockList;
                std::unordered_map <int, int> queueVisit;

                if (boolLinkCycle[nowNode]) {
                    unblockList.push({nowNode, 0});
                    queueVisit[(nowNode << 1) - 1] = 1;
                }
                if (boolEdgeCycle[nowNode]) {
                    unblockList.push({nowNode, 1});
                    queueVisit[(nowNode << 1)] = 1;
                }
                
                while (! unblockList.empty()) {
                    int unblockElement = unblockList.front().first;
                    bool unblockCharacter = unblockList.front().second;
                    unblockList.pop();

                    if (unblockCharacter) {
                        edgeBlocked[unblockElement] = 0;
                        for (auto adjacentNode: linkBlockList[unblockElement]) {
                            if (! queueVisit[(adjacentNode << 1) - 1]) {
                                queueVisit[(adjacentNode << 1) - 1] = 1;
                                unblockList.push({adjacentNode, 0});
                            }
                        }
                        linkBlockList[unblockElement] = std::set<int> ();

                    } else {
                        linkBlocked[unblockElement] = 0;
                        for (auto adjacentNode: edgeBlockList[unblockElement]) {
                            if (! queueVisit[(adjacentNode << 1)]) {
                                queueVisit[(adjacentNode << 1)] = 1;
                                unblockList.push({adjacentNode, 1});
                            }
                        }
                        edgeBlockList[unblockElement] = std::set<int> ();
                    }
                }
                boolLinkCycle[nowNode] = 0;
                boolEdgeCycle[nowNode] = 0;
            }

            if (! path.empty()) {
                int preFinished = path.back();
                int edgeLength = edgeVal[preFinished].lower_bound({nowNode, 0})->second;

                totalLength -= vertexVal[nowNode];
                totalLength -= edgeLength;
                visited[nowNode] = startVertex;
            }
        }
    }
}


void Cycle::print2File(const std::string& outputFile, int loopCount) {
    std::ofstream output(outputFile);
    if (!output.is_open()) {
        std::cerr << "Error opening file: " << outputFile << std::endl;
        return;
    }

    for (auto cycle: cycleLen) {
        output << cycle.first << "\t" << cycle.second << "\n";
    }
    output.close();
    std::cerr << "--- The " << loopCount + cycleCount << " cycle(s) (including " 
        <<  loopCount << " loop(s)) in the digraph has been successfully exported to the file: " << outputFile << std::endl;
    cycleLen = std::unordered_map <long long, int> ();
}