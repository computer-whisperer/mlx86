
#ifndef ACADEMY_H
#define ACADEMY_H

#define ACADEMY_MAX_AGENT_COUNT 1000000
#define ACADEMY_CHILDREN_PER_AGENT 100

typedef unsigned long ACADEMY_AGENT_ID;
#define ACADEMY_INVALID_AGENT_ID 0

#define ACADEMY_AGENT_DATA_LEN 10000

enum ACADEMY_HASHTABLE_ROW_STATE_T
{
	ACADEMY_HASHTABLE_ROW_STATE_TOMBSTONE,
	ACADEMY_HASHTABLE_ROW_STATE_EMPTY,
	ACADEMY_HASHTABLE_ROW_STATE_FULL
};

struct Academy_Hashtable_Row_T
{
	enum ACADEMY_HASHTABLE_ROW_STATE_T state;
	ACADEMY_AGENT_ID agent_id;
	unsigned long agent_table_index;
};

struct Academy_Agent_Metadata_T
{
	/* Statistics against sibling agents. */
	float points;
	long games_played;
	float value;
	double probability;
};

struct Academy_Agent_T {
	ACADEMY_AGENT_ID id;
	struct Academy_T * academy;
	ACADEMY_AGENT_ID parent_id;
	unsigned long parent_child_index;
	long generation;

	long queries_old;

	/* Statistics against children agents. */
	struct Academy_Agent_Metadata_T own_metadata;

	struct Academy_Agent_Metadata_T children_metadata[ACADEMY_CHILDREN_PER_AGENT];

	ACADEMY_AGENT_ID children_ids[ACADEMY_CHILDREN_PER_AGENT];

	unsigned long child_count;
	size_t data_len;
	unsigned char data[ACADEMY_AGENT_DATA_LEN];
};

struct Academy_T
{
	struct Academy_Hashtable_Row_T agent_id_hashtable[ACADEMY_MAX_AGENT_COUNT];
	unsigned long agent_id_hashtable_max_offset;
	struct Academy_Agent_T agents[ACADEMY_MAX_AGENT_COUNT];
	long agent_count;
	ACADEMY_AGENT_ID last_agent_id;
	ACADEMY_AGENT_ID root_agent_id;
	float max_value;
};

void build_new_academy(struct Academy_T * academy);

struct Academy_Agent_T * academy_get_agent_from_id(struct Academy_T * academy, ACADEMY_AGENT_ID agent_id);

void academy_prune_node(struct Academy_Agent_T * tree);

void academy_select_matchup(struct Academy_T * academy, struct Academy_Agent_T **, struct Academy_Agent_T **);

void academy_add_new_agent(struct Academy_T * academy, struct Academy_Agent_T * parent, unsigned char * data, size_t data_len);

void academy_report_agent_win(struct Academy_Agent_T * winner, float winner_points, struct Academy_Agent_T * looser, float looser_points);

void export_academy(struct Academy_T * academy, const char * fname);

#endif
