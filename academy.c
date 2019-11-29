#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utils.h"
#include "academy.h"

#define UCB_C 0.1

void academy_update_probabilities(struct Academy_Agent_T * agent);

long academy_node_assert_sanity(struct Academy_Agent_T * agent)
{
	long faults = 0;

	// Check academy
	if (!agent->academy)
	{
		printf("Agent is missing it's academy!\n");
		faults++;
	}

	// Check data
	if (!agent->data || (agent->data_len == 0))
	{
		printf("Agent is missing it's data!\n");
		faults++;
	}
	else
	{
		if (hash_with_len((unsigned char *) agent->data, agent->data_len) != agent->data_hash)
		{
			printf("Agent data does not match it's hash!\n");
			faults++;
		}
		struct Academy_Hashtable_Row_T * row = academy_hashtable_lookup(agent->academy, agent->data_hash);
		if (row->agent != agent)
		{
			printf("Hashtable does not map to agent!\n");
			faults++;
		}
	}

	// Check children list
	long live_count = 0;
	long pruned_count = 0;
	long empty_count = 0;
	long i;
	for (i = 0; i < agent->children_slots_allocated; i++)
	{
		if (agent->children[i].state == ACADEMY_AGENT_STATE_EMPTY)
		{
			empty_count++;
			if (agent->children[i].agent)
			{
				printf("This agent should be null.\n");
				faults++;
			}
		}
		else if (agent->children[i].state == ACADEMY_AGENT_STATE_UNLOADED)
		{
			pruned_count++;
			if (agent->children[i].agent)
			{
				printf("This agent should be null.\n");
				faults++;
			}
		}
		else
		{
			live_count++;
			if (!agent->children[i].agent)
			{
				printf("This agent should not be null.\n");
				faults++;
			}
			else
			{
				if (agent->children[i].agent->parent != agent)
				{
					printf("This child does not know it's parent!!!\n");
					faults++;
				}
				if (agent->children[i].agent->parent_child_index != i)
				{
					printf("This child does not know it's parent child index!!!\n");
					faults++;
				}
				faults += academy_node_assert_sanity(agent->children[i].agent);
			}
		}
	}
	return faults;
}

void academy_assert_sanity(struct Academy_T * academy)
{
	long faults = 0;
	// Check overall numbers
	if ((academy->hashtable_filled_rows - academy->hashtable_pruned_rows) != academy->loaded_agent_count)
	{
		printf("Bad hashtable or loaded agent count.\n");
		faults++;
	}

	// Check hashtable
	long filled_rows = 0;
	long empty_rows = 0;
	long pruned_rows = 0;
	long i;
	for (i = 0; i < academy->hashtable_len; i++)
	{
		if (academy->hashtable[i].state == ACADEMY_AGENT_STATE_EMPTY)
		{
			if (academy->hashtable[i].agent)
			{
				faults++;
				printf("Bad hashtable entry.\n");
			}
			empty_rows++;
		}
		else if (academy->hashtable[i].state == ACADEMY_AGENT_STATE_UNLOADED)
		{
			if (academy->hashtable[i].agent)
			{
				faults++;
				printf("Bad hashtable entry.\n");
			}
			pruned_rows++;
		}
		else
		{
			if (!academy->hashtable[i].agent)
			{
				faults++;
				printf("Bad empty hashtable entry.\n");
			}
			filled_rows++;
		}
	}
	if (filled_rows != (academy->hashtable_filled_rows - academy->hashtable_pruned_rows))
	{
		faults++;
		printf("Bad filled row count.\n");
	}
	if (pruned_rows != academy->hashtable_pruned_rows)
	{
		faults++;
		printf("Bad filled row count.\n");
	}

	if (academy->root_agent)
	{
		faults += academy_node_assert_sanity(academy->root_agent);
	}

	if (faults > 0)
	{
		printf("Aborting.\n");
		exit(-1);
	}
}

struct Academy_Hashtable_Row_T * academy_hashtable_lookup(struct Academy_T * academy, unsigned long hash) {
	struct Academy_Hashtable_Row_T * row;
	// Quadratic probing
	for (long i = 0; i < academy->hashtable_len; i++) {
		row = &(academy->hashtable[(i*i + hash)%academy->hashtable_len]);
		if (row->state == ACADEMY_AGENT_STATE_EMPTY)
			return row;
		if (row->hash == hash)
			return row;
	}
	return NULL;
}

