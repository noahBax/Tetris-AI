#include "ai_configuration.hpp"
#include <stdlib.h>     /* srand rand */
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <regex>

void AI_Configuration::initialize(double weightSize, std::mt19937 &generator) {

    std::uniform_real_distribution<> distrLarge(-weightSize, weightSize);
    std::uniform_real_distribution<> distrSmall(-0.5, 5);
    std::uniform_real_distribution<> distrDiffs(0.0, 20.0);

	// Go through and assign random values

	// for (int i = 0; i < sizeof(coefficientGenes) / sizeof(coefficientGenes[0]); i++) {
	// 	*coefficientGenes[i] = distrLarge(generator);
	// }
	crackiness = distrLarge(generator);
	coveredHoles = distrLarge(generator);
	wellDepth = distrLarge(generator);
	aggregateHeight = distrLarge(generator);
	linesCleared = distrLarge(generator);
	bumpiness = distrLarge(generator);
	highestBlock = distrLarge(generator);
	scoreValue = distrLarge(generator);
	averageColumnLoad = distrLarge(generator);
	rightColumn = distrLarge(generator);
	movedDown = distrLarge(generator);
	boardClearScore = distrLarge(generator);
	iPenalty = distrLarge(generator);

	// for (int i = 0; i < sizeof(pieceDiffMultipliers) / sizeof(pieceDiffMultipliers[0]); i++) {
	// 	*pieceDiffMultipliers[i] = distrLarge(generator);
	// }
	piece0DifferenceMultiplier = distrLarge(generator);
	piece1DifferenceMultiplier = distrLarge(generator);
	piece2DifferenceMultiplier = distrLarge(generator);
	piece3DifferenceMultiplier = distrLarge(generator);
	piece4DifferenceMultiplier = distrLarge(generator);
	piece5DifferenceMultiplier = distrLarge(generator);
	piece6DifferenceMultiplier = distrLarge(generator);

	// for (int i = 0; i < sizeof(exponentGenes) / sizeof(exponentGenes[0]); i++) {
	// 	*exponentGenes[i] = distrSmall(generator);
	// }
	wellExponent = distrSmall(generator);
	crackinessExponent = distrSmall(generator);
	coveredExponent = distrSmall(generator);
	scoreExponent = distrSmall(generator);
	perfectScoreExponent = distrSmall(generator);

	// for (int i = 0; i < sizeof(pieceLevels) / sizeof(pieceLevels[0]); i++) {
	// 	*pieceLevels[i] = distrDiffs(generator);
	// }
	piece0Level = distrDiffs(generator);
	piece1Level = distrDiffs(generator);
	piece2Level = distrDiffs(generator);
	piece3Level = distrDiffs(generator);
	piece4Level = distrDiffs(generator);
	piece5Level = distrDiffs(generator);
	piece6Level = distrDiffs(generator);
}

