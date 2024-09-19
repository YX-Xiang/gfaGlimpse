#include "connectivity.h"


Connectivity::Connectivity(long long vertexNumber) {
    nodeNumber = vertexNumber;
    bridgeCount = 0;
    cutPointCount = 0;
    sccCount = 0;
    wccCount = 0;
    vertexMap = std::vector <long long> (1,0);
}


Connectivity::~Connectivity() {}


void Connectivity::initGraph() {
    sccNumber = 0;
    dfsNumber = 0;
    graph = std::vector < std::set <long long> > (nodeNumber + 1, std::set <long long> ());
    isCut = std::vector <bool> (nodeNumber + 1, 0);
    bridge = std::set <std::pair<long long, long long>> ();
    visNum = std::vector <long long> (nodeNumber + 1, 0);
    low = std::vector <long long> (nodeNumber + 1, 0);
    sccNum = std::vector <long long> (nodeNumber + 1, 0);
    scc = std::stack <long long> ();
    sccList = std::vector < std::vector <long long> > ();
}


// mode为0表示判断割边和割点，为1表示找scc
void Connectivity::tarjan(long long fiNode, int mode) {
    int child = 0;
    std::map < std::pair<long long, long long>, bool> visEdge;
    std::unordered_map<long long, long long> fa;
    visNum[fiNode] = low[fiNode] = ++ dfsNumber;
    scc.push(fiNode);

    // 手动模拟dfs栈
    std::stack <long long> dfsStack = std::stack <long long> ();
    dfsStack.push(fiNode);

    while (!dfsStack.empty()) {
        long long nowNode = dfsStack.top();

        for (auto nxtNode: graph[nowNode]) {
            if (! visNum[nxtNode]) {
                if (nowNode == fiNode) {
                    child ++;
                }
                visEdge[{nowNode, nxtNode}] = 1;
                fa[nxtNode] = nowNode;
                visNum[nxtNode] = low[nxtNode] = ++ dfsNumber;
                scc.push(nxtNode);
                dfsStack.push(nxtNode);
                break;
            }
        }

        if (nowNode == dfsStack.top()) {
            for (auto nxtNode: graph[nowNode]) {
                if (visEdge[{nowNode, nxtNode}]) {
                    low[nowNode] = std::min(low[nowNode], low[nxtNode]);
                    if (mode == 0) {
                        if (low[nxtNode] > visNum[nowNode]) {
                            bridge.insert({std::min(nowNode, nxtNode), std::max(nowNode, nxtNode)});
                        }
                        if (low[nxtNode] >= visNum[nowNode] && nowNode != fiNode) {
                            isCut[nowNode] = 1;
                        }
                    }
                } else {
                    if (mode == 0) {
                        if (visNum[nxtNode] < visNum[nowNode] && nxtNode != fa[nowNode]) {
                            low[nowNode] = std::min(low[nowNode], visNum[nxtNode]);
                        }
                    } else {
                        if (!sccNum[nxtNode]) {
                            low[nowNode] = std::min(low[nowNode], visNum[nxtNode]);
                        }
                    }
                }
            }

            if (visNum[nowNode] == low[nowNode]) {
                std::vector <long long> tmp = std::vector <long long> ();
                sccNumber ++;
                while (! scc.empty()) {
                    long long sccNode = scc.top();
                    scc.pop();
                    sccNum[sccNode] = sccNumber;
                    tmp.push_back(sccNode);
                    if(sccNode == nowNode) break;
                }
                sccList.push_back(tmp);
            }
            dfsStack.pop();
        }
    }
    if (child >= 2) {
        isCut[fiNode] = 1;
    }
}


void Connectivity::findComponent(const BiedgedGraph& biedgedGraph) {
    // 求 bidirected graph 中的割点（需要缩点）
    initGraph();
    for (long long vID = 1; vID <= nodeNumber; vID ++) {
        for (auto e: biedgedGraph.edge[vID]) {
            long long nxtNode = e.to;
            graph[((vID + 1) >> 1)].insert(((nxtNode + 1) >> 1));
        }
    }
    for (long long vID = 1; vID <= (nodeNumber >> 1); vID ++) {
        if (! visNum[vID]) {
            tarjan(vID, 0);
        }
        if (isCut[vID]) {
            cutPointCount ++;
        }
    }
    ccCount = sccList.size();

    // 求 biedged graph 中的桥
    initGraph();
    for (long long vID = 1; vID <= nodeNumber; vID ++) {
        for (auto e: biedgedGraph.edge[vID]) {
            long long nxtNode = e.to;
            graph[vID].insert(nxtNode);
        }
    }

    for (long long vID = 1; vID <= nodeNumber; vID ++) {
        if (! visNum[vID]) {
            tarjan(vID, 0);
        }
    }
    bridgeCount = bridge.size();

    // 求 biedged graph 中的连通分量
    initGraph();
    for (int vID = 1; vID <= nodeNumber; vID ++) {
        for (auto e: biedgedGraph.edge[vID]) {
            int nxtNode = e.to;
            graph[vID].insert(nxtNode);
        }
    }
    for (int vID = 1; vID <= nodeNumber; vID ++) {
        if (! visNum[vID]) {
            tarjan(vID, 1);
        }
    }
}