struct Academy_Agent_T * academy_add_agent_from_file(struct Academy_T * academy, struct Academy_Agent_T * parent, char * fname) {
	FILE * f = fopen(fname, "r");
	long buffer_len = 1000;
	unsigned char * buffer = malloc(buffer_len);
	long i = 0;
	while (!i || buffer[i-1] != EOF) {
		buffer[i] = fgetc(f);
		i++;

		// Simple reallocating buffer growth
		if (i == buffer_len) {
			int new_buffer_len = buffer_len*5;
			char * new_buffer = malloc(new_buffer_len);
			memcpy(new_buffer, buffer, buffer_len);
			free(buffer);
			buffer = new_buffer;
			buffer_len = new_buffer_len;
		}
	}
	fclose(f);
	buffer[i-1] = '\0'; // Overwrite EOF with empty string
	return academy_add_new_agent(academy, parent, buffer, i);
}

struct Academy_T * build_new_academy() {
	struct Academy_T * academy = malloc(sizeof(struct Academy_T));
	academy->agent_count = 0;
	academy->duplicates_rejected = 0;
	academy->loaded_agent_count = 0;
	academy->root_agent = NULL;
	academy->hashtable = NULL;
	academy->hashtable_len = 1000;
	academy->hashtable_pruned_rows = 0;
	academy->hashtable_filled_rows = 0;
	academy_rebuild_hashtable(academy);
	academy->max_value = 0;
	return academy;
}

/* This deletes all node children and the node. */
void academy_prune_node(struct Academy_Agent_T * tree) {
	long i;
	if (tree == tree->academy->root_agent)
	{
		// Not allowed to kill the root node!!!
		return;
	}

	for (i = 0; i < tree->children_count; i++)
	{
		if (tree->children[i].state == ACADEMY_AGENT_STATE_ALIVE) {
			academy_prune_node(tree->children[i].agent);
		}
	}
	if (tree->data) {
		free(tree->data);
		tree->data = NULL;
	}
	tree->parent->children[tree->parent_child_index].agent = NULL;
	tree->parent->children[tree->parent_child_index].state = ACADEMY_AGENT_STATE_UNLOADED;
	tree->parent->pruned_children_count++;

	struct Academy_Hashtable_Row_T * hashrow = academy_hashtable_lookup(tree->academy, tree->data_hash);
	hashrow->agent = NULL;
	hashrow->state = ACADEMY_AGENT_STATE_UNLOADED;
	tree->academy->hashtable_pruned_rows++;
	tree->academy->loaded_agent_count--;

	free(tree);
}

void tree_search_test_prune_from_node(struct Academy_Agent_T * tree) {
	float threshold = tree->academy->max_value * ((float)tree->academy->loaded_agent_count/ACADEMY_MAX_LOADED_AGENT_COUNT);
	int nodes_pruned = 0;
	for (int i = 0; i < tree->children_count; i++) {
		if (tree->academy->loaded_agent_count < 0.5*ACADEMY_MAX_LOADED_AGENT_COUNT)
		{
			break;
		}
		if (tree->children[i].state == ACADEMY_AGENT_STATE_ALIVE && tree->children[i].subtree_games_played > 100 && tree->children[i].subtree_value < threshold) {
			academy_prune_node(tree->children[i].agent);
			threshold = (float)tree->academy->loaded_agent_count/ACADEMY_MAX_LOADED_AGENT_COUNT;
			nodes_pruned = 1;
		}
	}
	if (nodes_pruned)
	{
		if ((tree->children_count - tree->pruned_children_count) < (tree->children_slots_allocated/2))
		{
			academy_rebuild_agent_children_list(tree);
		}
		academy_update_probabilities(tree);
	}
}


