#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utils.h"
#include "academy.h"

#define UCB_C 2


struct Academy_Hashtable_Row_T * academy_hashtable_lookup(struct Academy_T * academy, unsigned long hash) {
	struct Academy_Hashtable_Row_T * row;
	// Quadratic probing
	for (int i = 0; i < academy->hashtable_len; i++) {
		row = &(academy->hashtable[(i*i + hash)%academy->hashtable_len]);
		if (!row->agent)
			return row;
		if (row->hash == hash)
			return row;
	}
	return NULL;
}

struct Academy_Agent_T * academy_add_agent_from_file(struct Academy_T * academy, struct Academy_Agent_T * parent, char * fname) {
	FILE * f = fopen(fname, "r");
	int buffer_len = 1000;
	char * buffer = malloc(buffer_len);
	int i = 0;
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
	academy->hashtable_len = 1000;
	academy->hashtable = malloc(sizeof(struct Academy_Hashtable_Row_T)*academy->hashtable_len);
	for (int i = 0; i < academy->hashtable_len; i++) {
		academy->hashtable[i].agent = NULL;
		academy->hashtable[i].hash = 0;
	}
	return academy;
}

void academy_prune_node(struct Academy_Agent_T * tree) {
	int i;
	for (i = 0; i < tree->children_count; i++)
		if (tree->children[i].state == ACADEMY_AGENT_STATE_ALIVE) {
			academy_prune_node(tree->children[i].agent);
			academy_hashtable_lookup(tree->academy, tree->data_hash)->agent = NULL;
			free(tree->children[i].agent);
			tree->children[i].agent = NULL;
		}
	if (tree->data) {
		free(tree->data);
		tree->data = NULL;
	}
	tree->academy->loaded_agent_count--;
}

void tree_search_test_prune_from_node(struct Academy_Agent_T * tree) {
	float threshold = (float)tree->academy->loaded_agent_count/ACADEMY_MAX_LOADED_AGENT_COUNT;
	for (int i = 0; i < tree->children_count; i++) {
		 if (tree->children[i].state == ACADEMY_AGENT_STATE_ALIVE && tree->children[i].subtree_games_played > 100 && tree->children[i].subtree_value < threshold) {
			 academy_prune_node(tree->children[i].agent);
			 tree->children[i].state = ACADEMY_AGENT_STATE_UNLOADED;
			 threshold = (float)tree->academy->loaded_agent_count/ACADEMY_MAX_LOADED_AGENT_COUNT;
		 }
	}
}


void academy_get_probability_field(struct Academy_Agent_T * agent, float * probabilities) {
	// Score all candidates (first is parent)

	probabilities[0] = agent->own_value + UCB_C*sqrt(ceil_log2(agent->own_and_children_games_played) / (float)agent->own_games_played);
	int i;
	float sum = probabilities[0];
	for (i = 0; i < agent->children_count; i++) {
		if (agent->children[i].state == ACADEMY_AGENT_STATE_ALIVE)
		{
			probabilities[i+1] = agent->children[i].subtree_value + UCB_C*sqrt(ceil_log2(agent->own_and_children_games_played) / (float)agent->children[i].subtree_games_played);
			sum += probabilities[i+1];
		}
		else {
			probabilities[i+1] = 0.0;
		}
	}
	// normalize
	for (i = 0; i < agent->children_count+1; i++) {
		probabilities[i] = probabilities[i]/sum;
	}
}

void academy_update_expected_values(struct Academy_Agent_T * agent) {
	float probabilities[agent->children_count + 1];
	academy_get_probability_field(agent, probabilities);
}


void academy_select_matchup_probablistic(struct Academy_T * academy, struct Academy_Agent_T ** agent1, struct Academy_Agent_T ** agent2) {
	struct Academy_Agent_T * current_agent = academy->root_agent;

	int pass = 0;
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
			if (selector <= 0)
				break;
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
				return;
			}
		}
		else {
			current_agent->children[chosen_index-1].subtree_games_played++;
			current_agent = current_agent->children[chosen_index-1].agent;
		}
	}
	return;
}



void academy_select_matchup(struct Academy_T * academy, struct Academy_Agent_T ** agent1, struct Academy_Agent_T ** agent2)
{
	academy_select_matchup_probablistic(academy, agent1, agent2);
}


