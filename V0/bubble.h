#ifndef __BUBBLE_H__
#define __BUBBLE_H__


#include <unordered_map>


#include "graph.h"


class Bubble {
    public:
        Bubble();
        ~Bubble();

        void findBubble(const DiGraph&);
        void bfs(int s,int direction,const BiedgedGraph& biedgedGraph); 
        void findBubble(const BiedgedGraph&);
        void print2File(const std::string&);
        
        void bfs(int s,int direction,const BiedgedGraph& biedgedGraph,int is_dibigraph); 
        void findBubble(const BiedgedGraph&,int is_dibigraph);
        
    private:
        struct bubble{
            int s,t;
            int type; // 0 simple 1 super 2 insertion
            std::vector <int> inside;
            bool operator < (const bubble &ts) const{
                return s<ts.s;
            }
        };
        std::vector < std::vector <int> > supperBubble, simpleBubble, insertion;
        std::unordered_map <int, int> nestedBubble;
        std::vector <bubble> Bubbles;
};

#endif
