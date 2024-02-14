#include "genetics.hpp"
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <random>		/* random_device, mt19937 */
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>
#include <cmath>
#include <chrono>
#include "TetrisGame/tetrisModule.hpp"


/*
	I was talking with Dad about this and we came up with a few ways we think that the algorithm could be improved
	1) Right now we are looking at the average fitness of the entire generation to see how good a thing is doing.
	   Could we instead do something where we keep track of a lineage and see specifically how that lineage is doing?
	   Idk how to think about this precisely, but I have a few ideas
	   a) So instead of mutating randomly across the entire population, we instead only mutate inside of specific
	   	  lineages. So like, you kinda do 2 evolution schemes. 2 layers. On the innermost layer you only do evolution
		  in a specific family. You pick two parents and they become the parents of a family. You do evolution on that
		  family for a certain amount of time then you pick parents by finding the average score of that family and
		  using that
	   b) We could keep track of the lineage each individual has and find the average of the ones that share a common
		  n-long lineage and if that average gets too low, prune them from the population pool
	   c) We could also do some analysis ninjitzu and look at a lineage to see if an individual produces a statistical
	      outlier that is above the lineage average, see what makes that individual different, and then either
		  i  ) Do a smaller evolution train and try to find what the optimal value for the things that changed were
		  ii ) Immediately crossover everyone in that lineage with the statistical anomaly in the hopes that another
		  	   anomaly will form soon after
	2) I'll put this in the game script too, but why don't we split up the board into 3 sections and each has their
	   own configuration/training done for it. We might have to reconsider how to evolve all 3 whether as a unit or
	   separate
	3) Me note, but just find the median of the scores you got for a game instead of taking the average
	4) Part of fitness should be the number of Tetrises you get
	5) Don't just test a config by putting it through 3 games. Test it in different scenarios (screen 1/2, 1/4, 8/10
	   full) that are in differing conditions
	6) Switch fitness values over time, maybe every 75 generations you switch between trying to maximize number of
	   tetrises and maximizing the score. Also think about how to manipulate mutation rate. Or add extinction events
*/

void generateInitialPopulation(std::mt19937 &generator, AI_InstanceHandler (&population)[POP_SIZE]) {
	for (int i = 0; i < POP_SIZE; i++) {
		population[i].initialize(WEIGHT_SIZE, generator);
	}
}

void evaluateFitness(AI_InstanceHandler (&population)[POP_SIZE], int seeds[4], std::ofstream &progressFile, int generationNumber) {

	// Run through each AI and test them over n games, average the result

	// We want to test each AI instance on the same set of blocks
	// So we create an identical generator for each of them
	
	int pieceCount = 700;

	bool useScore = true;

	for (int i = 0; i < POP_SIZE; i++) {

		double avgScore = 0;
		double avgLines = 0;
		double avgRatio = 0;
		double avgPiecesUsed = 0;
		double avgTetrises = 0;

		if (i % 5 == 0) progressFile << "Evaluating " << i << "th instance" << std::endl;


		for (int j = 0; j < 4; j++) {

			int results[4] = {};
			
			std::mt19937 generatorForGame(seeds[j]);

			// Run game...
			runTetrisGame(population[i], pieceCount, results, generatorForGame);


			avgScore += results[0];
			avgLines += results[1];
			avgPiecesUsed += results[2];
			avgTetrises += results[3];
			if (results[1] > 0) {
				avgRatio += (results[0] / results[1]);
			}
		}

		avgScore /= 4;
		avgLines /= 4;
		avgRatio /= 4;
		avgPiecesUsed /= 4;
		avgTetrises /= 4;

		population[i].scoreResult = avgScore;
		population[i].linesResult = avgLines;

		if (useScore) {
			population[i].fitness = avgScore;
		} else {
			population[i].fitness = avgTetrises;
		}

		// Uncomment this if you want to try to maximize ratio

		// In my experience, this isn't very fast but if you plot the average
		// score it shows some interesting behavior
		
		// if (avgLines == 0) {
		// 	population[i].fitness = 0;
		// } else {
		// 	population[i].fitness = avgScore / avgLines;
		// }
	}
}

void popNewGen(AI_InstanceHandler (&population)[POP_SIZE], std::mt19937 &generator, std::ofstream &logFile) {

	// Decide the parents first
	// Create empty parents array
	AI_InstanceHandler parents[POP_SIZE] = {};

	for (int i = 0; i < POP_SIZE; i++) {
		
		// Randomly select 5 potential parents (indexes)
		const int p1 = rand() % POP_SIZE;
		const int p2 = rand() % POP_SIZE;
		const int p3 = rand() % POP_SIZE;
		const int p4 = rand() % POP_SIZE;
		const int p5 = rand() % POP_SIZE;

		// Select the best parent out of all of them
		double bestScore = population[p1].fitness;
		int best = p1;
		if (population[p2].fitness > bestScore) {
			bestScore = population[p2].fitness;
			best = p2; 
		}
		if (population[p3].fitness > bestScore) {
			bestScore = population[p3].fitness;
			best = p3; 
		}
		if (population[p4].fitness > bestScore) {
			bestScore = population[p4].fitness;
			best = p4; 
		}
		if (population[p5].fitness > bestScore) {
			bestScore = population[p5].fitness;
			best = p5; 
		}


		// Set the tournament winner as the parent
		parents[i] = population[best];
	}


	// Now populate the new generation
	population[0].crossover(parents[0], parents[POP_SIZE - 1]);
	for (int i = 1; i < POP_SIZE; i++) {
		population[i].crossover(parents[i-1], parents[i]);
	}

	// And apply mutations
	int mutationCount = 0;
	for (int i = 0; i < POP_SIZE; i++) {
		mutationCount += population[i].applyMutation(MUTATE_RATE, WEIGHT_SIZE, generator);
	}

	// progressFile << "Mutations applied" << std::endl;
	logFile << "Applied " << mutationCount << " mutations" << std::endl;
}