void AI_Configuration::crossover(AI_Configuration &parent1, AI_Configuration &parent2) {

	// Compute the average of all the values
	// for (int i = 0; i < genomeSize; i++) {
	// 	*genome[i] = (*parent1.genome[i] + *parent2.genome[i]) / 2.0;
	// }

	// This makes absolutely no sense to me and I will chalk it up to my lack of
	// experience here, but the above loop can cause genome[i] to be set to
	// parent1.genome[i]. Idk why. Maybe it has something to do with pointers or
	// maybe it is an issue with C++ itself (unlikely) or maybe I'm just an
	// idiot and missing something obvious. I might have something else messed
	// up in here somewhere too which could be causing it.

	// It's not rounding errors. 

	// Either way, I've decided to go with this stupid approach below. It seems
	// to work without error so I'm fine with that.

	// I do with I didn't have to do this because it is VERY ugly, but I'm at a
	// loss for what I should really do here.

	// But really, this is not the part of the code that needs the optimizations
	// so I don't really care that much.

	crackiness = (parent1.crackiness + parent2.crackiness) / 2.0;
	coveredHoles = (parent1.coveredHoles + parent2.coveredHoles) / 2.0;
	wellDepth = (parent1.wellDepth + parent2.wellDepth) / 2.0;
	aggregateHeight = (parent1.aggregateHeight + parent2.aggregateHeight) / 2.0;
	linesCleared = (parent1.linesCleared + parent2.linesCleared) / 2.0;
	bumpiness = (parent1.bumpiness + parent2.bumpiness) / 2.0;
	highestBlock = (parent1.highestBlock + parent2.highestBlock) / 2.0;
	scoreValue = (parent1.scoreValue + parent2.scoreValue) / 2.0;
	averageColumnLoad = (parent1.averageColumnLoad + parent2.averageColumnLoad) / 2.0;
	wellExponent = (parent1.wellExponent + parent2.wellExponent) / 2.0;
	rightColumn = (parent1.rightColumn + parent2.rightColumn) / 2.0;
	crackinessExponent = (parent1.crackinessExponent + parent2.crackinessExponent) / 2.0;
	coveredExponent = (parent1.coveredExponent + parent2.coveredExponent) / 2.0;
	movedDown = (parent1.movedDown + parent2.movedDown) / 2.0;
	boardClearScore = (parent1.boardClearScore + parent2.boardClearScore) / 2.0;
	scoreExponent = (parent1.scoreExponent + parent2.scoreExponent) / 2.0;
	perfectScoreExponent = (parent1.perfectScoreExponent + parent2.perfectScoreExponent) / 2.0;
	iPenalty = (parent1.iPenalty + parent2.iPenalty) / 2.0;
	piece0Level = (parent1.piece0Level + parent2.piece0Level) / 2.0;
	piece1Level = (parent1.piece1Level + parent2.piece1Level) / 2.0;
	piece2Level = (parent1.piece2Level + parent2.piece2Level) / 2.0;
	piece3Level = (parent1.piece3Level + parent2.piece3Level) / 2.0;
	piece4Level = (parent1.piece4Level + parent2.piece4Level) / 2.0;
	piece5Level = (parent1.piece5Level + parent2.piece5Level) / 2.0;
	piece6Level = (parent1.piece6Level + parent2.piece6Level) / 2.0;
	piece0DifferenceMultiplier = (parent1.piece0DifferenceMultiplier + parent2.piece0DifferenceMultiplier) / 2.0;
	piece1DifferenceMultiplier = (parent1.piece1DifferenceMultiplier + parent2.piece1DifferenceMultiplier) / 2.0;
	piece2DifferenceMultiplier = (parent1.piece2DifferenceMultiplier + parent2.piece2DifferenceMultiplier) / 2.0;
	piece3DifferenceMultiplier = (parent1.piece3DifferenceMultiplier + parent2.piece3DifferenceMultiplier) / 2.0;
	piece4DifferenceMultiplier = (parent1.piece4DifferenceMultiplier + parent2.piece4DifferenceMultiplier) / 2.0;
	piece5DifferenceMultiplier = (parent1.piece5DifferenceMultiplier + parent2.piece5DifferenceMultiplier) / 2.0;
	piece6DifferenceMultiplier = (parent1.piece6DifferenceMultiplier + parent2.piece6DifferenceMultiplier) / 2.0;

}

int AI_Configuration::applyMutation(int mutationRate, double weightSize, std::mt19937 &generator) {

	if (rand() % 100 > mutationRate) return 0;

	int mutationCount = 0;
	int mutationsLeft = (rand() % 3) + 2;

	while (mutationsLeft > 0) {
		if (rand() % 100 < 20) {
			// We do a swap between two random mutations here
			mutationCount += 2;
			mutationsLeft -= 2;

			// Pick 2 genes to swap
			int ind1 = rand() % AI_Configuration::genomeSize;
			int ind2 = rand() % AI_Configuration::genomeSize;

			// Swap
			double temp = getGeneValueByIndex(ind1);
			alterGeneByIndex(ind1, getGeneValueByIndex(ind2));
			alterGeneByIndex(ind2, temp);
		} else {
			mutationCount++;
			mutationsLeft--;

			randomizeRandomGeneRandomly(generator, weightSize);
		}
	}

	return mutationCount;
}

AI_Configuration::AI_Configuration() {}

AI_Configuration::AI_Configuration(double genes[genomeSize]) {

	// for (int i = 0; i < genomeSize; i++) {
	// 	*genome[i] = genes[i];
	// }
}


