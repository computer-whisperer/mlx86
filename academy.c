#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utils.h"
#include "academy.h"

#define UCB_C 0.01

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
	char * buffer = malloc(buffer_len);
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

/* This deletes all node children and the node data. */
void academy_prune_node(struct Academy_Agent_T * tree) {
	long i;
	for (i = 0; i < tree->children_count; i++)
	{
		if (tree->children[i].state == ACADEMY_AGENT_STATE_ALIVE) {
			academy_prune_node(tree->children[i].agent);
			struct Academy_Hashtable_Row_T * hashrow = academy_hashtable_lookup(tree->academy, tree->data_hash);
			hashrow->agent = NULL;
			hashrow->state = ACADEMY_AGENT_STATE_UNLOADED;
			tree->academy->hashtable_pruned_rows++;
			tree->children[i].agent = NULL;
			tree->children[i].state = ACADEMY_AGENT_STATE_UNLOADED;
			free(tree->children[i].agent);
		}
	}
	if (tree->data) {
		free(tree->data);
		tree->data = NULL;
	}
	tree->academy->loaded_agent_count--;
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
			tree->children[i].state = ACADEMY_AGENT_STATE_UNLOADED;
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
	}
}


void academy_get_probability_field(struct Academy_Agent_T * agent, float * probabilities) {
	// Score all candidates (first is parent)

	probabilities[0] = agent->own_value + UCB_C*sqrt(ceil_log2(agent->own_and_children_games_played) / ((float)agent->own_games_played+1));
	long i;
	float sum = probabilities[0];
	for (i = 0; i < agent->children_count; i++) {
		if (agent->children[i].state == ACADEMY_AGENT_STATE_ALIVE)
		{
			probabilities[i+1] = agent->children[i].subtree_value + UCB_C*sqrt(ceil_log2(agent->own_and_children_games_played) / ((float)agent->children[i].subtree_games_played+1));
			sum += probabilities[i+1];
		}
		else {
			probabilities[i+1] = 0.0;
		}
	}
	// normalize
	for (i = 0; i < agent->children_count+1; i++) {
		if (sum > 0)
		{
			probabilities[i] = probabilities[i]/sum;
		}
		else
		{
			probabilities[i] = 1.0/(agent->children_count+1.0);
		}
	}
}

void academy_update_expected_values(struct Academy_Agent_T * agent) {
	float probabilities[agent->children_count + 1];
	academy_get_probability_field(agent, probabilities);
}


void academy_select_matchup_probablistic(struct Academy_T * academy, struct Academy_Agent_T ** agent1, struct Academy_Agent_T ** agent2) {
	struct Academy_Agent_T * current_agent = academy->root_agent;

	if (fast_rand()%1000 == 0)
	{
		//academy_assert_sanity(academy);
	}

	long pass = 0;
	while(current_agent) {

		// Randomly check for prunable nodes to keep memory usage sane
		if (fast_rand()%30 == 0 && pass == 0)
			tree_search_test_prune_from_node(current_agent);

		float scores[current_agent->children_count + 1];
		academy_get_probability_field(current_agent, scores);

		// Select agent
		float selector = ((float)fast_rand())/FAST_RAND_MAX;
		int chosen_index;
		for (chosen_index = 0; chosen_index < current_agent->children_count; chosen_index++) {
			selector -= scores[chosen_index];


			if ((selector <= 0))
			{
				if ((chosen_index == 0) || (current_agent->children[chosen_index-1].state == ACADEMY_AGENT_STATE_ALIVE))
				{
					break;
				}
			}
		}

		if ((chosen_index != 0) && (!current_agent->children[chosen_index-1].agent))
		{
			printf("wtf???\n");
			*((char *) 0) = 1;
			exit(1);
		}

		// I select ME!
		if (chosen_index == 0)
		{
			if (pass == 0) {
				*agent1 = current_agent;
				pass = 1;
				current_agent = academy->root_agent;
			}
			else {
				*agent2 = current_agent;
				break;
			}
		}
		else {
			current_agent->children[chosen_index-1].subtree_games_played++;
			current_agent = current_agent->children[chosen_index-1].agent;
		}


	}
	if (!*agent1 || !*agent2)
	{
		printf("wtf???\n");
		*((char *) 0) = 1;
		exit(1);
	}

	return;
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

struct Academy_Agent_T * academy_add_new_agent(struct Academy_T * academy, struct Academy_Agent_T * parent, char * data, size_t data_len) {
	static long agents_created = 0;
	// Hash and check for duplicate agents first
	unsigned long agent_hash = hash_with_len((unsigned char *) data, data_len);
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
		parent->children[index].subtree_value = 0;
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

	agent->own_and_children_games_played = 0;

	agent->own_games_played = 0;
	agent->own_points = 0;
	agent->own_value = 0;

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

	return agent;
}

#define UPDATE_VAL 0.01

float score_update(float last_score, float score, float score_sum, long games_played)
{
	return score_sum/games_played;
	//return (last_score * (1 - UPDATE_VAL)) + (score * UPDATE_VAL);
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
	winner->own_and_children_games_played += 2;
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
}

void export_agent_nodes(struct Academy_Agent_T * agent, FILE * fp)
{
	unsigned long i;

	int subtree_games_played = -1;
	float subtree_value = -1;

	if (agent->parent)
	{
		subtree_games_played = agent->parent->children[agent->parent_child_index].subtree_games_played;
		subtree_value = agent->parent->children[agent->parent_child_index].subtree_value;
	}

	fprintf(fp, "<node id=\"%ld\">\n", agent->data_hash);
	fprintf(fp, "<attvalues>\n");
	fprintf(fp, "<attvalue for=\"0\" value=\"%d\"/>\n", agent->generation);
	fprintf(fp, "<attvalue for=\"1\" value=\"%d\"/>\n", agent->own_games_played);
	fprintf(fp, "<attvalue for=\"2\" value=\"%f\"/>\n", agent->own_value);
	fprintf(fp, "<attvalue for=\"3\" value=\"%i\"/>\n", subtree_games_played);
	fprintf(fp, "<attvalue for=\"4\" value=\"%f\"/>\n", subtree_value);
	fprintf(fp, "<attvalue for=\"5\" value=\"%d\"/>\n", agent->agent_num);
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
    fprintf(fp, "<attribute id=\"1\" title=\"Own Games Played\" type=\"integer\"/>\n");
    fprintf(fp, "<attribute id=\"2\" title=\"Own Value\" type=\"float\"/>\n");
    fprintf(fp, "<attribute id=\"3\" title=\"Subtree Games Played\" type=\"integer\"/>\n");
    fprintf(fp, "<attribute id=\"4\" title=\"Subtree Value\" type=\"float\"/>\n");
    fprintf(fp, "<attribute id=\"5\" title=\"Agent Number\" type=\"integer\"/>\n");
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
