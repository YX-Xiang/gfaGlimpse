#include "bubble.h"
#include <iostream>
#include <queue>
#include <algorithm>

Bubble::Bubble() {
    supperBubble = std::vector < std::vector <int> > ();
    simpleBubble = std::vector < std::vector <int> > ();
	insertion =  std::vector < std::vector<int> > ();
    nestedBubble = std::unordered_map <int, int> ();
	Bubbles = std::vector <bubble> ();
}

Bubble::~Bubble() {}

int id(int x,int di){  //这里能这么写吗
	return x*2-(1-di);
}


void Bubble::findBubble(const DiGraph& diGraph) {
    // 统计supperBubble，simpleBubble，nestedBubble和chainLen
    // T. B. C.
}

// now is bidirected graph
bool is_simple(int di,std::vector<int> &in,const BiedgedGraph& biedgedGraph){
	int s,t,x,y;
	s=in[0];
	t=in[3];
	x=in[1];
	y=in[2];
	if(biedgedGraph.edge[id(x,0)].size()==2 && biedgedGraph.edge[id(x,1)].size()==2 && biedgedGraph.edge[id(y,0)].size()==2 && biedgedGraph.edge[id(y,1)].size()==2 ) ;
	else {
		return false;
	}


	std::set <int> neis,neit;
	for(auto [v,val]:biedgedGraph.edge[id(s,0)]) if(val==0) neis.insert((v+1)/2);
	for(auto [v,val]:biedgedGraph.edge[id(s,1)]) if(val==0) neis.insert((v+1)/2);
	
	for(auto [v,val]:biedgedGraph.edge[id(t,0)]) if(val==0) neit.insert((v+1)/2);
	for(auto [v,val]:biedgedGraph.edge[id(t,1)]) if(val==0) neit.insert((v+1)/2);
	
	if(neis.count(t) || neit.count(s)) return false; // 这种情况是有的 不加这句的话 会有一些super的变成simple，
	// if(edges[s].count(t) || edges[t].count(s)) return 0;
	
	// return 1;
	std::vector <int> neignbor1,neignbor2;
	for(auto [v,val]:biedgedGraph.edge[id(x,0)]) if(val==0) neignbor1.push_back(v);
	for(auto [v,val]:biedgedGraph.edge[id(x,1)]) if(val==0) neignbor1.push_back(v);
	
	for(auto [v,val]:biedgedGraph.edge[id(y,0)]) if(val==0) neignbor2.push_back(v);
	for(auto [v,val]:biedgedGraph.edge[id(y,1)]) if(val==0) neignbor2.push_back(v);
	
	std::sort(neignbor1.begin(),neignbor1.end());
	std::sort(neignbor2.begin(),neignbor2.end());
	if(neignbor1!=neignbor2)  return false;

	return 1;
}

bool is_insertion(int di,std::vector<int> &in,const BiedgedGraph& biedgedGraph){
	// 可能差点细节（目前暂时没问题） 但没想到三个点的特殊情况还有什么 要么对比源码
	return 1;
}

void Bubble::bfs(int s,int direction,const BiedgedGraph& biedgedGraph){
    std::set <std::pair<int,int>> seen;
	std::set <int> visited;
	std::vector <int> in;
	seen.insert({s,direction}); // s /2
 
	std::queue <std::pair<int,int>> q;
	q.push({s,direction});  // s /2

	while(q.size()){
		int u=q.front().first;
		int di=q.front().second;
        int idu=id(u,di);
        // std::cerr<<"id:"<<idu<<std::endl;
		q.pop();
		visited.insert(u);
		in.push_back(u);
		// std::cerr<<seen.count({idu,di})<<std::endl;
		seen.erase(seen.find({u,di}));

		if(biedgedGraph.edge[idu].size()==1){  //这里要改成1 因为每个点一定有一条黑边相连，如果度为1的话，说明没有其他灰边，才是tip  
												//!!重要提醒:代码里面和度有关的一定要考虑黑边加1的度
			// its a tip
			return ;
		}

		for(auto &[to,val]:biedgedGraph.edge[idu]) {
			if(val!=0) continue; //only gray edges
			int nowdi=!(to&1);

			if((to+1)/2==s){
				// its a loop
				return ;
			}
			std::vector <int> to_parent;
			for(auto &[fa,faV]:biedgedGraph.edge[to]) if(faV==0){
				to_parent.push_back((fa+1)/2);
			}
			seen.insert({(to+1)/2,1-nowdi});

			bool is_all_visited=1;
			for(auto &fa:to_parent){
				if(!visited.count(fa)) is_all_visited=0;
			}
			if(is_all_visited) q.push({(to+1)/2,1-nowdi});
		}

		if(q.size()==1 && seen.size()==1){
			int t=q.front().first;
			int nowdi=q.front().second;
			q.pop();
			in.push_back(t);
			int type;

			if(in.size()==2) {
				return ; // its a empty bubble
			}
			else if(in.size()==3){
				if(is_insertion(di,in,biedgedGraph)) {
					insertion.push_back(in);
					type=2;
				}
				else {
					supperBubble.push_back(in);
					type=1;
				}
			}
			else if(in.size()==4){
				if(is_simple(di,in,biedgedGraph)) {
					simpleBubble.push_back(in);
					type=0;
				}
				else {
					supperBubble.push_back(in);
					type=1;
				}
			}
			else{
				supperBubble.push_back(in);
				type=1;
			}
			Bubbles.push_back({s,t,type,in});
			return ;
		}
	}
}