// Yes yes, redundant. Simplify it when you know what you are doing for sure.
void AI_Configuration::alterGeneByIndex(int index, double newValue) {
	
	// if (index < 0 || index >= genomeSize) {
	// 	throw std::invalid_argument("Index received for altering gene is invalid value");
	// }

	// *genome[index] = newValue;

	switch (index) {
		case 0:
			crackiness = newValue;
			break;
		case 1:
			coveredHoles = newValue;
			break;
		case 2:
			wellDepth = newValue;
			break;
		case 3:
			aggregateHeight = newValue;
			break;
		case 4:
			linesCleared = newValue;
			break;
		case 5:
			bumpiness = newValue;
			break;
		case 6:
			highestBlock = newValue;
			break;
		case 7:
			scoreValue = newValue;
			break;
		case 8:
			averageColumnLoad = newValue;
			break;
		case 9:
			wellExponent = newValue;
			break;
		case 10:
			rightColumn = newValue;
			break;
		case 11:
			crackinessExponent = newValue;
			break;
		case 12:
			coveredExponent = newValue;
			break;
		case 13:
			movedDown = newValue;
			break;
		case 14:
			boardClearScore = newValue;
			break;
		case 15:
			scoreExponent = newValue;
			break;
		case 16:
			perfectScoreExponent = newValue;
			break;
		case 17:
			iPenalty = newValue;
			break;
		case 18:
			piece0Level = newValue;
			break;
		case 19:
			piece1Level = newValue;
			break;
		case 20:
			piece2Level = newValue;
			break;
		case 21:
			piece3Level = newValue;
			break;
		case 22:
			piece4Level = newValue;
			break;
		case 23:
			piece5Level = newValue;
			break;
		case 24:
			piece6Level = newValue;
			break;
		case 25:
			piece0DifferenceMultiplier = newValue;
			break;
		case 26:
			piece1DifferenceMultiplier = newValue;
			break;
		case 27:
			piece2DifferenceMultiplier = newValue;
			break;
		case 28:
			piece3DifferenceMultiplier = newValue;
			break;
		case 29:
			piece4DifferenceMultiplier = newValue;
			break;
		case 30:
			piece5DifferenceMultiplier = newValue;
			break;
		case 31:
			piece6DifferenceMultiplier = newValue;
			break;
		default:
			throw std::invalid_argument("Index received for altering gene is invalid value");
	}
	
}

void AI_Configuration::randomizeRandomGeneRandomly(std::mt19937 &generator, double weightSize) {

	// Pick a random weight to mutate
	int index = rand() % AI_Configuration::genomeSize;

	// if (index < 7) { // Piece level genes

	// 	std::uniform_real_distribution<> distrDiffs(0, 20.0);
	// 	*pieceLevels[index] = distrDiffs(generator);

	// } else if (index < 14) { // Piece difference genes

	// 	std::uniform_real_distribution<> distrLarge(-weightSize, weightSize);
	// 	index -= 7;
	// 	*pieceDiffMultipliers[index] = distrLarge(generator);

	// } else if (index < 14 + sizeof(coefficientGenes) / sizeof(coefficientGenes[0])) { // Coefficients

	// 	std::uniform_real_distribution<> distrLarge(-weightSize, weightSize);
	// 	index -= 14;
	// 	*coefficientGenes[index] = distrLarge(generator);
		
	// } else { // Exponents

	// 	index -= (14 + sizeof(coefficientGenes) / sizeof(coefficientGenes[0]));
	// 	std::uniform_real_distribution<> distrSmall(-5.5, 5.5);
	// 	wellExponent = distrSmall(generator);

	// }

	std::uniform_real_distribution<> distrLarge(-weightSize, weightSize);
	std::uniform_real_distribution<> distrSmall(-5.5, 5.5);
	std::uniform_real_distribution<> distrDiffs(0, 20.0);


	switch (index) {
		case 0:
			crackiness = distrLarge(generator);
			break;
		case 1:
			coveredHoles = distrLarge(generator);
			break;
		case 2:
			wellDepth = distrLarge(generator);
			break;
		case 3:
			aggregateHeight = distrLarge(generator);
			break;
		case 4:
			linesCleared = distrLarge(generator);
			break;
		case 5:
			bumpiness = distrLarge(generator);
			break;
		case 6:
			highestBlock = distrLarge(generator);
			break;
		case 7:
			scoreValue = distrLarge(generator);
			break;
		case 8:
			averageColumnLoad = distrLarge(generator);
			break;
		case 9:
			wellExponent = distrSmall(generator);
			break;
		case 10:
			rightColumn = distrLarge(generator);
			break;
		case 11:
			crackinessExponent = distrSmall(generator);
			break;
		case 12:
			coveredExponent = distrSmall(generator);
			break;
		case 13:
			movedDown = distrLarge(generator);
			break;
		case 14:
			boardClearScore = distrLarge(generator);
			break;
		case 15:
			scoreExponent = distrSmall(generator);
			break;
		case 16:
			perfectScoreExponent = distrSmall(generator);
			break;
		case 17:
			iPenalty = distrLarge(generator);
			break;
		case 18:
			piece0Level = distrDiffs(generator);
			break;
		case 19:
			piece1Level = distrDiffs(generator);
			break;
		case 20:
			piece2Level = distrDiffs(generator);
			break;
		case 21:
			piece3Level = distrDiffs(generator);
			break;
		case 22:
			piece4Level = distrDiffs(generator);
			break;
		case 23:
			piece5Level = distrDiffs(generator);
			break;
		case 24:
			piece6Level = distrDiffs(generator);
			break;
		case 25:
			piece0DifferenceMultiplier = distrLarge(generator);
			break;
		case 26:
			piece1DifferenceMultiplier = distrLarge(generator);
			break;
		case 27:
			piece2DifferenceMultiplier = distrLarge(generator);
			break;
		case 28:
			piece3DifferenceMultiplier = distrLarge(generator);
			break;
		case 29:
			piece4DifferenceMultiplier = distrLarge(generator);
			break;
		case 30:
			piece5DifferenceMultiplier = distrLarge(generator);
			break;
		case 31:
			piece6DifferenceMultiplier = distrLarge(generator);
			break;
		default:
			throw std::invalid_argument("Index received for altering gene is invalid value");
	}

}

