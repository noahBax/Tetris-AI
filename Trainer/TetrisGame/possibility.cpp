#include "possibility.hpp"
#include "tetrisRules.hpp"

Possibility::Possibility(Piece &pieceRef, double score, int linesCleared, bool hasTetris) {
	
	this->pieceRots = pieceRef.rotations;
	this->pieceX = pieceRef.position[0];
	this->pieceY = pieceRef.position[1];

	this->score = score;
	this->linesCleared = linesCleared;
	this->hasTetris = hasTetris;
}

void Possibility::resetTo(Piece &pieceRef, double score, int linesCleared, bool hasTetris) {
	
	this->pieceRots = pieceRef.rotations;
	this->pieceX = pieceRef.position[0];
	this->pieceY = pieceRef.position[1];

	this->score = score;
	this->linesCleared = linesCleared;
	this->hasTetris = hasTetris;
}