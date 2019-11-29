
#ifndef ACADEMY_H
#define ACADEMY_H

#define ACADEMY_MAX_LOADED_AGENT_COUNT 100000

enum Academy_Agent_State_T {
	ACADEMY_AGENT_STATE_EMPTY,
	ACADEMY_AGENT_STATE_ALIVE,
	ACADEMY_AGENT_STATE_UNLOADED,
};

struct Academy_Agent_Child_T {
	struct Academy_Agent_T * agent;
	enum Academy_Agent_State_T state;

	/* Statistics against sibling agents. */
	float subtree_points;
	long subtree_games_played;
	float subtree_value;

	double subtree_probability;

	long queries_old;
};

struct Academy_Agent_T {
	long agent_num;

	struct Academy_T * academy;
	struct Academy_Agent_T * parent;
	long parent_child_index;
	long generation;

	/* Statistics against children agents. */
	float own_points;
	long own_games_played;
	float own_value;

	long queries_old;

	double own_probability;

	struct Academy_Agent_Child_T * children;
	long children_slots_allocated;
	long pruned_children_count;
	long children_count;
	size_t data_len;
	char * data;
	unsigned long data_hash;
};

struct Academy_Hashtable_Row_T {
	struct Academy_Agent_T * agent;
	unsigned long hash;
	enum Academy_Agent_State_T state;
};

struct Academy_T {
	struct Academy_Agent_T * root_agent;
	long agent_count;
	long duplicates_rejected;
	long loaded_agent_count;
	struct Academy_Hashtable_Row_T * hashtable;
	long hashtable_len;
	long hashtable_filled_rows;
	long hashtable_pruned_rows;

	float max_value;
};

struct Academy_T * build_new_academy();

struct Academy_Agent_T * academy_add_agent_from_file(struct Academy_T * academy, struct Academy_Agent_T * parent, char * fname);

void academy_prune_node(struct Academy_Agent_T * tree);

void academy_select_matchup(struct Academy_T * academy, struct Academy_Agent_T **, struct Academy_Agent_T **);

struct Academy_Agent_T * academy_add_new_agent(struct Academy_T * academy, struct Academy_Agent_T * parent, unsigned char * data, size_t data_len);

struct Academy_Hashtable_Row_T * academy_hashtable_lookup(struct Academy_T * academy, unsigned long hash);

void academy_rebuild_hashtable(struct Academy_T * academy);
void academy_rebuild_agent_children_list(struct Academy_Agent_T * agent);

void academy_report_agent_win(struct Academy_Agent_T * winner, float winner_points, struct Academy_Agent_T * looser, float looser_points);

void export_academy(struct Academy_T * academy, const char * fname);

#endif
