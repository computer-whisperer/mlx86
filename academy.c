#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utils.h"
#include "academy.h"

#define UCB_C 0.01

void academy_update_probabilities(struct Academy_Agent_T * agent);

unsigned long academy_agent_id_hash(ACADEMY_AGENT_ID agent_id)
{
	return agent_id*7;
}

long academy_node_assert_sanity(struct Academy_Agent_T * agent)
{
	long faults = 0;

	// Check academy
	if (!agent->academy)
	{
		printf("Agent is missing it's academy!\n");
		faults++;
	}

	// Check id
	if (academy_get_agent_from_id(agent->academy, agent->id) != agent)
	{
		printf("Agent ID is incorrect!\n");
		faults++;
	}

	// Check data
	if (!agent->data || (agent->data_len == 0))
	{
		printf("Agent is missing it's data!\n");
		faults++;
	}

	// Check children list
	long live_count = 0;
	long i;
	for (i = 0; i < ACADEMY_CHILDREN_PER_AGENT; i++)
	{
		struct Academy_Agent_T * child = academy_get_agent_from_id(agent->academy, agent->children_ids[i]);
		if (child)
		{
			live_count++;
			if (child->parent_id != agent->id)
			{
				printf("This child does not know it's parent!!!\n");
				faults++;
			}
			if (child->parent_child_index != i)
			{
				printf("This child does not know it's parent child index!!!\n");
				faults++;
			}
			faults += academy_node_assert_sanity(child);
		}
	}
	return faults;
}

void academy_assert_sanity(struct Academy_T * academy)
{
	long faults = 0;

	struct Academy_Agent_T * agent = academy_get_agent_from_id(academy, academy->root_agent_id);
	if (agent)
	{
		faults += academy_node_assert_sanity(agent);
	}

	// Check hash table
	unsigned long table_rows_full = 0;
	for (unsigned long i = 0; i < ACADEMY_MAX_AGENT_COUNT; i++)
	{
		if (academy->agent_id_hashtable[i].state == ACADEMY_HASHTABLE_ROW_STATE_FULL)
		{
			table_rows_full++;
			if (academy->agent_id_hashtable[i].agent_id == ACADEMY_INVALID_AGENT_ID)
			{
				printf("An invalid agent id was found in a hashtable row that had a FULL state.\n");
				faults++;
			}
			if (academy->agent_id_hashtable[i].agent_table_index >= ACADEMY_MAX_AGENT_COUNT)
			{
				printf("An invalid agent table index was found in a hashtable row that had a FULL state.\n");
				faults++;
			}
			if (academy->agents[academy->agent_id_hashtable[i].agent_table_index].id != academy->agent_id_hashtable[i].agent_id)
			{
				printf("A hashtable row that was FULL points to a agent with the wrong ID.\n");
				faults++;
			}
		}
	}
	if (table_rows_full != academy->agent_count)
	{
		printf("The number of filled rows in the hashtable does not match the number of agents!\n");
		faults++;
	}

	if (faults > 0)
	{
		printf("Aborting.\n");
		exit(-1);
	}
}

struct Academy_Agent_T * academy_get_agent_from_id(struct Academy_T * academy, ACADEMY_AGENT_ID agent_id)
{
	struct Academy_Agent_T * agent = NULL;
	unsigned long hashtable_index = academy_agent_id_hash(agent_id) % ACADEMY_MAX_AGENT_COUNT;
	unsigned long probing_offset = 0;
	while (1)
	{
		if ((academy->agent_id_hashtable[hashtable_index].state == ACADEMY_HASHTABLE_ROW_STATE_EMPTY) || (probing_offset > academy->agent_id_hashtable_max_offset))
		{
			break;
		}

		if (academy->agent_id_hashtable[hashtable_index].agent_id == agent_id)
		{
			agent = &academy->agents[academy->agent_id_hashtable[hashtable_index].agent_table_index];
			break;
		}

		probing_offset += 1;
		hashtable_index += 1;
		if (hashtable_index == ACADEMY_MAX_AGENT_COUNT)
		{
			hashtable_index = 0;
		}
	}
	return agent;
}

void build_new_academy(struct Academy_T * academy)
{
	academy->agent_count = 0;
	academy->max_value = 0;
	academy->last_agent_id = ACADEMY_INVALID_AGENT_ID;
	academy->root_agent_id = ACADEMY_INVALID_AGENT_ID;
	academy->agent_id_hashtable_max_offset = 0;

	for (long i = 0; i < ACADEMY_MAX_AGENT_COUNT; i++)
	{
		academy->agent_id_hashtable[i].state = ACADEMY_HASHTABLE_ROW_STATE_EMPTY;
		academy->agent_id_hashtable[i].agent_id = ACADEMY_INVALID_AGENT_ID;
		academy->agent_id_hashtable[i].agent_table_index = i;
	}
}

