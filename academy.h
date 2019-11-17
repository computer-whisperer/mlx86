
#ifndef ACADEMY_H
#define ACADEMY_H

#define ACADEMY_MAX_LOADED_AGENT_COUNT 10000

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
	int subtree_games_played;
	float subtree_value;
};

struct Academy_Agent_T {
	struct Academy_T * academy;
	struct Academy_Agent_T * parent;
	int parent_child_index;
	int generation;

	int own_and_children_games_played;

	/* Statistics against children agents. */
	float own_points;
	int own_games_played;
	float own_value;

	struct Academy_Agent_Child_T * children;
	int children_slots_allocated;
	int aborted_children_count;
	int children_count;
	size_t data_len;
	char * data;
	unsigned long data_hash;
};

struct Academy_Hashtable_Row_T {
	struct Academy_Agent_T * agent;
	unsigned long hash;
};

struct Academy_T {
	struct Academy_Agent_T * root_agent;
	int agent_count;
	int duplicates_rejected;
	int loaded_agent_count;
	struct Academy_Hashtable_Row_T * hashtable;
	int hashtable_len;
};

struct Academy_T * build_new_academy();

struct Academy_Agent_T * academy_add_agent_from_file(struct Academy_T * academy, struct Academy_Agent_T * parent, char * fname);

void academy_prune_node(struct Academy_Agent_T * tree);

void academy_select_matchup(struct Academy_T * academy, struct Academy_Agent_T **, struct Academy_Agent_T **);

struct Academy_Agent_T * academy_add_new_agent(struct Academy_T * academy, struct Academy_Agent_T * parent, char * data, size_t data_len);

void academy_report_agent_win(struct Academy_Agent_T * winner, float winner_points, struct Academy_Agent_T * looser, float looser_points);

void export_academy(struct Academy_T * academy, const char * fname);

#endif
