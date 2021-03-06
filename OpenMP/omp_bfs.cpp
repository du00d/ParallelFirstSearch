#include "omp_bfs.h"
#include <iostream>
#include <list>
#include <set>
#include <algorithm>
void bfs_omp(const adjacencyList &G, bfs_node origin,void (*action)(bfs_node),int NUM_THREADS){
    omp_set_num_threads(NUM_THREADS);
    std::set<bfs_node> q;
    std::vector<bool>seen(G.size(),false);
    
    q.insert(origin);
    seen[origin]=true;

    while(!q.empty()){
        std::list<bfs_node> newq[NUM_THREADS];
        std::vector<bfs_node> qnodes;
        for(auto i = q.begin();i!=q.end();i++)
            qnodes.push_back(*i);
        
        #pragma omp parallel for shared(seen)
        for(int k = 0;k<qnodes.size();k++){
            bfs_node node = qnodes[k];
            
            if(action!=NULL)
                action(node);
            for(auto i = G.at(node).begin();i!=G.at(node).end();i++){
                bfs_node neighbor = *i;
                if(seen[neighbor]==false){
                    // newq[omp_get_thread_num()].insert(neighbor);
                    newq[omp_get_thread_num()].push_back(neighbor);
                    seen[neighbor]=true;
                }
            }
            
        }
        
        q.clear();
        for(int i = 0;i<NUM_THREADS;i++)
            for(bfs_node node = newq[i].front();!newq[i].empty();newq[i].pop_front(),node = newq[i].front())
                q.insert(node);
    }
}