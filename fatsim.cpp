#include "fatsim.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

//Using the Breadth-First-Traversal, starting from the end of the file-path,
//determine the longest file-path.
std::vector<long> fat_check(const std::vector<long> & fat) {

  std::vector<std::vector<long>> adj_list(fat.size()+1);
  std::vector<long> terminating_blocks;

  for(int i = 0; i < (int)fat.size(); i++){
    if(fat.at(i) != -1){
      adj_list.at(fat.at(i)).push_back(i);
    }
    else{
      terminating_blocks.push_back(i);
    }
  }
  
  std::vector<long> chain_sizes;
  for(int i = 0; i < (int)terminating_blocks.size(); i++){
    std::list<long> queue;
    std::vector<bool> visited(adj_list.size());
    std::vector<long> level(adj_list.size());
    level.at(terminating_blocks.at(i)) = 1;
    queue.push_back(terminating_blocks.at(i));
    while(!queue.empty()){
      long curr = queue.front();
      queue.pop_front();
      for(int j = 0; j < (int)adj_list.at(curr).size(); j++){
        if(!visited.at(j)){
          queue.push_back(adj_list.at(curr).at(j));
          visited.at(j) = false;
          level.at(adj_list.at(curr).at(j)) = level.at(curr) + 1;
        }
      }
    }
    chain_sizes.push_back(*max_element(level.begin(), level.end()));
  }
  std::sort(chain_sizes.begin(), chain_sizes.end());

  return chain_sizes;
}