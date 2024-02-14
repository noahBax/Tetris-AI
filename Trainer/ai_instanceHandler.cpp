#include "ai_instanceHandler.hpp"
#include <iostream>
#include <fstream>

void AI_InstanceHandler::initialize(double weightSize, std::mt19937 &generator) {

	this->bottomConfig.initialize(weightSize, generator);
	this->middleConfig.initialize(weightSize, generator);
	this->topConfig.initialize(weightSize, generator);
	
}

void AI_InstanceHandler::crossover(AI_InstanceHandler &parent1, AI_InstanceHandler &parent2) {

	this->bottomConfig.crossover(parent1.bottomConfig, parent2.bottomConfig);
	this->middleConfig.crossover(parent1.middleConfig, parent2.middleConfig);
	this->topConfig.crossover(parent1.topConfig, parent2.topConfig);
	
}

int AI_InstanceHandler::applyMutation(int mutationRate, double weightSize, std::mt19937 &generator) {

	int totalMutations = 0;

	totalMutations += this->bottomConfig.applyMutation(mutationRate, weightSize, generator);
	totalMutations += this->middleConfig.applyMutation(mutationRate, weightSize, generator);
	totalMutations += this->topConfig.applyMutation(mutationRate, weightSize, generator);

	// The next part is switching of entire configurations
	// I don't want it to happen as often as a normal mutation

	if (rand() % 100 > mutationRate / 2) return totalMutations;

	// Select which one to NOT mutate
	int notThis = rand() % 6;
	AI_Configuration temp;
	switch (notThis) {
		case 0:
			this->middleConfig = this->topConfig;
			break;
		case 1:
			this->topConfig = this->middleConfig;
			break;
		case 2:
			this->bottomConfig = topConfig;
			break;
		case 3:
			this->topConfig = bottomConfig;
			break;
		case 4:
			this->bottomConfig = middleConfig;
			break;
		case 5:
			this->middleConfig = bottomConfig;
			break;
	}

	return totalMutations + 2;
}

AI_InstanceHandler::AI_InstanceHandler() {}

void AI_InstanceHandler::serializeAll(AI_InstanceHandler (&instances)[POP_SIZE], int &generationNumber, std::string &logPrefix) {
	std::string fname = logPrefix + ".aihandlelist";
	std::ofstream file(fname, std::ios::out | std::ios::binary);
	if (file.is_open()) {
		file.write(reinterpret_cast<const char*>(&generationNumber), sizeof(int));
		file.write(reinterpret_cast<const char*>(instances), sizeof(AI_InstanceHandler) * POP_SIZE);
		file.close();
	} else {
		std::cerr << "Unable to open file for serialization\n";
	}
}

bool AI_InstanceHandler::deserializeAll(AI_InstanceHandler (&instances)[POP_SIZE], int &generationNumber, std::string &logPrefix) {
	std::string fname = logPrefix + ".aihandlelist";
	std::ifstream file(fname, std::ios::in | std::ios::binary);
	if (file.is_open()) {
		file.read(reinterpret_cast<char*>(&generationNumber), sizeof(int));
		file.read(reinterpret_cast<char*>(instances), sizeof(AI_InstanceHandler) * POP_SIZE);
		file.close();
		return true;
	} else {
		return false;
	}
}

std::string AI_InstanceHandler::displayHandler() {
	std::string ret = "{\n\"bottom\": {\n" +
	this->bottomConfig.displayConfig() +
	"},\n\"middle\": {\n" +
	this->middleConfig.displayConfig() +
	"},\n\"top\": {\n" +
	this->topConfig.displayConfig() +
	"},\n" +
	"\"levelControlsSectionsBeneath\": true,\n" +
	"\"catersToIndividualPieces\": true\n" +
	"}\n";

	return ret;
}