void Connectivity::edgeCompress(const DiGraph& diGraph) {
    reVertexMap = std::vector <long long> ((nodeNumber << 1) + 1, 0);
    long long newVertexID = 1;

    for (auto vertex: diGraph.vertexVal) {
        long long vID = vertex.first;
        vertexMap.push_back(vID);
        reVertexMap[vID] = newVertexID ++;
    }
}


void Connectivity::findWCC(const DiGraph& diGraph) {
    initGraph();
    for (auto vertex: diGraph.vertexVal) {
        long long vID = vertex.first;
        for (auto e: diGraph.edge[vID]) {
            graph[reVertexMap[vID]].insert(reVertexMap[e]);
            graph[reVertexMap[e]].insert(reVertexMap[vID]);
        }
    }

    for (long long vID = 1; vID <= nodeNumber; vID ++) {
        if (! visNum[vID]) {
            tarjan(vID, 1);
        }
    }
    wccCount = sccList.size();
}


void Connectivity::findSCC(const DiGraph& diGraph) {
    initGraph();
    for (auto vertex: diGraph.vertexVal) {
        long long vID = vertex.first;
        for (auto e: diGraph.edge[vID]) {
            graph[reVertexMap[vID]].insert(reVertexMap[e]);
        }
    }

    for (long long vID = 1; vID <= nodeNumber; vID ++) {
        if (! visNum[vID]) {
            tarjan(vID, 1);
        }
    }
    sccCount = sccList.size();
}


void Connectivity::SCC2Subgraph(const DiGraph& diGraph, std::vector <DiGraph>& subgraphList) {
    for (auto SCC: sccList) {
        if (SCC.size() == 1) continue;

        DiGraph subGraph = DiGraph();
        subGraph.edge = std::vector <std::set <long long> > (SCC.size() + 1, std::set <long long>());
        std::map <long long, long long> nodeId = std::map <long long, long long>();
        long long id = 0;

        for (auto node: SCC) {
            long long initNode = vertexMap[node];
            if (! diGraph.edge[initNode].empty()) {
                if (! nodeId.count(initNode)) {
                    nodeId[initNode] = ++ id;
                    subGraph.vertexVal[nodeId[initNode]] = diGraph.vertexVal.at(initNode);
                    subGraph.map2RawGraph[nodeId[initNode]] = initNode;
                }

                for (auto nxtNode: diGraph.edge[initNode]) {
                    long long nxtNodeMap = reVertexMap[nxtNode];
                    if (sccNum[nxtNodeMap] == sccNum[node]) {
                        if (! nodeId.count(nxtNode)) {
                            nodeId[nxtNode] = ++ id;
                            subGraph.vertexVal[nodeId[nxtNode]] = diGraph.vertexVal.at(nxtNode);
                            subGraph.map2RawGraph[nodeId[nxtNode]] = nxtNode;
                        }

                        subGraph.edge[nodeId[initNode]].insert(nodeId[nxtNode]);
                    }
                }
            }
        }
        subGraph.vertexNumber = id;
        subgraphList.push_back(subGraph);
    }
}


void Connectivity::SCC2Subgraph(const BiedgedGraph& biedgedGraph, std::vector <BiedgedGraph>& subgraphList) {
    for (auto SCC: sccList) {
        if (SCC.size() == 1) continue;

        BiedgedGraph subGraph = BiedgedGraph();
        subGraph.edge = std::vector <std::set <Biedge> > (SCC.size() + 1, std::set <Biedge>());
        std::map <long long, long long> nodeId = std::map <long long, long long>();
        long long id = 0;

        for (auto node: SCC) {
            long long initNode = node;
            if (! biedgedGraph.edge[initNode].empty()) {
                if (! nodeId.count(initNode)) {
                    nodeId[initNode] = ++ id;
                    subGraph.map2RawGraph[nodeId[initNode]] = initNode;
                }

                for (auto nxt: biedgedGraph.edge[initNode]) {
                    long long nxtNode = nxt.to;
                    if (sccNum[nxtNode] == sccNum[node]) {
                        if (! nodeId.count(nxtNode)) {
                            nodeId[nxtNode] = ++ id;
                            subGraph.map2RawGraph[nodeId[nxtNode]] = nxtNode;
                        }

                        Biedge newEdge = Biedge(nodeId[nxtNode], nxt.value);
                        subGraph.edge[nodeId[initNode]].insert(newEdge);
                    }
                }
            }
        }
        subGraph.vertexNumber = id;
        subgraphList.push_back(subGraph);
    }
}


void Connectivity::print2File(const std::string& outputFile) {
    // T. B. C.

}