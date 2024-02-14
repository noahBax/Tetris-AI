#include <vector>
#include "tetrisRules.hpp"
#include <random>

const Matrix tetrominoes[] = {
	{	/* cyan */
		{ 7, 7, 7, 7 },
		{ 0, 0, 0, 0 },
		{ 7, 7, 7, 7 },
		{ 7, 7, 7, 7 },
	},
	{	/* "blue" */
		{ 1, 7, 7 },
		{ 1, 1, 1 },
		{ 7, 7, 7 },
	},
	{	/* "orange" */
		{ 7, 7, 2 },
		{ 2, 2, 2 },
		{ 7, 7, 7 },
	},
	{	/* "yellow" */
		{ 3, 3 }, 
		{ 3, 3 }, 
	},
	{	/* "green" */
		{ 7, 4, 4 },
		{ 4, 4, 7 },
		{ 7, 7, 7 },
	},
	{	/* "purple" */
		{ 7, 5, 7 },
		{ 5, 5, 5 },
		{ 7, 7, 7 },
	},
	{	/* "red" */
		{ 6, 6, 7 },
		{ 7, 6, 6 },
		{ 7, 7, 7 },
	}
};

class Piece {

public:
	Matrix shape;
	int position[2] = {3, 0};
	int rotations = 0;
	int pieceNum;

	Piece(std::mt19937 &generator);

	void rotatePiece(bool isRight);
	bool canPieceFit(const TetrisBoard &board);
	bool movePieceDown(const TetrisBoard &board);
	int castPieceShadow(TetrisBoard &shadowBoard);
	void undoShadow(TetrisBoard &shadowBoard, int timesDropped);
	void print();
};