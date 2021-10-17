#include <bits/stdc++.h>
using namespace std;
#define pp pair<double, event>
#define fi first
#define se second
struct txn{
	int id;
	int idx; // -1 for coinbase txn
	int idy;
	int c;
};
struct block{
	int id;
	int parent_id; // parent block id
	int idx; // peer who created the block
	int chain_length;
	vector<int> txn_ids;
};
struct event
{
	int parent_node_id; // -1 for send event
	int type;	// 0 txn, 1 block
	int idx;
	int block_id;
	int txn_id;
	bool operator<(const event &e) const{
		return tie(idx) < tie(e.idx);
	}
};
void init_event(int n, int adversary);
int create_txn(int x, int y);
int create_block(int x, int par);
event create_send_txn_event(int x);
event create_send_block_event(int x);
void send_txn(int txn_id,int x,multiset<pp> &simulator,double start_time,int par_node_id);
void add_txns_to_block(int x,int block_id);
bool add_invalid_txns_to_block(int x,int block_id);
bool is_block_valid(int block_id);
void send_block(int block_id,int x,multiset<pp> &simulator,double start_time,int par_node_id,int mean_tk);
void print_tree();
int total_blocks_of_peer(int x);
int blocks_of_peer_in_chain(int x,int root_id);
int get_global_root_block_id();
void print_blocks_received(int x);
double get_branching_in_block_tree();
double get_avg_txns_in_longest_chain();
vector<int> get_branches_length();
int get_length_chain(int root_id);
int total_blocks();