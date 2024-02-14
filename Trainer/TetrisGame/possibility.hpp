#include "piece.hpp"

class Possibility {

public:
	int pieceX;
	int pieceY;
	int pieceRots;
	
	double score;
	int linesCleared;
	bool hasTetris;

	Possibility(Piece &pieceRef, double score, int linesCleared, bool hasTetris);
	void resetTo(Piece &pieceRef, double score, int linesCleared, bool hasTetris);
};