double AI_Configuration::getGeneValueByIndex(int index) {

	// if (index < 0 || index >= genomeSize) {
	// 	throw std::invalid_argument("Index received for altering gene is invalid value");
	// }

	// return *genome[index];
	switch (index) {
		case 0:
			return crackiness;
			break;
		case 1:
			return coveredHoles;
			break;
		case 2:
			return wellDepth;
			break;
		case 3:
			return aggregateHeight;
			break;
		case 4:
			return linesCleared;
			break;
		case 5:
			return bumpiness;
			break;
		case 6:
			return highestBlock;
			break;
		case 7:
			return scoreValue;
			break;
		case 8:
			return averageColumnLoad;
			break;
		case 9:
			return wellExponent;
			break;
		case 10:
			return rightColumn;
			break;
		case 11:
			return crackinessExponent;
			break;
		case 12:
			return coveredExponent;
			break;
		case 13:
			return movedDown;
			break;
		case 14:
			return boardClearScore;
			break;
		case 15:
			return scoreExponent;
			break;
		case 16:
			return perfectScoreExponent;
			break;
		case 17:
			return iPenalty;
			break;
		case 18:
			return piece0Level;
			break;
		case 19:
			return piece1Level;
			break;
		case 20:
			return piece2Level;
			break;
		case 21:
			return piece3Level;
			break;
		case 22:
			return piece4Level;
			break;
		case 23:
			return piece5Level;
			break;
		case 24:
			return piece6Level;
			break;
		case 25:
			return piece0DifferenceMultiplier;
			break;
		case 26:
			return piece1DifferenceMultiplier;
			break;
		case 27:
			return piece2DifferenceMultiplier;
			break;
		case 28:
			return piece3DifferenceMultiplier;
			break;
		case 29:
			return piece4DifferenceMultiplier;
			break;
		case 30:
			return piece5DifferenceMultiplier;
			break;
		case 31:
			return piece6DifferenceMultiplier;
			break;
		default:
			throw std::invalid_argument("Index received for altering gene is invalid value");
	}
}