void Bubble::findBubble(const BiedgedGraph& biedgedGraph) {
    // 统计supperBubble，simpleBubble，nestedBubble和chainLen
    // T. B. C.
	
    int n=biedgedGraph.edge.size()/2;
    for(int i=2;i<=n*2;i+=2){
		//  std::cerr<<"i:"<<i<<std::endl;
		
        bfs((i+1)/2,1-i&1,biedgedGraph);
    }

	sort(Bubbles.begin(),Bubbles.end());

	long long maxdp=0,maxdpp=0;
	std::vector <long long> dp(n+1),dpp(n+1),cnt(n+1);
	std::vector <bool> is_bubble_s(n+1);

	for(auto &p:Bubbles){
		int x=p.s;
		int y=p.t;

		is_bubble_s[x]=1;

		dp[y]=dp[x]+1;
		maxdp=std::max(maxdp,dp[y]);

		int sum=0;
		for(int i=1;i<p.inside.size();i++){
			cnt[p.inside[i]]++;
            sum+=(*biedgedGraph.edge[p.inside[i]*2].begin()).value;  //加bubble 里的点的大小
	  	}
		if(dp[x]==0){
			sum+=(*biedgedGraph.edge[x*2].begin()).value; //加这个bubble 里面s的大小
		}
		if(dp[x]==0) cnt[p.inside[0]]++;
		dpp[y]=dpp[x]+sum;
		maxdpp=std::max(maxdpp,dpp[y]);
	}

	std::map <int,int> bubble_siz;
	int tot_node=0,tot_seq=0,all_seqlen=0;
	for(int i=1;i<=n;i++){
		all_seqlen+=(*biedgedGraph.edge[i*2].begin()).value;
		if(cnt[i]){
			tot_node++;
			tot_seq+=(*biedgedGraph.edge[i*2].begin()).value;
		}
		if(is_bubble_s[i]){
			bubble_siz[cnt[i]]++;
		}
	}

	std::cerr<<"This is bidirected graph:\n";
	std::cerr<<"Sequence coverage of the bubble chains is "<<100.0*tot_seq/all_seqlen<<"%"<<std::endl;
	std::cerr<<"Node coverage of the bubble chains is "<<100.0*tot_node/n<<"%"<<std::endl;
	std::cerr<<"The longest chain seq-wise has "<<maxdpp<<" bp"<<std::endl;
	std::cerr<<"The longest chain bubble-wise has "<<maxdp<<" bubbles"<<std::endl;

	std::cerr<<"Bubble nested level:"<<std::endl;
	for(auto &[x,y]:bubble_siz){
		std::cerr<<x<<" "<<y<<std::endl;
	}
	std::cerr<<std::endl;


	std::cerr<<"total num: "<<Bubbles.size()<<std::endl;
	std::cerr<<"simple:    "<<simpleBubble.size()<<std::endl;
	std::cerr<<"super:     "<<supperBubble.size()<<std::endl;
	std::cerr<<"insertion: "<<insertion.size()<<std::endl;

}