/* This deletes all node children and the node. */
void academy_prune_node(struct Academy_Agent_T * agent)
{
	struct Academy_T * academy = agent->academy;
	if (agent->id == academy->root_agent_id)
	{
		// Not allowed to kill the root node!!!
		return;
	}

	for (long i = 0; i < ACADEMY_CHILDREN_PER_AGENT; i++)
	{
		if (agent->children_ids[i] != ACADEMY_INVALID_AGENT_ID)
		{
			academy_prune_node(academy_get_agent_from_id(academy, agent->children_ids[i]));
		}
	}
	struct Academy_Agent_T * agent_parent = academy_get_agent_from_id(agent->academy, agent->parent_id);
	agent_parent->children_ids[agent->parent_child_index] = ACADEMY_INVALID_AGENT_ID;
	agent_parent->child_count--;

	ACADEMY_AGENT_ID agent_id = agent->id;

	// Delete this agent from the id hashtable
	unsigned long hashtable_index = academy_agent_id_hash(agent_id) % ACADEMY_MAX_AGENT_COUNT;
	unsigned long probing_offset = 0;
	while (1)
	{
		if ((academy->agent_id_hashtable[hashtable_index].state == ACADEMY_HASHTABLE_ROW_STATE_EMPTY) || (probing_offset > academy->agent_id_hashtable_max_offset))
		{
			break;
		}

		if (academy->agent_id_hashtable[hashtable_index].agent_id == agent_id)
		{
			agent = &academy->agents[academy->agent_id_hashtable[hashtable_index].agent_table_index];
			break;
		}

		probing_offset += 1;
		hashtable_index += 1;
		if (hashtable_index == ACADEMY_MAX_AGENT_COUNT)
		{
			hashtable_index = 0;
		}
	}

	/* Found the node, mark it as deleted. */
	if (agent)
	{
		academy->agent_id_hashtable[hashtable_index].state = ACADEMY_HASHTABLE_ROW_STATE_TOMBSTONE;
	}

	agent->academy->agent_count--;

}

void tree_search_test_prune_from_node(struct Academy_Agent_T * agent)
{
	struct Academy_T * academy = agent->academy;
	float threshold = academy->max_value * ((float)academy->agent_count/ACADEMY_MAX_AGENT_COUNT) + 0.001;
	int nodes_pruned = 0;
	for (int i = 0; i < ACADEMY_CHILDREN_PER_AGENT; i++)
	{
		if (academy->agent_count < 0.5*ACADEMY_MAX_AGENT_COUNT)
		{
			break;
		}
		struct Academy_Agent_T * child = academy_get_agent_from_id(academy, agent->children_ids[i]);
		if (child && agent->children_metadata[i].games_played > 100 && agent->children_metadata[i].value < threshold)
		{
			academy_prune_node(child);
			threshold = (float)academy->agent_count/ACADEMY_MAX_AGENT_COUNT;
			nodes_pruned = 1;
		}
	}
	if (nodes_pruned)
	{
		academy_update_probabilities(agent);
	}
}

