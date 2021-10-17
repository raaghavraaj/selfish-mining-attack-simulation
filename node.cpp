#include <bits/stdc++.h>
#include "node.h"
#include "utils.h"
using namespace std;
const int max_num = 1000;
node peers_array[max_num];
edge peers_edges[max_num][max_num];
int num1;
int attack; // 1 -> mining attack by 0th peer 
double prob_choosing_edge; // STORES THE PROBABILITY OF CHOOSING EDGE 
///// INITIALISES NODE DATA STRUCTURES /////
void init_node(int n, double z,int adversary)
{
	num1 = n;
	prob_choosing_edge = 3 * log(num1) / num1;
	int genesis_id = create_block(-1, -1);
	if(adversary > 0){
		attack = 1;
	}
	for (int i = 0; i < num1; i++)
	{
		double random_0to1 = get_uniform_0to1();
		peers_array[i].speed = random_0to1 > (z / 100);
		peers_array[i].root_id = genesis_id;
		peers_array[i].txn_ids.clear();
		peers_array[i].block_ids.clear();
		peers_array[i].time_arrival.clear();
		receive_block(i, genesis_id, 0);
	}
	if(attack){
		peers_array[0].speed = 1;
	}
}
///// CREATES CONNECTED GRAPH ///////////
void create_connected_graph()
{
	for (int i = attack; i < num1; i++)
	{
		for (int j = i + 1; j < num1; j++)
		{
			double random_0to1 = get_uniform_0to1();
			if (random_0to1 > prob_choosing_edge)
			{
				// EDGE IS NOT ADDED
				peers_edges[i][j].present = 0;
				peers_edges[j][i].present = 0;
			}
			else
			{
				// EDGE IS ADDED
				peers_edges[i][j].present = 1;
				peers_edges[i][j].p = get_uniform_0to1() * 490 + 10;
				if (peers_array[i].speed == 1 && peers_array[j].speed == 1)
				{
					peers_edges[i][j].c = 102.4;
				}
				else
				{
					peers_edges[i][j].c = 5.12;
				}
				peers_edges[j][i] = peers_edges[i][j];
			}
		}
	}
}
int visited[max_num];
void dfs(int root)
{
	for (int i = attack; i < num1; i++)
	{
		if (!visited[i] && peers_edges[root][i].present)
		{
			visited[i] = 1;
			dfs(i);
		}
	}
}
////////// CHECKS IF GRAPH IS CONNECTED OR NOT ////////
bool is_connected()
{
	for (int i = attack; i < num1; i++)
	{
		visited[i] = 0;
	}
	visited[attack] = 1;
	dfs(attack);
	for (int i = attack; i < num1; i++)
	{
		if (!visited[i])
		{
			return false;
		}
	}
	return true;
}
void connect_adversary(double zeta){
	int cnt = 0;
	for(int i=1;i<num1;i++){
		double random_0to1 = get_uniform_0to1();
		if (random_0to1 * 100 > zeta)
		{
			// EDGE IS NOT ADDED
			peers_edges[i][0].present = 0;
			peers_edges[0][i].present = 0;
		}
		else
		{
			// EDGE IS ADDED
			cnt++;
			peers_edges[i][0].present = 1;
			peers_edges[i][0].p = get_uniform_0to1() * 490 + 10;
			if (peers_array[i].speed == 1 && peers_array[0].speed == 1)
			{
				peers_edges[i][0].c = 102.4;
			}
			else
			{
				peers_edges[i][0].c = 5.12;
			}
			peers_edges[0][i] = peers_edges[i][0];
		}	
	}
	if(cnt == 0){
		// IN CASE NO PEER IS CONNECTED, WE MANUALLY CONNECT ONE
		// PEER TO KEEP THE GRAPH CONNECTED 
		int i = get_random_int(num1-1)+1;
		peers_edges[i][0].present = 1;
		peers_edges[i][0].p = get_uniform_0to1() * 490 + 10;
		if (peers_array[i].speed == 1 && peers_array[0].speed == 1)
		{
			peers_edges[i][0].c = 102.4;
		}
		else
		{
			peers_edges[i][0].c = 5.12;
		}
		peers_edges[0][i] = peers_edges[i][0];
	}
}
////// RETURNS ROOT BLOCK ID OF A PEER //////
int get_root_block_id(int x)
{
	return peers_array[x].root_id;
}
////// UPDATES ROOT BLOCK ID OF A PEER //////
void update_root_block_id(int x, int block_id)
{
	peers_array[x].root_id = block_id;
}
////// RETURNS TXNS RECEIVED BY A PEER //////
vector<int> get_peer_txn_ids(int x)
{
	return peers_array[x].txn_ids;
}
////// PEER RECEIVES A TXN //////
void receive_txn(int x, int txn_id)
{
	peers_array[x].txn_ids.push_back(txn_id);
}
////// CHECKS IF A TXN IS RECEIVED OR NOT //////
bool is_txn_received(int x, int txn_id)
{
	for (auto tid : peers_array[x].txn_ids)
	{
		if (tid == txn_id)
		{
			return true;
		}
	}
	return false;
}
/////// PEER RECEIVES A BLOCK ////////////
void receive_block(int x, int block_id, double time)
{
	peers_array[x].block_ids.push_back(block_id);
	peers_array[x].time_arrival.push_back(time);
}
////// CHECKS IF A BLOCK IS RECEIVED OR NOT //////
bool is_block_received(int x, int block_id)
{
	for (auto bid : peers_array[x].block_ids)
	{
		if (bid == block_id)
		{
			return true;
		}
	}
	return false;
}
edge get_edge(int x, int y)
{
	return peers_edges[x][y];
}
vector<int> get_peer_block_ids(int x)
{
	return peers_array[x].block_ids;
}
vector<double> get_peer_time_arrivals(int x)
{
	return peers_array[x].time_arrival;	
}
int get_speed_of_peer(int x)
{
	return peers_array[x].speed;
}