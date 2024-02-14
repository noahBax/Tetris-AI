#include <vector>
#include "piece.hpp"
#include "tetrisRules.hpp"
#include <iostream>

Piece::Piece(std::mt19937 &generator) {
	pieceNum = generator() % NUMBER_PIECES;
	shape = tetrominoes[pieceNum];
}

void Piece::rotatePiece(bool isRight) {

	if (pieceNum == 3) return; // Why not?
	
	const int size = shape.size();
	const Matrix backup = shape;


	if (isRight) {
		rotations++;
		rotations %= 4;
		
		for (int j = 0; j < size; j++) {
			for (int i = 0; i < size; i++) {
				shape[i][size - 1 - j] = backup[j][i];
			}
		}
	}  else {
		rotations += 3;	// Same thing as subtracting 1
		rotations %= 4;

		for (int j = 0; j < size; j++) {
			for (int i = 0; i < size; i++) {
				shape[size - 1 - i][j] = backup[j][i];
			}
		}
	}
}

bool Piece::canPieceFit(const TetrisBoard &board) {

	const int size = shape.size();

	// Loop through rows in board and check to see if either or both are 7
	for (int j = 0; j < size; j++) {
		const bool belowBoard = position[1] + j >= BOARD_HEIGHT;
		for (int i = 0; i < size; i++) {
			const bool shapeIsBlock = shape[j][i] != 7;
			const bool boardIsBlock = (board[position[1] + j][position[0] + i] != 7);
			const bool leftOfBoard = position[0] + i < 0;
			const bool rightOfBoard = position[0] + i >= BOARD_WIDTH;
			if (shapeIsBlock && (leftOfBoard || rightOfBoard || boardIsBlock || belowBoard)) {
				return false;
			}
		}
	}

	return true;
}

bool Piece::movePieceDown(const TetrisBoard &board) {
	if (!canPieceFit(board)) return false;

	position[1]++;

	if (canPieceFit(board)) {
		return true;
	} else {
		position[1]--;
		return false;
	}
}

int Piece::castPieceShadow(TetrisBoard &shadowBoard) {

	// Keep track of the original position of us
	const int originalY = position[1];


	// Most of the time the board will be mostly empty right?
	// So let's try moving it up from the bottom instead of down from the top

	// We set it's position to BOARD_HEIGHT, then move it up until it hits a place it can fit.
	// Additionally, I think it would be beneficial to try moving the piece up 4 blocks at a time
	// instead of just the 1. If we get to a place where the piece fits, try moving it down as normal
	// Bubble up instead of falling down, y'know

	position[1] = BOARD_HEIGHT;
	int movedDown = BOARD_HEIGHT;

	// Bubble up from the bottom fast first

	while (!canPieceFit(shadowBoard)) {
		position[1] -= 4;
		movedDown -= 4;
	}

	// Fall to where we should be slowly

	while (movePieceDown(shadowBoard)) {
		movedDown++;
	}

	// Draw the shadow onto the shadow board
	for (int j = 0; j < shape.size() && position[1] + j < BOARD_HEIGHT; j++) {

		for (int i = 0; i < shape.size() && position[0] + i < BOARD_WIDTH; i++) {

			if (shape[j][i] != 7) {
				shadowBoard[position[1] + j][position[0] + i] = shape[j][i];
			}
		}
	}

	// Restore our original position
	position[1] = originalY;
	
	return movedDown;
}

void Piece::undoShadow(TetrisBoard &shadowBoard, int timesDropped) {

	position[1] += timesDropped;

	// Un-draw the shadow onto the shadow board
	for (int j = 0; j < shape.size() && position[1] + j < BOARD_HEIGHT; j++) {

		for (int i = 0; i < shape.size() && position[0] + i < BOARD_WIDTH; i++) {

			if (shape[j][i] != 7) {
				shadowBoard[position[1] + j][position[0] + i] = 7;
			}
		}
	}

	position[1] -= timesDropped;

}

void Piece::print() {
	for (int j = 0; j < shape.size(); j++) {
		for (int i = 0; i < shape.size(); i++) {
			if (shape[j][i] == 7) std::cout << ".  ";
			else std::cout << shape[j][i] << ", ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}