void academy_expand_hashtable(struct Academy_T * academy) {
	int old_hashtable_len = academy->hashtable_len;
	struct Academy_Hashtable_Row_T * old_hashtable = academy->hashtable;

	// Allocate new table
	academy->hashtable_len *= 2;
	academy->hashtable = malloc(sizeof(struct Academy_Hashtable_Row_T) * academy->hashtable_len);

	// Clear new table
	for (int i = 0; i < academy->hashtable_len; i++) {
		academy->hashtable[i].hash = 0;
		academy->hashtable[i].agent = NULL;
	}

	// Move in all old data
	for (int i = 0; i < old_hashtable_len; i++) {
		if (old_hashtable[i].agent) {
			struct Academy_Hashtable_Row_T * row = academy_hashtable_lookup(academy, old_hashtable[i].hash);
			if (!row) {
				printf("Something went really, really wrong. Consider religion.");
				exit(1);
			}
			row->agent = old_hashtable[i].agent;
			row->hash = old_hashtable[i].hash;
		}
	}

	free(old_hashtable);
}

struct Academy_Agent_T * academy_add_new_agent(struct Academy_T * academy, struct Academy_Agent_T * parent, char * data, size_t data_len) {

	// Hash and check for duplicate agents first
	unsigned long agent_hash = hash_with_len((unsigned char *) data, data_len);
	struct Academy_Hashtable_Row_T * row = academy_hashtable_lookup(academy, agent_hash);
	if (!row) {
		printf("The academy's hashtable is completely full!!!\n");
		exit(1);
	}
	if (row->agent) {
		// Busted! The given code is identical to another agent!
		parent->aborted_children_count++;
		academy->duplicates_rejected++;
		free(data);
		return NULL;
	}

	// Check if we should expand the academy hashtable
	if ((float)academy->agent_count/(float)academy->hashtable_len > 0.75) {
		academy_expand_hashtable(academy);
		// Need to find row again!
		row = academy_hashtable_lookup(academy, agent_hash);
	}

	struct Academy_Agent_T * agent = malloc(sizeof(struct Academy_Agent_T));

	row->agent = agent;
	row->hash = agent_hash;

	if (parent) {
		// Add child data to parent

		// Expand parent array if neccessary
		if (parent->children_slots_allocated == parent->children_count) {
			parent->children_slots_allocated *= 2;
			parent->children = realloc(parent->children, sizeof(struct Academy_Agent_Child_T)*parent->children_slots_allocated);
		}
		int index = parent->children_count++;

		parent->children[index].agent = agent;
		parent->children[index].subtree_games_played = 1;
		parent->children[index].subtree_points = 0;
		parent->children[index].subtree_value = (float)parent->children[index].subtree_points/(float)parent->children[index].subtree_games_played;
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

	agent->own_and_children_games_played = 2;

	agent->own_games_played = 1;
	agent->own_points = 0;
	agent->own_value = (float)agent->own_points/(float)agent->own_games_played;

	agent->children_count = 0;
	agent->aborted_children_count = 0;
	agent->children_slots_allocated = 10;
	agent->children = malloc(sizeof(struct Academy_Agent_Child_T) * agent->children_slots_allocated);
	agent->data = data;
	agent->data_len = data_len;
	agent->data_hash = agent_hash;

	academy->agent_count++;
	academy->loaded_agent_count++;
	return agent;
}

void academy_report_agent_win(struct Academy_Agent_T * winner, float winner_points, struct Academy_Agent_T * looser, float looser_points) {
	struct Academy_Agent_T * winner_child = winner;
	struct Academy_Agent_T * looser_child = looser;

	/* Find the common ancestor of the winner and looser. */

	int i;

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
		winner->own_value = (float)winner->own_points/(float)winner->own_games_played;
	}
	else {
		// Identify child
		for (i = 0; i < winner->children_count; i++){
			if (winner->children[i].agent == winner_child)
				break;
		}
		winner->children[i].subtree_games_played++;
		winner->children[i].subtree_points += winner_points;
		winner->children[i].subtree_value = (float)winner->children[i].subtree_points/(float)winner->children[i].subtree_games_played;
	}

	// Looser scores
	if (looser == looser_child) {
		looser->own_games_played++;
		looser->own_points += looser_points;
		looser->own_value = (float)looser->own_points/(float)looser->own_games_played;
	}
	else {
		// Identify child
		for (i = 0; i < looser->children_count; i++){
			if (looser->children[i].agent == looser_child)
				break;
		}
		looser->children[i].subtree_games_played++;
		looser->children[i].subtree_points += looser_points;
		looser->children[i].subtree_value = (float)looser->children[i].subtree_points/(float)looser->children[i].subtree_games_played;
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
