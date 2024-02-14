#include <random>
#include "ai_configuration.hpp"

class AI_InstanceHandler {

private:

public:
	AI_Configuration bottomConfig;
	AI_Configuration middleConfig;
	AI_Configuration topConfig;

	static void serializeAll(AI_InstanceHandler (&instances)[POP_SIZE], int &generationNumber, std::string &logPrefix);
	static bool deserializeAll(AI_InstanceHandler (&instances)[POP_SIZE], int &generationNumber, std::string &logPrefix);

	AI_InstanceHandler();

	void crossover(AI_InstanceHandler &parent1, AI_InstanceHandler &parent2);
	void initialize(double weightSize, std::mt19937 &generator);
	std::string displayHandler();

	// I think what I'm going to do is just mutate each configuration individually
	// Random gene hopping is done after though
	int applyMutation(int mutationRate, double weightSize, std::mt19937 &generator);

	double fitness = 0;
	double linesResult = 0;
	double scoreResult = 0;
};