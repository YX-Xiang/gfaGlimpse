# graph.h 
**定义图的结构**
- digraph
- biedged graph

# gfa.h 
**读入gfa文件，并转换为定义的图结构**
- gfa
    - read
    - stat $\begin{cases} \text{file size} \\ \text{seg} \\ \text{link} \\ \text{path} \end{cases}$
    - to_graph

# vertex.h
**计算图上点相关的统计量**
- vertex
    - get_vertex_number
    - get_vertex_weight (node size, total length, N50, L50)
- degree
    - get_distribution
    - get_dead_end
- coverage
- growth

# edge.h
**计算图上边相关的统计量**
- edge
    - get_edge_number
    - get_loop
- cycle
    - get_cycle_num
    - get_cycle_length

# connectivity.h
**计算图上连通性相关的统计量**
- connectivity
    - get_cut_vertex
    - get_bridge
    - get_weak_component
    - get_scc

# bubble.h