// Serialization method to write a vector of AI_Instance objects to a file
// void AI_Configuration::serializeAll(AI_Configuration (&instances)[POP_SIZE], int &generationNumber, std::string &logPrefix) {
// 	std::string fname = logPrefix + ".ailist";
// 	std::ofstream file(fname, std::ios::out | std::ios::binary);
// 	if (file.is_open()) {
// 		file.write(reinterpret_cast<const char*>(&generationNumber), sizeof(int));
// 		file.write(reinterpret_cast<const char*>(instances), sizeof(AI_Configuration) * POP_SIZE);
// 		file.close();
// 	} else {
// 		std::cerr << "Unable to open file for serialization\n";
// 	}
// }

// bool AI_Configuration::deserializeAll(AI_Configuration (&instances)[POP_SIZE], int &generationNumber, std::string &logPrefix) {
// 	std::string fname = logPrefix + ".ailist";
// 	std::ifstream file(fname, std::ios::in | std::ios::binary);
// 	if (file.is_open()) {
// 		file.read(reinterpret_cast<char*>(&generationNumber), sizeof(int));
// 		file.read(reinterpret_cast<char*>(instances), sizeof(AI_Configuration) * POP_SIZE);
// 		file.close();
// 		return true;
// 	} else {
// 		return false;
// 	}
// }

std::string AI_Configuration::displayConfig() {
	std::string ret = "  \"crackiness\":                 " + std::to_string(crackiness) + ",\n" +
	"  \"coveredHoles\":               " + std::to_string(coveredHoles) + ",\n" +
	"  \"wellDepth\":                  " + std::to_string(wellDepth) + ",\n" +
	"  \"aggregateHeight\":            " + std::to_string(aggregateHeight) + ",\n" +
	"  \"linesCleared\":               " + std::to_string(linesCleared) + ",\n" +
	"  \"bumpiness\":                  " + std::to_string(bumpiness) + ",\n" +
	"  \"highestBlock\":               " + std::to_string(highestBlock) + ",\n" +
	"  \"scoreValue\":                 " + std::to_string(scoreValue) + ",\n" +
	"  \"averageColumnLoad\":          " + std::to_string(averageColumnLoad) + ",\n" +
	"  \"wellExponent\":               " + std::to_string(wellExponent) + ",\n" +
	"  \"rightColumn\":                " + std::to_string(rightColumn) + ",\n" +
	"  \"crackinessExponent\":         " + std::to_string(crackinessExponent) + ",\n" +
	"  \"coveredExponent\":            " + std::to_string(coveredExponent) + ",\n" +
	"  \"movedDown\":                  " + std::to_string(movedDown) + ",\n" +
	"  \"boardClearScore\":            " + std::to_string(boardClearScore) + ",\n" +
	"  \"scoreExponent\":              " + std::to_string(scoreExponent) + ",\n" +
	"  \"perfectScoreExponent\":       " + std::to_string(perfectScoreExponent) + ",\n" +
	"  \"iPenalty\":                   " + std::to_string(iPenalty) + ",\n" +
	"  \"piece0Level\":                " + std::to_string(piece0Level) + ",\n" +
	"  \"piece1Level\":                " + std::to_string(piece1Level) + ",\n" +
	"  \"piece2Level\":                " + std::to_string(piece2Level) + ",\n" +
	"  \"piece3Level\":                " + std::to_string(piece3Level) + ",\n" +
	"  \"piece4Level\":                " + std::to_string(piece4Level) + ",\n" +
	"  \"piece5Level\":                " + std::to_string(piece5Level) + ",\n" +
	"  \"piece6Level\":                " + std::to_string(piece6Level) + ",\n" +
	"  \"piece0DifferenceMultiplier\": " + std::to_string(piece0DifferenceMultiplier) + ",\n" +
	"  \"piece1DifferenceMultiplier\": " + std::to_string(piece1DifferenceMultiplier) + ",\n" +
	"  \"piece2DifferenceMultiplier\": " + std::to_string(piece2DifferenceMultiplier) + ",\n" +
	"  \"piece3DifferenceMultiplier\": " + std::to_string(piece3DifferenceMultiplier) + ",\n" +
	"  \"piece4DifferenceMultiplier\": " + std::to_string(piece4DifferenceMultiplier) + ",\n" +
	"  \"piece5DifferenceMultiplier\": " + std::to_string(piece5DifferenceMultiplier) + ",\n" +
	"  \"piece6DifferenceMultiplier\": " + std::to_string(piece6DifferenceMultiplier) + "\n";

	return ret;
}