/*
 * simulated_annealing.c
 *
 *  Created on: Nov 16, 2020
 *      Author: christian
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "problems/problems.h"
#include "types.h"
#include "utils.h"


float best_score;

int last_iterations = 0;
double last_progress_time = 0;

void report_progress(struct Problem_T * problem, U8 * data, float score, int current_cycle, int current_iteration, int iterations_in_cycle, int total_iterations) {

  printf("\033c"); // Clear screen
  if (current_cycle >= 0)
    printf("Cycle: #%i \n" , current_cycle);
  printf("Current Score = %f \n", score);
  printf("Best Score = %f \n", best_score);
  printf("Current rate = %d trials/s \n", (int)((total_iterations-last_iterations)/(getUnixTime() - last_progress_time)));
  printf("Iterations = %'i / %'i\n\n", current_iteration, iterations_in_cycle);
  printf("Current data:\n");
  problem->data_pretty_printer(problem, data);
  last_progress_time = getUnixTime();
  last_iterations = total_iterations;
}


U8 * simulated_annealing(struct Problem_T * problem)
{
	U8 * data = malloc(problem->data_len);
	U8 * data_b = malloc(problem->data_len);
	problem->data_initializer(problem, data);
	memcpy(data_b, data, problem->data_len);

	float score = problem->scalar_trial(problem, data);
	float current_score_total = score;
	int current_score_count = 1;
	best_score = score;

	double last_update = getUnixTime();

	int last_batch_score = 0;
	long last_batch_iterations = 0;
	int batch_num = 0;

	long max_iterations = 1000000;
	long total_iterations = 0;
	while (1) {
	    long i;
	    for (i = 0; i < max_iterations; i++)
	    {
	    	//sanity_check(data);
	    	// Copy data to data_b and modify it
	    	memcpy(data_b, data, problem->data_len);

	    	if ((i%10) == 0)
	    	{

				// Average prev score with new run
				current_score_total += problem->scalar_trial(problem, data);
				current_score_count += 1;
				score = current_score_total/current_score_count;
	    	}

			/* Make a random change and introduce a new agent */
	    	problem->scrambler(problem, data);

	    	// Score run
	    	float new_score = problem->scalar_trial(problem, data);


	    	// Simulated annealing to determine if this passes
	    	int does_pass = 0;
	    	if (new_score >= score)
	    	{
	    		does_pass = 1;
	    	}
	    	else {
	    		float t = (float)max_iterations/(float)(i);
	    		float p = exp((float)((new_score - score)*10000.0)/(t));
	        	does_pass = fast_rand() < (32767.0 * p);
	        }
		    if (does_pass) {
			  	if (new_score > best_score)
			  	{
				    best_score = new_score;
			  	}
				// Keep the new code
				score = new_score;
				current_score_total = score;
				current_score_count = 1;
	        }
		    else
		    {
				// Revert to the old code
				U8* t = data_b;
				data_b = data;
				data = t;
	        }
	        if (!(i % 100) && (getUnixTime()-last_update > 0.5)) {
	            last_update += 0.5;
	            report_progress(problem, data, score, batch_num, i, max_iterations, total_iterations);
	        }
	        total_iterations++;
	    }
	    if (score >= 0.9999) {
	      report_progress(problem, data, score, batch_num, i, max_iterations, total_iterations);
	      printf("Goal achieved, keep going? (Y/N)");
	      char buff[10];
	      gets(buff);
	      if (buff[0] != 'Y' && buff[0] != 'y')
	        break;
	      last_update = getUnixTime();
	    }
	    last_batch_score = score;
	    last_batch_iterations = max_iterations;
	    max_iterations = max_iterations * 1.5;
	    batch_num++;
	  }


	free(data_b);
	return data;
}