void academy_update_probabilities(struct Academy_Agent_T * agent)
{
	// Score all candidates (first is parent)
	long i;
	double sum;

	struct Academy_Agent_T * agents[ACADEMY_CHILDREN_PER_AGENT];
	for (i = 0; i < ACADEMY_CHILDREN_PER_AGENT; i++)
	{
		agents[i] = academy_get_agent_from_id(agent->academy, agent->children_ids[i]);
	}

	// Start with agent value
	agent->own_metadata.probability = agent->own_metadata.value;
	// Do not allow own agent if all children exist
	if (agent->child_count >= ACADEMY_CHILDREN_PER_AGENT)
	{
		agent->own_metadata.probability = 0;
	}
	for (i = 0; i < ACADEMY_CHILDREN_PER_AGENT; i++)
	{
		if (agents[i])
		{
			agent->children_metadata[i].probability = agent->children_metadata[i].value;
		}
		else
		{
			agent->children_metadata[i].probability = 0;
		}
	}

	// Raise to powers
	agent->own_metadata.probability = pow(agent->own_metadata.probability, 10);
	for (i = 0; i < ACADEMY_CHILDREN_PER_AGENT; i++)
	{
		if (agents[i])
		{
			agent->children_metadata[i].probability = pow(agent->children_metadata[i].probability, 10);
		}
	}

	// Get sum
	sum = agent->own_metadata.probability;
	for (i = 0; i < ACADEMY_CHILDREN_PER_AGENT; i++)
	{
		if (agents[i])
		{
			sum += agent->children_metadata[i].probability;
		}
	}

	// Normalize
	agent->own_metadata.probability = agent->own_metadata.probability/sum;
	for (i = 0; i < ACADEMY_CHILDREN_PER_AGENT; i++)
	{
		if (agents[i])
		{
			agent->children_metadata[i].probability = agent->children_metadata[i].probability/sum;
		}
	}

	// Add UCB
	agent->own_metadata.probability += UCB_C*sqrt(ceil_log2(agent->queries_old+1) / ((double)agent->own_metadata.games_played+1));
	for (i = 0; i < ACADEMY_CHILDREN_PER_AGENT; i++)
	{
		if (agents[i])
		{
			agent->children_metadata[i].probability += UCB_C*sqrt(ceil_log2(agent->queries_old+1) / ((double)agent->children_metadata[i].games_played+1));
		}
	}

	// Set to 1 if values are all null
	if (sum == 0)
	{
		agent->own_metadata.probability = 1;
		for (i = 0; i < ACADEMY_CHILDREN_PER_AGENT; i++)
		{
			if (agents[i])
			{
				agent->children_metadata[i].probability = 1;
			}
		}
	}

	if (!isfinite(agent->own_metadata.probability))
	{
		printf("Infinity FTW!!!");
		abort();
	}

	// Get sum
	sum = agent->own_metadata.probability;
	for (i = 0; i < ACADEMY_CHILDREN_PER_AGENT; i++)
	{
		if (agents[i])
		{
			sum += agent->children_metadata[i].probability;
		}
	}

	if (!isfinite(agent->own_metadata.probability))
	{
		printf("Infinity FTW!!!");
		abort();
	}

	// Normalize
	agent->own_metadata.probability = agent->own_metadata.probability/sum;
	for (i = 0; i < ACADEMY_CHILDREN_PER_AGENT; i++)
	{
		agent->children_metadata[i].probability = agent->children_metadata[i].probability/sum;
	}

	if (!isfinite(agent->own_metadata.probability))
	{
		printf("Infinity FTW!!!");
		abort();
	}
}

void academy_select_matchup(struct Academy_T * academy, struct Academy_Agent_T ** agent1, struct Academy_Agent_T ** agent2)
{
	struct Academy_Agent_T * current_agent = academy_get_agent_from_id(academy, academy->root_agent_id);

	if (fast_rand()%100000 == 0)
	{
		academy_assert_sanity(academy);
	}

	long pass = 0;

	struct Academy_Agent_T * least_tried_fork_agent = current_agent;
	long least_tried_fork_tries = 10000000L;

	while(current_agent)
	{

		if (!isfinite(current_agent->own_metadata.probability))
		{
			printf("Infinity FTW!!!");
			abort();
		}

		// Randomly check for prunable nodes to keep memory usage sane
		if (fast_rand()%30 == 0 && pass == 0)
		{
			tree_search_test_prune_from_node(current_agent);
		}
		current_agent->queries_old++;

		// Select agent
		double selector = ((double)fast_rand())/FAST_RAND_MAX;
		selector -= current_agent->own_metadata.probability;

		if (selector <= 0)
		{
			// I select ME!!!
			if (current_agent->own_metadata.games_played < least_tried_fork_tries && current_agent->child_count > 0)
			{
				least_tried_fork_agent = current_agent;
				least_tried_fork_tries = current_agent->own_metadata.games_played;
			}

			if (pass == 0)
			{
				*agent1 = current_agent;
				pass = 1;
				current_agent = least_tried_fork_agent;
			}
			else
			{
				*agent2 = current_agent;
				return;
			}
		}
		else
		{
			// Iterate to a child
			for (unsigned long i = 0; i < ACADEMY_CHILDREN_PER_AGENT; i++)
			{
				selector -= current_agent->children_metadata[i].probability;
				if (selector <= 0)
				{
					current_agent->children_metadata[i].games_played++;
					if (current_agent->children_metadata[i].games_played < least_tried_fork_tries)
					{
						least_tried_fork_agent = current_agent;
						least_tried_fork_tries = current_agent->children_metadata[i].games_played;
					}
					struct Academy_Agent_T * next_agent = academy_get_agent_from_id(academy, current_agent->children_ids[i]);
					if (!next_agent)
					{
						printf("This is not right.\n");
					}
					current_agent = next_agent;
					break;
				}
			}
		}
	}
	abort();
}