void academy_update_probabilities(struct Academy_Agent_T * agent) {
	// Score all candidates (first is parent)
	long i;
	double sum;

	// Start with agent value
	agent->own_probability = agent->own_value;
	for (i = 0; i < agent->children_count; i++)
	{
		if (agent->children[i].state == ACADEMY_AGENT_STATE_ALIVE)
		{
			agent->children[i].subtree_probability = agent->children[i].subtree_value;
		}
		else
		{
			agent->children[i].subtree_probability = 0.0;
		}
	}

	// Raise to powers
	agent->own_probability = pow(agent->own_probability, 10);
	for (i = 0; i < agent->children_count; i++)
	{
		agent->children[i].subtree_probability = pow(agent->children[i].subtree_probability, 10);
	}

	// Get sum
	sum = agent->own_probability;
	for (i = 0; i < agent->children_count; i++)
	{
		sum += agent->children[i].subtree_probability;
	}

	// Normalize
	agent->own_probability = agent->own_probability/sum;
	for (i = 0; i < agent->children_count; i++)
	{
		agent->children[i].subtree_probability = agent->children[i].subtree_probability/sum;
	}

	// Add UCB
	agent->own_probability += UCB_C*sqrt(ceil_log2(agent->queries_old+1) / ((double)agent->own_games_played+1));
	for (i = 0; i < agent->children_count; i++)
	{
		if (agent->children[i].state == ACADEMY_AGENT_STATE_ALIVE)
		{
			agent->children[i].subtree_probability += UCB_C*sqrt(ceil_log2(agent->queries_old+1) / ((double)agent->children[i].subtree_games_played+1));
		}
	}

	// Set to 1 if values are all null
	if (sum == 0)
	{
		agent->own_probability = 1;
		for (i = 0; i < agent->children_count; i++)
		{
			if (agent->children[i].state == ACADEMY_AGENT_STATE_ALIVE)
			{
				agent->children[i].subtree_probability = 1;
			}
			else
			{
				agent->children[i].subtree_probability = 0;
			}
		}
	}

	if (!isfinite(agent->own_probability))
	{
		printf("Infinity FTW!!!");
		abort();
	}

	// Get sum
	sum = agent->own_probability;
	for (i = 0; i < agent->children_count; i++)
	{
		sum += agent->children[i].subtree_probability;
	}

	if (!isfinite(agent->own_probability))
	{
		printf("Infinity FTW!!!");
		abort();
	}

	// Normalize
	agent->own_probability = agent->own_probability/sum;
	for (i = 0; i < agent->children_count; i++)
	{
		agent->children[i].subtree_probability = agent->children[i].subtree_probability/sum;
	}

	if (!isfinite(agent->own_probability))
	{
		printf("Infinity FTW!!!");
		abort();
	}
}

void academy_select_matchup_probablistic(struct Academy_T * academy, struct Academy_Agent_T ** agent1, struct Academy_Agent_T ** agent2) {
	struct Academy_Agent_T * current_agent = academy->root_agent;

//	if (fast_rand()%1000 == 0)
//	{
//
	//}
	//academy_assert_sanity(academy);
	long pass = 0;

	struct Academy_Agent_T * least_tried_fork_agent = current_agent;
	long least_tried_fork_tries = 10000000L;

	while(current_agent) {

		if (!isfinite(current_agent->own_probability))
		{
			printf("Infinity FTW!!!");
			abort();
		}

		// Randomly check for prunable nodes to keep memory usage sane
		if (fast_rand()%30 == 0 && pass == 0)
			tree_search_test_prune_from_node(current_agent);

		current_agent->queries_old++;
		long i;
		for (i = 0; i < current_agent->children_count; i++)
		{
			if (current_agent->children[i].state == ACADEMY_AGENT_STATE_ALIVE)
			{
				current_agent->children[i].queries_old++;
			}
		}

		// Select agent
		double selector = ((double)fast_rand())/FAST_RAND_MAX;
		selector -= current_agent->own_probability;

		if (selector <= 0)
		{
			// I select ME!!!
			if (current_agent->own_games_played < least_tried_fork_tries && current_agent->children_count > 0)
			{
				least_tried_fork_agent = current_agent;
				least_tried_fork_tries = current_agent->own_games_played;
			}

			if (pass == 0) {
				*agent1 = current_agent;
				pass = 1;
				current_agent = least_tried_fork_agent;
			}
			else {
				*agent2 = current_agent;
				break;
			}
		}
		else
		{
			// Iterate to a child
			for (i = 0; i < current_agent->children_count; i++)
			{
				selector -= current_agent->children[i].subtree_probability;
				if (selector <= 0)
				{
					current_agent->children[i].subtree_games_played++;
					if (current_agent->children[i].subtree_games_played < least_tried_fork_tries)
					{
						least_tried_fork_agent = current_agent;
						least_tried_fork_tries = current_agent->children[i].subtree_games_played;
					}
					current_agent = current_agent->children[i].agent;
					break;
				}
			}
		}
	}
}



void academy_select_matchup(struct Academy_T * academy, struct Academy_Agent_T ** agent1, struct Academy_Agent_T ** agent2)
{
	academy_select_matchup_probablistic(academy, agent1, agent2);
}