// now is directed biedged graph
bool is_simple(int di,std::vector<int> &in,const BiedgedGraph& biedgedGraph,int is_dibigraph){
	int s,t,x,y;
	s=in[0];
	t=in[3];
	x=in[1];
	y=in[2];
	if(biedgedGraph.edge[id(x,0)].size()==1 && biedgedGraph.edge[id(x,1)].size()==1 && biedgedGraph.edge[id(y,0)].size()==1 && biedgedGraph.edge[id(y,1)].size()==1 ) ;
	else {
		return false;
	}

	std::set <int> neis,neit;
	// for(auto [v,val]:biedgedGraph.edge[id(s,0)]) if(val==0) neis.insert((v+1)/2);
	for(auto [v,val]:biedgedGraph.edge[id(s,1)]) if(val==0) neis.insert((v+1)/2);
	
	for(auto [v,val]:biedgedGraph.edge[id(t,0)]) if(val==0) neit.insert((v+1)/2);
	// for(auto [v,val]:biedgedGraph.edge[id(t,1)]) if(val==0) neit.insert((v+1)/2);
	
	if(neis.count(t) || neit.count(s)) return false; // 这种情况是有的 不加这句的话 会有一些super的变成simple，
	// if(edges[s].count(t) || edges[t].count(s)) return 0;
	
	// return 1;
	std::vector <int> neignbor1,neignbor2;
	for(auto [v,val]:biedgedGraph.edge[id(x,0)]) if(val==0) neignbor1.push_back(v);
	for(auto [v,val]:biedgedGraph.edge[id(x,1)]) if(val==0) neignbor1.push_back(v);
	
	for(auto [v,val]:biedgedGraph.edge[id(y,0)]) if(val==0) neignbor2.push_back(v);
	for(auto [v,val]:biedgedGraph.edge[id(y,1)]) if(val==0) neignbor2.push_back(v);
	
	std::sort(neignbor1.begin(),neignbor1.end());
	std::sort(neignbor2.begin(),neignbor2.end());
	if(neignbor1!=neignbor2)  return false;

	return 1;
}

void Bubble::bfs(int s,int direction,const BiedgedGraph& biedgedGraph,int is_dibigraph){
    std::set <std::pair<int,int>> seen;
	std::set <int> visited;
	std::vector <int> in;
	seen.insert({s,direction}); // s /2
 
	std::queue <std::pair<int,int>> q;
	q.push({s,direction});  // s /2

	while(q.size()){
		int u=q.front().first;
		int di=q.front().second;
        int idu=id(u,di);
        // std::cerr<<"id:"<<u<<std::endl;
        
		q.pop();
		visited.insert(u);
		in.push_back(u);

        // for(auto i:visited){
            // std::cerr<<i<<" ";
        // }
        // std::cerr<<std::endl;
		// std::cerr<<seen.count({idu,di})<<std::endl;
		seen.erase(seen.find({u,di}));

		if(biedgedGraph.edge[idu].size()==0 || (biedgedGraph.edge[idu].size()==1 && biedgedGraph.edge[idu].begin()->value!=0)){  //这里要改成1 因为每个点一定有一条黑边相连，如果度为1的话，说明没有其他灰边，才是tip  
												//!!重要提醒:代码里面和度有关的一定要考虑黑边加1的度
			// its a tip
			return ;
		}

		for(auto &[to,val]:biedgedGraph.edge[idu]) {
			if(val!=0) continue; //only gray edges
			int nowdi=!(to&1);

			if((to+1)/2==s){
				// its a loop
				return ;
			}
			std::vector <int> to_parent;
			for(auto &[fa,faV]:biedgedGraph.redge[to]) if(faV==0){
                // std::cerr<<"fa: "<<to<<" "<<fa<<std::endl;
				to_parent.push_back((fa+1)/2);
			}
			seen.insert({(to+1)/2,1-nowdi});
            
			bool is_all_visited=1;
			for(auto &fa:to_parent){
                // if(to==15){
                //     std::cerr<<fa<<" ";
                // }
				if(!visited.count(fa)) is_all_visited=0;
			}
			if(is_all_visited) q.push({(to+1)/2,1-nowdi});
		}

		if(q.size()==1 && seen.size()==1){
			int t=q.front().first;
			int nowdi=q.front().second;
			q.pop();
			in.push_back(t);
			int type;

			if(in.size()==2) {
				return ; // its a empty bubble
			}
			else if(in.size()==3){
				if(is_insertion(di,in,biedgedGraph)) {
					insertion.push_back(in);
					type=2;
				}
				else {
					supperBubble.push_back(in);
					type=1;
				}
			}
			else if(in.size()==4){
				if(is_simple(di,in,biedgedGraph,1)) {
					simpleBubble.push_back(in);
					type=0;
				}
				else {
					supperBubble.push_back(in);
					type=1;
				}
			}
			else{
				supperBubble.push_back(in);
				type=1;
			}
			if(t<s){
				std::reverse(in.begin(),in.end());
			}
			Bubbles.push_back({std::min(s,t),std::max(s,t),type,in});
			return ;
		}
	}
}

