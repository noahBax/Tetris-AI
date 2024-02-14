#include <random>		/* random_device, mt19937 */
#include "genetics.hpp"

class AI_Configuration {

private:
	void alterGeneByIndex(int index, double newValue);
	void randomizeRandomGeneRandomly(std::mt19937 &generator, double weightSize);
	double getGeneValueByIndex(int index);

public:

	// Being honest, I don't know the best way to organize things here

	double crackiness;
	double coveredHoles;
	double wellDepth;
	double aggregateHeight;
	double linesCleared;
	double bumpiness;
	double highestBlock;
	double scoreValue;
	double averageColumnLoad;
	double wellExponent;
	double rightColumn;
	double crackinessExponent;
	double coveredExponent;
	double movedDown;
	double boardClearScore;
	double scoreExponent;
	double perfectScoreExponent;
	double iPenalty;

	double piece0Level;
	double piece1Level;
	double piece2Level;
	double piece3Level;
	double piece4Level;
	double piece5Level;
	double piece6Level;

	double piece0DifferenceMultiplier;
	double piece1DifferenceMultiplier;
	double piece2DifferenceMultiplier;
	double piece3DifferenceMultiplier;
	double piece4DifferenceMultiplier;
	double piece5DifferenceMultiplier;
	double piece6DifferenceMultiplier;

	static const int genomeSize = 32;

	// double* genome[genomeSize] = {
	// 	&crackiness, &coveredHoles, &wellDepth, &aggregateHeight,&linesCleared,
	// 	&bumpiness, &highestBlock, &scoreValue, &averageColumnLoad, &wellExponent,
	// 	&rightColumn, &crackinessExponent, &coveredExponent, &movedDown, &boardClearScore,
	// 	&scoreExponent, &perfectScoreExponent, &iPenalty, &piece0Level, &piece1Level,
	// 	&piece2Level, &piece3Level, &piece4Level, &piece5Level, &piece0DifferenceMultiplier,
	// 	&piece1DifferenceMultiplier, &piece2DifferenceMultiplier,
	// 	&piece3DifferenceMultiplier, &piece4DifferenceMultiplier,
	// 	&piece5DifferenceMultiplier, &piece6Level, &piece6DifferenceMultiplier
	// };

	// double* exponentGenes[5] = {
	// 	&wellExponent, &crackinessExponent, &coveredExponent,
	// 	&scoreExponent, &perfectScoreExponent
	// };

	// double* coefficientGenes[13] = {
	// 	&crackiness, &coveredHoles, &wellDepth, &aggregateHeight, &linesCleared, &bumpiness,
	// 	&highestBlock, &scoreValue, &averageColumnLoad, &rightColumn, &movedDown,
	// 	&boardClearScore, &iPenalty
	// };

	// double* pieceLevels[7] = {
	// 	&piece0Level, &piece1Level, &piece2Level, &piece3Level,
	// 	&piece4Level, &piece5Level, &piece6Level
	// };

	// double* pieceDiffMultipliers[7] = {
	// 	&piece0DifferenceMultiplier, &piece1DifferenceMultiplier,
	// 	&piece2DifferenceMultiplier, &piece3DifferenceMultiplier,
	// 	&piece4DifferenceMultiplier, &piece5DifferenceMultiplier,
	// 	&piece6DifferenceMultiplier
	// };


	// static void serializeAll(AI_Configuration (&instances)[POP_SIZE], int &generationNumber, std::string &logPrefix);
	// static bool deserializeAll(AI_Configuration (&instances)[POP_SIZE], int &generationNumber, std::string &logPrefix);

	AI_Configuration(double genes[genomeSize]);

	AI_Configuration();
	
	void crossover(AI_Configuration &parent1, AI_Configuration &parent2);
	void initialize(double weightSize, std::mt19937 &generator);
	std::string displayConfig();
	int applyMutation(int mutationRate, double weightSize, std::mt19937 &generator);

};