int findBestAIIndex(const AI_InstanceHandler (&population)[POP_SIZE]) {
	int bestIndex = 0;
	double bestFitness = population[0].fitness;
	for (int i = 1; i < POP_SIZE; i++) {
		if (population[i].fitness > bestFitness) {
			bestIndex = i;
			bestFitness = population[i].fitness;
		}
	}

	return bestIndex;
}

void generationTrainingInfo(int (&seeds)[4], int &lastTrainedInstance) {
	
}

int main() {
	
	// Before we do anything, seed the random number generator
	srand((unsigned) time(NULL));

	// std::random_device rd;
	std::mt19937 generator(time(0));

	// Create our population
	AI_InstanceHandler population[POP_SIZE] = {};

	int genNumber = 1;

	// Goal right now
	std::string significantChange = "Catering to individual pieces in each section";

	// Create log file
	std::string logPrefix = "cateringToPieces";
		

	std::string logName = logPrefix + ".txt";
	std::ofstream logFile(logName, std::ios_base::app);

	// A log to keep track of where progress currently is
	// More for error debugging than anything
	std::string progressName = logPrefix + "Progress.txt";
	std::ofstream progressFile(progressName, std::ios_base::app);

	// This is what will keep track of progress in the current generation.
	// These things take increasingly long to train, so I'd rather save work
	// where I can if something explodes unexpectedly.

	// Try to load in the last trained generation
	const bool didLoad = AI_InstanceHandler::deserializeAll(population, genNumber, logPrefix);

	// If we weren't able to load the previous generation in, create a new one
	if (!didLoad) {
		logFile << "Couldn't find any previous population\n";
		generateInitialPopulation(generator, population);
		AI_InstanceHandler::serializeAll(population, genNumber, logPrefix);
	} else {
		logFile << "Found previous population at generation " << genNumber << "\n";
	}
	logFile << "Using 3 tiered scoring approach 8:7:5" << std::endl;
	logFile << "Training Parameters:" << std::endl;
	logFile << "Weight size: " << WEIGHT_SIZE << std::endl;
	logFile << "Generation limit: " << GENERATION_LIMIT << std::endl;
	logFile << "Mutation rate: " << MUTATE_RATE << std::endl;
	logFile << "Piece count: " << "Static 700" << std::endl;
	logFile << "Population size: " << POP_SIZE << std::endl;
	logFile << "File information prefix is \"" << logPrefix << "\"" << std::endl;
	logFile << "What changed: \"" << significantChange << "\"" << std::endl;

	try {
		for (int generation = 0; generation < GENERATION_LIMIT; generation++) {


			logFile << "Training generation " << genNumber << std::endl;
			progressFile << "Started evaluating" << std::endl;

			// Evaluate fitness
			auto evaluationStartTime = std::chrono::high_resolution_clock::now();

			// Create the seed values
			int seeds[4];
			for (int i = 0; i < 4; i++) {
				seeds[i] = generator();
			}
			
			evaluateFitness(population, seeds, progressFile, genNumber);

			auto evaluationEndTime = std::chrono::high_resolution_clock::now();
			auto durationSeconds = std::chrono::duration_cast<std::chrono::seconds>(evaluationEndTime - evaluationStartTime);
			auto averageDuration = durationSeconds.count() / static_cast<float>(POP_SIZE);

			progressFile << "Finished evaluating generation in " << durationSeconds.count() << " seconds" << std::endl;

			// We don't need to change up the population on the last round
			if (generation == GENERATION_LIMIT - 1) {
				break;
			}

			// Find the best model to display of the generation
			const int bestAI = findBestAIIndex(population);
			// double meanFitness = 0;
			// for (int i = 0; i < POP_SIZE; i++) {
			// 	meanFitness += population[i].fitness;
			// }
			// meanFitness /= POP_SIZE;
			double medianFitness = population[POP_SIZE / 2].fitness;


			logFile << "Generation evaluated. Best fitness " << round(population[bestAI].fitness);
			logFile << " with " << population[bestAI].linesResult << " lines cleared and a score of " << population[bestAI].scoreResult << "." << std::endl;
			logFile << "Median fitness: " << medianFitness << "." << std::endl;
			logFile << "Average time per evaluation was " << averageDuration << " seconds." << std::endl;
			logFile << "Best AI was..." << std::endl;
			logFile << population[bestAI].displayHandler();

			// Go onto the next generation
			progressFile << "Populating new generation" << std::endl;
			popNewGen(population, generator, logFile);
			genNumber++;

			// Write this to disk so we can access it next time we try to start
			progressFile << "Writing current pop to disk" << std::endl;
			AI_InstanceHandler::serializeAll(population, genNumber, logPrefix);

		}
		
	} catch (const std::exception &e) {
		// Unfortunately, this does not seem to work with a stack overflow
		// error. Which sucks if you've never dealt with it outside of
		// recursion. And you spend 5 hours looking for the bug. Ughhhhh
		progressFile << "Exception: " << e.what() << std::endl;
		return 0;
	}


}