void academy_add_new_agent(struct Academy_T * academy, struct Academy_Agent_T * parent, unsigned char * data, size_t data_len)
{
	ACADEMY_AGENT_ID agent_id = academy->last_agent_id + 1;

	// Verify that the academy list is not full
	if  (academy->agent_count >= ACADEMY_MAX_AGENT_COUNT)
	{
		return;
	}

	// First verify that the parent has an available slot
	unsigned long parent_child_index = ACADEMY_CHILDREN_PER_AGENT;
	unsigned long generation = 0;
	unsigned long parent_id = ACADEMY_INVALID_AGENT_ID;
	if (parent) {
		// Add child data to parent

		for (parent_child_index = 0; parent_child_index < ACADEMY_CHILDREN_PER_AGENT; parent_child_index++)
		{
			if (parent->children_ids[parent_child_index] == ACADEMY_INVALID_AGENT_ID)
			{
				break;
			}
		}
		if (parent_child_index == ACADEMY_CHILDREN_PER_AGENT)
		{
			/* No room in parent! */
			return;
		}

		parent->child_count++;

		parent->children_ids[parent_child_index] = agent_id;
		generation = parent->generation + 1;
		parent_id = parent->id;
	}
	else {
		academy->root_agent_id = agent_id;
	}

	// We are past the point of no return, we must succeed with this insertion!
	academy->last_agent_id = agent_id;

	// Find an open slot in agent memory
	// Insert into the Robin Hood table

	struct Academy_Agent_T * agent = NULL;
	unsigned long hashtable_index = academy_agent_id_hash(agent_id) % ACADEMY_MAX_AGENT_COUNT;
	unsigned long probing_offset = 0;

	unsigned long hashtable_index_for_new_agent;

	ACADEMY_AGENT_ID currently_relocating_agent_id = agent_id;
	unsigned long currently_relocating_agent_table_index = ACADEMY_MAX_AGENT_COUNT;
	while (1)
	{
		if (academy->agent_id_hashtable[hashtable_index].state == ACADEMY_HASHTABLE_ROW_STATE_EMPTY)
		{
			break;
		}

		/* Calculate the probing offset for the currently selected hashtable entry. */

		unsigned long selected_row_preferred_hashtable_index = academy_agent_id_hash(academy->agent_id_hashtable[hashtable_index].agent_id) % ACADEMY_MAX_AGENT_COUNT;
		unsigned long selected_row_offset = (hashtable_index + ACADEMY_MAX_AGENT_COUNT - selected_row_preferred_hashtable_index)%ACADEMY_MAX_AGENT_COUNT;

		if (selected_row_offset < probing_offset)
		{
			if (academy->agent_id_hashtable[hashtable_index].state == ACADEMY_HASHTABLE_ROW_STATE_TOMBSTONE)
			{
				// We do not need to relocate what is already here.
				break;
			}

			if (currently_relocating_agent_id == agent_id)
			{
				// This is the first relocation event we have found, mark this for the eventual resting place of the new agent.
				hashtable_index_for_new_agent = hashtable_index;
			}

			/* State of this hashtable row must already be set to FULL */

			// Swap the info we are currently carrying
			if (probing_offset > academy->agent_id_hashtable_max_offset)
			{
				academy->agent_id_hashtable_max_offset = probing_offset;
			}

			ACADEMY_AGENT_ID agent_id_for_insertion_here = currently_relocating_agent_id;
			unsigned long agent_table_index_for_insertion_here = currently_relocating_agent_table_index;
			currently_relocating_agent_id = academy->agent_id_hashtable[hashtable_index].agent_id;
			currently_relocating_agent_table_index = academy->agent_id_hashtable[hashtable_index].agent_table_index;
			academy->agent_id_hashtable[hashtable_index].agent_id = agent_id_for_insertion_here;
			academy->agent_id_hashtable[hashtable_index].agent_table_index = agent_table_index_for_insertion_here;

			probing_offset = selected_row_offset;
		}

		probing_offset += 1;
		hashtable_index += 1;
		if (hashtable_index == ACADEMY_MAX_AGENT_COUNT)
		{
			hashtable_index = 0;
		}
	}
	if (currently_relocating_agent_id == agent_id)
	{
		hashtable_index_for_new_agent = hashtable_index;
	}
	else
	{
		// Set the last (empty) slot to the data we were relocating .
		ACADEMY_AGENT_ID agent_id_for_insertion_here = currently_relocating_agent_id;
		unsigned long agent_table_index_for_insertion_here = currently_relocating_agent_table_index;
		currently_relocating_agent_table_index = academy->agent_id_hashtable[hashtable_index].agent_table_index;
		academy->agent_id_hashtable[hashtable_index].agent_id = agent_id_for_insertion_here;
		academy->agent_id_hashtable[hashtable_index].agent_table_index = agent_table_index_for_insertion_here;
		academy->agent_id_hashtable[hashtable_index].state = ACADEMY_HASHTABLE_ROW_STATE_FULL;
		// Use the unused agent_table_index for the new agent
		academy->agent_id_hashtable[hashtable_index_for_new_agent].agent_table_index = currently_relocating_agent_table_index;
	}

	if (probing_offset > academy->agent_id_hashtable_max_offset)
	{
		academy->agent_id_hashtable_max_offset = probing_offset;
	}

	academy->agent_id_hashtable[hashtable_index_for_new_agent].agent_id = agent_id;
	academy->agent_id_hashtable[hashtable_index_for_new_agent].state = ACADEMY_HASHTABLE_ROW_STATE_FULL;
	agent = &academy->agents[academy->agent_id_hashtable[hashtable_index_for_new_agent].agent_table_index];

	// Resume refactor here
	agent->academy = academy;
	agent->id = agent_id;
	agent->generation = generation;
	agent->parent_id = parent_id;
	agent->parent_child_index = parent_child_index;

	agent->queries_old = 0;

	agent->own_metadata.probability = 0;
	agent->own_metadata.games_played = 0;
	agent->own_metadata.points = 0;

	if (parent)
	{
		agent->own_metadata.value = parent->own_metadata.value;
	}
	else
	{
		agent->own_metadata.value = 0.5;
	}

	agent->child_count = 0;
	for (unsigned long i = 0; i < ACADEMY_CHILDREN_PER_AGENT; i++)
	{
		agent->children_ids[i] = ACADEMY_INVALID_AGENT_ID;
	}
	if (data_len > ACADEMY_AGENT_DATA_LEN)
	{
		data_len = ACADEMY_AGENT_DATA_LEN;
	}
	memcpy(agent->data, data, data_len);
	agent->data_len = data_len;

	academy->agent_count++;
	if (parent)
	{
		/* Set subtree probabilities to default. */
		parent->children_metadata[parent_child_index].games_played = 0;
		parent->children_metadata[parent_child_index].points = 0;
		parent->children_metadata[parent_child_index].value = parent->own_metadata.value;
		academy_update_probabilities(parent);
	}
	academy_update_probabilities(agent);

}