void Bubble::findBubble(const BiedgedGraph& biedgedGraph,int is_dibigraph) {
    // 统计supperBubble，simpleBubble，nestedBubble和chainLen
    // T. B. C.
	
    int n=biedgedGraph.edge.size()/2;

    for(int i=1;i<=n*2;i++){
		//  std::cerr<<"i:"<<i<<std::endl;
		
        bfs((i+1)/2,1-i&1,biedgedGraph,1);
    }
    // std::cerr<<cnt<<std::endl;
	sort(Bubbles.begin(),Bubbles.end());

	long long maxdp=0,maxdpp=0;
	std::vector <long long> dp(n+1),dpp(n+1),cnt(n+1);
	std::vector <bool> is_bubble_s(n+1);

	for(auto &p:Bubbles){
		int x=p.s;
		int y=p.t;
		is_bubble_s[x]=1;

		dp[y]=dp[x]+1;
		maxdp=std::max(maxdp,dp[y]);

		int sum=0;
		for(int i=1;i<p.inside.size();i++){
			cnt[p.inside[i]]++;
			if(biedgedGraph.edge[p.inside[i]*2-1].size() && biedgedGraph.edge[p.inside[i]*2-1].begin()->value!=0){
				sum+=biedgedGraph.edge[p.inside[i]*2-1].begin()->value;
			}
            else if(biedgedGraph.edge[p.inside[i]*2].size() && biedgedGraph.edge[p.inside[i]*2].begin()->value!=0){
				sum+=biedgedGraph.edge[p.inside[i]*2].begin()->value;
			}
            
	  	}
		if(dp[x]==0){
			if(biedgedGraph.edge[x*2-1].size() && biedgedGraph.edge[x*2-1].begin()->value!=0){
				sum+=biedgedGraph.edge[x*2-1].begin()->value;
			}
            else if(biedgedGraph.edge[x*2].size() && biedgedGraph.edge[x*2].begin()->value!=0){
				sum+=biedgedGraph.edge[x*2].begin()->value;
			}
		}
		if(dp[x]==0) cnt[p.inside[0]]++;
		dpp[y]=dpp[x]+sum;
		maxdpp=std::max(maxdpp,dpp[y]);
	}
	
	std::map <int,int> bubble_siz;
	int tot_node=0,tot_seq=0,all_seqlen=0;
	for(int i=1;i<=n;i++){
		if(biedgedGraph.edge[i*2-1].size() && biedgedGraph.edge[i*2-1].begin()->value!=0){
			all_seqlen+=biedgedGraph.edge[i*2-1].begin()->value;
		}
		else if(biedgedGraph.edge[i*2].size() && biedgedGraph.edge[i*2].begin()->value!=0){
			all_seqlen+=biedgedGraph.edge[i*2].begin()->value;
		}

		if(cnt[i]){
			tot_node++;
			if(biedgedGraph.edge[i*2-1].size() && biedgedGraph.edge[i*2-1].begin()->value!=0){
				tot_seq+=biedgedGraph.edge[i*2-1].begin()->value;
			}
			else if(biedgedGraph.edge[i*2].size() && biedgedGraph.edge[i*2].begin()->value!=0){
				tot_seq+=biedgedGraph.edge[i*2].begin()->value;
			}
			// tot_seq+= 这个点的大小
		}
		if(is_bubble_s[i]){
			bubble_siz[cnt[i]]++;
		}
	}
	std::cerr<<"This is directed biedged graph:\n";
	std::cerr<<"Sequence coverage of the bubble chains is "<<100.0*tot_seq/all_seqlen<<"%"<<std::endl;
	std::cerr<<"Node coverage of the bubble chains is "<<100.0*tot_node/n<<"%"<<std::endl;
	std::cerr<<"The longest chain seq-wise has "<<maxdpp<<" bp"<<std::endl;
	std::cerr<<"The longest chain bubble-wise has "<<maxdp<<" bubbles"<<std::endl;

	std::cerr<<"Bubble nested level:"<<std::endl;
	for(auto &[x,y]:bubble_siz){
		std::cerr<<x<<" "<<y<<std::endl;
	}
	std::cerr<<std::endl;


	std::cerr<<"total num: "<<Bubbles.size()<<std::endl;
	std::cerr<<"simple:    "<<simpleBubble.size()<<std::endl;
	std::cerr<<"super:     "<<supperBubble.size()<<std::endl;
	std::cerr<<"insertion: "<<insertion.size()<<std::endl;
}


void Bubble::print2File(const std::string& outputFile) {
    // T. B. C.
}