void academy_rebuild_hashtable(struct Academy_T * academy) {
	long old_hashtable_len = academy->hashtable_len;
	struct Academy_Hashtable_Row_T * old_hashtable = academy->hashtable;

	// Check if we need to resize the table
	if (academy->hashtable && ((academy->hashtable_filled_rows - academy->hashtable_pruned_rows) > (academy->hashtable_len/2)))
	{
		academy->hashtable_len *= 2;
	}

	// Allocate new table
	academy->hashtable = malloc(sizeof(struct Academy_Hashtable_Row_T) * academy->hashtable_len);
	memset(academy->hashtable, 0, academy->hashtable_len * sizeof(struct Academy_Hashtable_Row_T));
	academy->hashtable_filled_rows = 0;
	academy->hashtable_pruned_rows = 0;

	// Move in all old data

	if (old_hashtable)
	{
		for (long i = 0; i < old_hashtable_len; i++) {
			if (old_hashtable[i].agent) {
				struct Academy_Hashtable_Row_T * row = academy_hashtable_lookup(academy, old_hashtable[i].hash);
				if (!row) {
					printf("Something went really, really wrong. Consider religion.");
					exit(1);
				}
				row->agent = old_hashtable[i].agent;
				row->hash = old_hashtable[i].hash;
				row->state = old_hashtable[i].state;
				academy->hashtable_filled_rows++;
			}
		}

		free(old_hashtable);
	}
}

void academy_rebuild_agent_children_list(struct Academy_Agent_T * agent)
{
	struct Academy_Agent_Child_T * old_children = agent->children;
	long old_children_count = agent->children_count;
	long i;

	// Expand array if neccessary
	if ((agent->children_count - agent->pruned_children_count) > agent->children_slots_allocated/2)
	{
		agent->children_slots_allocated *= 2;
	}

	agent->children = malloc(sizeof(struct Academy_Agent_Child_T) * agent->children_slots_allocated);
	agent->children_count = 0;
	agent->pruned_children_count = 0;

	if (old_children)
	{
		for (i = 0; i < old_children_count; i++)
		{
			if (old_children[i].state == ACADEMY_AGENT_STATE_ALIVE)
			{
				memcpy(&(agent->children[agent->children_count]), &old_children[i], sizeof(struct Academy_Agent_Child_T));
				agent->children[agent->children_count].agent->parent_child_index = agent->children_count;
				agent->children_count++;
			}
		}

		free(old_children);
	}

	for (i = agent->children_count; i < agent->children_slots_allocated; i++)
	{
		agent->children[i].state = ACADEMY_AGENT_STATE_EMPTY;
		agent->children[i].agent = 0;
	}
}

struct Academy_Agent_T * academy_add_new_agent(struct Academy_T * academy, struct Academy_Agent_T * parent, unsigned char * data, size_t data_len) {
	static long agents_created = 0;
	// Hash and check for duplicate agents first
	unsigned long agent_hash = hash_with_len(data, data_len);
	struct Academy_Hashtable_Row_T * row = academy_hashtable_lookup(academy, agent_hash);
	if (!row) {
		printf("The academy's hashtable is completely full!!!\n");
		exit(1);
	}
	if (row->state != ACADEMY_AGENT_STATE_EMPTY) {
		// Busted! The given code is identical to another agent!
		academy->duplicates_rejected++;
		free(data);
		return NULL;
	}

	// Check if we should expand the academy hashtable
	if ((float)academy->hashtable_filled_rows/(float)academy->hashtable_len > 0.75) {
		academy_rebuild_hashtable(academy);
		// Need to find row again!
		row = academy_hashtable_lookup(academy, agent_hash);
	}

	struct Academy_Agent_T * agent = malloc(sizeof(struct Academy_Agent_T));

	agent->agent_num = agents_created++;

	row->agent = agent;
	row->hash = agent_hash;
	row->state = ACADEMY_AGENT_STATE_ALIVE;

	if (parent) {
		// Add child data to parent

		if (parent->children_slots_allocated == parent->children_count) {
			academy_rebuild_agent_children_list(parent);
		}

		long index = parent->children_count++;

		parent->children[index].agent = agent;
		parent->children[index].subtree_games_played = 0;
		parent->children[index].subtree_points = 0;
		parent->children[index].subtree_value = parent->own_value;
		parent->children[index].queries_old = 0;
		parent->children[index].subtree_probability = 0;
		parent->children[index].state = ACADEMY_AGENT_STATE_ALIVE;
		agent->generation = parent->generation + 1;
		agent->parent_child_index = index;
	}
	else {
		agent->generation = 0;
		agent->parent_child_index = 0;
		if (academy->root_agent) {
			printf("An impostor is trying to assassinate the root node, oh no!\n");
			exit(1);
		}
		academy->root_agent = agent;
	}