#define UPDATE_VAL 0.1

void academy_agent_metadata_update_for_new_game(struct Academy_Agent_Metadata_T * metadata, float points_gained)
{
	metadata->games_played++;
	metadata->points += points_gained;

	//metadata->value = metadata->points/metadata->games_played;
	metadata->value =  (metadata->value * (1.0 - UPDATE_VAL)) + (points_gained * UPDATE_VAL);
	//if (points_gained > metadata->value)
	//	metadata->value = points_gained;
}

void academy_report_agent_win(struct Academy_Agent_T * winner, float winner_points, struct Academy_Agent_T * looser, float looser_points)
{
	struct Academy_Agent_T * winner_child = winner;
	struct Academy_Agent_T * looser_child = looser;
	struct Academy_T * academy = winner->academy;

	if (winner_points > winner->academy->max_value)
		winner->academy->max_value = winner_points;

	/* Find the common ancestor of the winner and looser. */

	while (winner != looser) {
		struct Academy_Agent_T * next_winner = winner;
		struct Academy_Agent_T * next_looser = looser;
		if (winner->generation >= looser->generation) {
			next_winner = academy_get_agent_from_id(academy, winner->parent_id);
			winner_child = winner;
		}
		if (winner->generation <= looser->generation) {
			next_looser = academy_get_agent_from_id(academy, looser->parent_id);
			looser_child = looser;
		}
		winner = next_winner;
		looser = next_looser;
	}
	// Now winner == looser

	// use winner_child and looser_child to figure out where scores go

	// Winner scores
	if (winner == winner_child)
	{
		academy_agent_metadata_update_for_new_game(&(winner->own_metadata), winner_points);
	}
	else
	{
		academy_agent_metadata_update_for_new_game(&(winner->children_metadata[winner_child->parent_child_index]), winner_points);
	}

	// Looser scores
	if (looser == looser_child)
	{
		academy_agent_metadata_update_for_new_game(&(looser->own_metadata), looser_points);
	}
	else
	{
		academy_agent_metadata_update_for_new_game(&(looser->children_metadata[looser_child->parent_child_index]), looser_points);
	}

	academy_update_probabilities(winner);
}

