#include "events.h"
struct edge
{
	int present; // 0 DNE, 1 exist
	double p;
	double c;
};
struct node
{
	int speed;			   // 0 slow, 1 fast
	int root_id;		   // root block id
	vector<int> txn_ids;   // txn ids received
	vector<int> block_ids; // block ids received
	vector<double> time_arrival; // arrival time corresponding to block ids
};
void init_node(int n, double z, int adversary);
void create_connected_graph();
bool is_connected();
void connect_adversary(double zeta);
int get_root_block_id(int x);
void update_root_block_id(int x, int block_id);
void receive_txn(int x, int txn_id);
bool is_txn_received(int x, int txn_id);
void receive_block(int x, int block_id, double time);
bool is_block_received(int x, int block_id);
edge get_edge(int x, int y);
vector<int> get_peer_txn_ids(int x);
vector<int> get_peer_block_ids(int x);
vector<double> get_peer_time_arrivals(int x);
int get_speed_of_peer(int x);