	agent->academy = academy;
	agent->parent = parent;

	agent->queries_old = 0;

	agent->own_probability = 0;

	agent->own_games_played = 0;
	agent->own_points = 0;
	if (parent)
	{
		agent->own_value = parent->own_value;
	}
	else
	{
		agent->own_value = 0.5;
	}

	agent->children_count = 0;
	agent->pruned_children_count = 0;
	agent->children_slots_allocated = 10;
	agent->children = NULL;
	academy_rebuild_agent_children_list(agent);
	agent->data = data;
	agent->data_len = data_len;
	agent->data_hash = agent_hash;

	academy->agent_count++;
	academy->loaded_agent_count++;
	academy->hashtable_filled_rows++;

	if (parent)
	{
		academy_update_probabilities(parent);
	}
	academy_update_probabilities(agent);

	return agent;
}

#define UPDATE_VAL 0.1

float score_update(float last_score, float score, float score_sum, long games_played)
{
	//return score_sum/games_played;
	return (last_score * (1.0 - UPDATE_VAL)) + (score * UPDATE_VAL);
	//if (score > last_score)
	//	return score;
	//else
	//	return last_score;
}

void academy_report_agent_win(struct Academy_Agent_T * winner, float winner_points, struct Academy_Agent_T * looser, float looser_points) {
	struct Academy_Agent_T * winner_child = winner;
	struct Academy_Agent_T * looser_child = looser;

	if (winner_points > winner->academy->max_value)
		winner->academy->max_value = winner_points;

	/* Find the common ancestor of the winner and looser. */

	long i;

	while (winner != looser) {
		struct Academy_Agent_T * next_winner = winner;
		struct Academy_Agent_T * next_looser = looser;
		if (winner->generation >= looser->generation) {
			next_winner = winner->parent;
			winner_child = winner;
		}
		if (winner->generation <= looser->generation) {
			next_looser = looser->parent;
			looser_child = looser;
		}
		winner = next_winner;
		looser = next_looser;
	}
	// Now winner == looser

	// use winner_child and looser_child to figure out where scores go

	// Winner scores
	if (winner == winner_child) {
		winner->own_games_played++;
		winner->own_points += winner_points;
		winner->own_value = score_update(winner->own_value, winner_points, winner->own_points, winner->own_games_played);
	}
	else {
		// Identify child
		for (i = 0; i < winner->children_count; i++){
			if (winner->children[i].agent == winner_child)
				break;
		}
		winner->children[i].subtree_games_played++;
		winner->children[i].subtree_points += winner_points;
		winner->children[i].subtree_value = score_update(winner->children[i].subtree_value, winner_points, winner->children[i].subtree_points, winner->children[i].subtree_games_played);
	}

	// Looser scores
	if (looser == looser_child) {
		looser->own_games_played++;
		looser->own_points += looser_points;
		looser->own_value = score_update(looser->own_value, looser_points, looser->own_points, looser->own_games_played);

	}
	else {
		// Identify child
		for (i = 0; i < looser->children_count; i++){
			if (looser->children[i].agent == looser_child)
				break;
		}
		looser->children[i].subtree_games_played++;
		looser->children[i].subtree_points += looser_points;
		looser->children[i].subtree_value = score_update(looser->children[i].subtree_value, looser_points, looser->children[i].subtree_points, looser->children[i].subtree_games_played);

	}

	academy_update_probabilities(winner);
}

