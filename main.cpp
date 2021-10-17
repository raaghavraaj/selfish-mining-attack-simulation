#include <bits/stdc++.h>
#include "node.h"
#include "utils.h"
using namespace std;
#define pp pair<double, event>
#define fi first
#define se second
#define ll unsigned long long
vector<int> invalid_block_ids;
void print_invalid_block_ids(){
	cout<<"Invalid block ids: ";
	for(auto b:invalid_block_ids){
		cout<<b<<" ";
	}
	cout<<endl;
}

/////////////////////  PARAMETERS ////////////////////////////
int num = 5; /// NUMBER OF PEERS
double tx = 10; // MEAN INTERARRIVAL TIME OF TXN (in msec)
double z = 50; // %AGE OF SLOW NODES
double zeta = 50; // %AGE OF HONEST NODES ADVERSARY IS CONNECTED TO
int seed = 0;
int adversary = 2; // 0TH PEER WILL BE ADVERSARY IF NOT EQUAL TO ZERO
				   // 1 -> SELFISH MINING ATTACK
				   // 2 -> STUBBORN MINING ATTACK
bool invalid = false; // 0TH PEER CAN GENERATE INVALID BLOCKS IF SET TO TRUE
double total_run_time = 25000; // TOTAL RUN TIME OF PROGRAM (in msec)
double mean_tk[1000]; // MEAN BLOCK CREATION TIME OF PEER (in msec)
int main()
{
	mean_tk[0] = 2500; // ADVERSARY MEAN TK
	for(int i=1;i<num;i++){
		mean_tk[i] = 2500;
	}
//////////////////////////////////////////////////////////////
	////////// INITIALISING THE SYSTEM STATE ///////////
	invalid_block_ids.clear();
	init_utils(seed);
	init_event(num, adversary);
	init_node(num, z, adversary);

	// TO MAKE IT SOMEWHAT DETERMINISTIC
	ll start = get_time_in_ms();
	while(get_time_in_ms()<start+1){}
	start++;
	
	////////// CREATE CONNECTED GRAPH /////////////////
	while(true){
		create_connected_graph();
		if (is_connected()){
			break;
		}
	}
	// CONNECTING THE ADVERSARY WITH ZETA% OF HONEST MINERS
	if(adversary > 0){
		connect_adversary(zeta);
	}
	/////// ADD START EVENTS TO EVENT-QUEUE ///////////
	multiset<pp> simulator;
	for (int i = 0; i < num; i++)
	{
		event e = create_send_txn_event(i);
		simulator.insert({get_expo_dist(tx), e});
		e = create_send_block_event(i);
		simulator.insert({get_expo_dist(mean_tk[i]), e});
	}
	while (!simulator.empty()){
		ll end = get_time_in_ms();
		////////// CHECK FOR SIMULATION COMPLETION ///////
		if(end-start>total_run_time){
			break;
		}
		pp top = (*simulator.begin());
		simulator.erase(simulator.find(top));
		double evt_time = top.fi;
		event evt = top.se;
		////////// WAIT FOR EVENT TIME /////////////
		while(get_time_in_ms()-start < evt_time){
		}
		if(evt.type == 0){
			if(evt.parent_node_id == -1){
				//// EVENT OF SENDING TXN ////
				int y = get_random_int(num-1);
				if(y >= evt.idx){
					y++;
				}
				int txn_id = create_txn(evt.idx, y);
				event e = create_send_txn_event(evt.idx);
				simulator.insert({get_expo_dist(tx) + evt_time, e});
				send_txn(txn_id,evt.idx,simulator,evt_time,evt.parent_node_id);
			}else{
				//// EVENT OF RECEIVING TXN ////
				if(!is_txn_received(evt.idx,evt.txn_id)){
					send_txn(evt.txn_id,evt.idx,simulator,evt_time,evt.parent_node_id);	
				}
			}
		}else{
			if(evt.parent_node_id == -1){
				//// EVENT OF SENDING BLOCK ////
				if(evt.block_id == get_root_block_id(evt.idx)){
					int block_id = create_block(evt.idx,evt.block_id);
					if(invalid && evt.idx == 0){
						///// ADD SUBSET OF TXNS (CAN CREATE INVALID BLOCK) ////
						if(add_invalid_txns_to_block(evt.idx,block_id)){
							invalid_block_ids.push_back(block_id);
						}
					}else{
						///// ADD SUBSET OF TXNS TO CREATE VALID BLOCK////
						add_txns_to_block(evt.idx,block_id);
					}
					send_block(block_id,evt.idx,simulator,evt_time,evt.parent_node_id,mean_tk[evt.idx]);
				}
			}else{
				//// EVENT OF RECEIVING BLOCK ////
				if(!is_block_received(evt.idx,evt.block_id) && is_block_valid(evt.block_id)){
					send_block(evt.block_id,evt.idx,simulator,evt_time,evt.parent_node_id,mean_tk[evt.idx]);	
				}	
			}
		}
	}
	/////// GIVE OUTPUT ////////// 
	if(invalid){
		print_invalid_block_ids();
	}
	int root_id = get_global_root_block_id();
	cout<<(1.0*blocks_of_peer_in_chain(0,root_id)/total_blocks_of_peer(0))<<endl;
	cout<<(1.0*get_length_chain(root_id)/total_blocks())<<endl;
	ofstream out_tree("tree.txt");
	cout.rdbuf(out_tree.rdbuf());
	print_tree();
	for(int i=0;i<num;i++){
		ofstream peer(to_string(i)+"_peer_info.txt");
		cout.rdbuf(peer.rdbuf());
		print_blocks_received(i);
	}
}