void export_agent_nodes(struct Academy_Agent_T * agent, FILE * fp)
{
	unsigned long i;

	int subtree_games_played = -1;
	float subtree_value = -1;
	double subtree_probability = -1;

	if (agent->parent_id != ACADEMY_INVALID_AGENT_ID)
	{
		struct Academy_Agent_T * parent = academy_get_agent_from_id(agent->academy, agent->parent_id);
		subtree_games_played = parent->children_metadata[agent->parent_child_index].games_played;
		subtree_value = parent->children_metadata[agent->parent_child_index].value;
		subtree_probability = parent->children_metadata[agent->parent_child_index].probability;
	}

	fprintf(fp, "<node id=\"%ld\">\n", agent->id);
	fprintf(fp, "<attvalues>\n");
	fprintf(fp, "<attvalue for=\"0\" value=\"%ld\"/>\n", agent->generation);
	fprintf(fp, "<attvalue for=\"1\" value=\"%ld\"/>\n", agent->queries_old);
	fprintf(fp, "<attvalue for=\"2\" value=\"%ld\"/>\n", agent->own_metadata.games_played);
	fprintf(fp, "<attvalue for=\"3\" value=\"%f\"/>\n", agent->own_metadata.value);
	fprintf(fp, "<attvalue for=\"4\" value=\"%f\"/>\n", agent->own_metadata.probability);
	fprintf(fp, "<attvalue for=\"5\" value=\"%i\"/>\n", subtree_games_played);
	fprintf(fp, "<attvalue for=\"6\" value=\"%f\"/>\n", subtree_value);
	fprintf(fp, "<attvalue for=\"7\" value=\"%f\"/>\n", subtree_probability);
	fprintf(fp, "</attvalues>\n");
	fprintf(fp, "</node>\n");
	for (i = 0; i < ACADEMY_CHILDREN_PER_AGENT; i++)
	{
		if (agent->children_ids[i] != ACADEMY_INVALID_AGENT_ID)
		{
			export_agent_nodes(academy_get_agent_from_id(agent->academy, agent->children_ids[i]), fp);
		}
	}
}

void export_agent_edges(struct Academy_Agent_T * agent, FILE * fp)
{
	unsigned long i;
	for (i = 0; i < ACADEMY_CHILDREN_PER_AGENT; i++)
	{
		if (agent->children_ids[i] != ACADEMY_INVALID_AGENT_ID)
		{
			fprintf(fp, "<edge source=\"%ld\" target=\"%ld\"/>\n", agent->id, agent->children_ids[i]);
			export_agent_edges(academy_get_agent_from_id(agent->academy, agent->children_ids[i]), fp);
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
    fprintf(fp, "<attribute id=\"1\" title=\"Queries Old\" type=\"integer\"/>\n");
    fprintf(fp, "<attribute id=\"2\" title=\"Own Games Played\" type=\"integer\"/>\n");
    fprintf(fp, "<attribute id=\"3\" title=\"Own Value\" type=\"float\"/>\n");
    fprintf(fp, "<attribute id=\"4\" title=\"Own Probability\" type=\"float\"/>\n");
    fprintf(fp, "<attribute id=\"5\" title=\"Subtree Games Played\" type=\"integer\"/>\n");
    fprintf(fp, "<attribute id=\"6\" title=\"Subtree Value\" type=\"float\"/>\n");
    fprintf(fp, "<attribute id=\"7\" title=\"Subtree Probability\" type=\"float\"/>\n");
    fprintf(fp, "</attributes>\n");
    fprintf(fp, "<nodes>\n");
    export_agent_nodes(academy_get_agent_from_id(academy, academy->root_agent_id), fp);
    fprintf(fp, "</nodes>\n");
    fprintf(fp, "<edges>\n");
    export_agent_edges(academy_get_agent_from_id(academy, academy->root_agent_id), fp);
    fprintf(fp, "</edges>\n");
    fprintf(fp, "</graph>\n");
    fprintf(fp, "</gexf>\n");
    fclose(fp);
}