void export_agent_nodes(struct Academy_Agent_T * agent, FILE * fp)
{
	unsigned long i;

	int subtree_games_played = -1;
	float subtree_value = -1;
	int subtree_queries_old = -1;
	double subtree_probability = -1;

	if (agent->parent)
	{
		subtree_games_played = agent->parent->children[agent->parent_child_index].subtree_games_played;
		subtree_value = agent->parent->children[agent->parent_child_index].subtree_value;
		subtree_queries_old = agent->parent->children[agent->parent_child_index].queries_old;
		subtree_probability = agent->parent->children[agent->parent_child_index].subtree_probability;
	}

	fprintf(fp, "<node id=\"%ld\">\n", agent->data_hash);
	fprintf(fp, "<attvalues>\n");
	fprintf(fp, "<attvalue for=\"0\" value=\"%ld\"/>\n", agent->generation);
	fprintf(fp, "<attvalue for=\"1\" value=\"%ld\"/>\n", agent->queries_old);
	fprintf(fp, "<attvalue for=\"2\" value=\"%ld\"/>\n", agent->own_games_played);
	fprintf(fp, "<attvalue for=\"3\" value=\"%f\"/>\n", agent->own_value);
	fprintf(fp, "<attvalue for=\"4\" value=\"%f\"/>\n", agent->own_probability);
	fprintf(fp, "<attvalue for=\"5\" value=\"%i\"/>\n", subtree_queries_old);
	fprintf(fp, "<attvalue for=\"6\" value=\"%i\"/>\n", subtree_games_played);
	fprintf(fp, "<attvalue for=\"7\" value=\"%f\"/>\n", subtree_value);
	fprintf(fp, "<attvalue for=\"8\" value=\"%f\"/>\n", subtree_probability);
	fprintf(fp, "<attvalue for=\"9\" value=\"%d\"/>\n", agent->agent_num);
	fprintf(fp, "</attvalues>\n");
	fprintf(fp, "</node>\n");
	for (i = 0; i < agent->children_count; i++)
	{
		if (agent->children[i].state == ACADEMY_AGENT_STATE_ALIVE)
		{
			export_agent_nodes(agent->children[i].agent, fp);
		}
	}
}

void export_agent_edges(struct Academy_Agent_T * agent, FILE * fp)
{
	unsigned long i;
	for (i = 0; i < agent->children_count; i++)
	{
		if (agent->children[i].state == ACADEMY_AGENT_STATE_ALIVE)
		{
			fprintf(fp, "<edge source=\"%ld\" target=\"%ld\"/>\n", agent->data_hash, agent->children[i].agent->data_hash);
			export_agent_edges(agent->children[i].agent, fp);
		}
	}
}

void export_academy(struct Academy_T * academy, const char * fname)
{
	FILE * fp = fopen(fname, "w");
	fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fp, "<gexf xmlns=\"http://www.gexf.net/1.2draft\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.gexf.net/1.2draft http://www.gexf.net/1.2draft/gexf.xsd\" version=\"1.2\">\n");
    fprintf(fp, "<meta lastmodifieddate=\"2009-03-20\">\n");
    fprintf(fp, "<creator>Gephi.org</creator>\n");
    fprintf(fp, "<description>A Web network</description>\n");
    fprintf(fp, "</meta>\n");
    fprintf(fp, "<graph defaultedgetype=\"directed\">\n");
    fprintf(fp, "<attributes class=\"node\">\n");
    fprintf(fp, "<attribute id=\"0\" title=\"Generation\" type=\"integer\"/>\n");
    fprintf(fp, "<attribute id=\"1\" title=\"Own Queries Old\" type=\"integer\"/>\n");
    fprintf(fp, "<attribute id=\"2\" title=\"Own Games Played\" type=\"integer\"/>\n");
    fprintf(fp, "<attribute id=\"3\" title=\"Own Value\" type=\"float\"/>\n");
    fprintf(fp, "<attribute id=\"4\" title=\"Own Probability\" type=\"float\"/>\n");
    fprintf(fp, "<attribute id=\"5\" title=\"Subtree Queries Old\" type=\"integer\"/>\n");
    fprintf(fp, "<attribute id=\"6\" title=\"Subtree Games Played\" type=\"integer\"/>\n");
    fprintf(fp, "<attribute id=\"7\" title=\"Subtree Value\" type=\"float\"/>\n");
    fprintf(fp, "<attribute id=\"8\" title=\"Subtree Probability\" type=\"float\"/>\n");
    fprintf(fp, "<attribute id=\"9\" title=\"Agent Number\" type=\"integer\"/>\n");
    fprintf(fp, "</attributes>\n");
    fprintf(fp, "<nodes>\n");
    export_agent_nodes(academy->root_agent, fp);
    fprintf(fp, "</nodes>\n");
    fprintf(fp, "<edges>\n");
    export_agent_edges(academy->root_agent, fp);
    fprintf(fp, "</edges>\n");
    fprintf(fp, "</graph>\n");
    fprintf(fp, "</gexf>\n");
    fclose(fp);
}
