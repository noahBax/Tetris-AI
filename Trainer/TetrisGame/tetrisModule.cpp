#include "tetrisModule.hpp"
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include "../TetrisGame/possibility.hpp"


#include <chrono>
#include <thread>

void printTetrisBoard(TetrisBoard BOARD) {
	for (int j = 0; j < BOARD_HEIGHT; j++) {
		for (int i = 0; i < BOARD_WIDTH; i++) {
			std::cout << BOARD[j][i] << " ";
		}
		std::cout << "\n";

	}
	std::cout << "\n";

}

const int scoresForLines[] 		= { 0, 100, 300, 500, 800 };
const int perfectClearScores[] 	= { 0, 800, 1200, 1800, 2000, 3200 };

int getRidOfClearedRows(TetrisBoard &board) {

	int linesCleared = 0;

	int* newBoard[BOARD_HEIGHT] = {};
	int lowestClearedRow = -1;

	for (int j = BOARD_HEIGHT - 1; j >= 0; j--) {
		bool lineFull = true;

		for (int i = 0; i < BOARD_WIDTH && lineFull; i++) {
			if (board[j][i] == 7) lineFull = false;
		}

		if (lineFull) {
			if (linesCleared == 0) lowestClearedRow = j;
			linesCleared++;
		} else {
			newBoard[j+linesCleared] = board[j];
		}
	}

	// Now construct the board again with the rows removed
	// We do it this way because we are referring to the original board the
	// whole time and we don't want to alter values before we use them

	if (linesCleared > 0) {
		for (int j = lowestClearedRow; j >= linesCleared; j--) {
			for (int i = 0; i < BOARD_WIDTH; i++) {
				board[j][i] = newBoard[j][i];
			}
		}
		for (int j = 0; j < linesCleared; j++) {
			for (int i = 0; i < BOARD_WIDTH; i++) {
				board[j][i] = 7;
			}
		}
	}

	return linesCleared;
}

// This function takes a board, finds rows that are full, and returns pointers
// to the rows that would be left if they were cleared away in order
// It's essentially getRidOfClearedRows but it doesn't alter the board at all
int getRowsLeftAfterClearing(TetrisBoard &board, int* (&rowList)[BOARD_HEIGHT]) {
	int linesCleared = 0;

	for (int j = BOARD_HEIGHT - 1; j >= 0; j--) {
		bool lineFull = true;

		for (int i = 0; i < BOARD_WIDTH && lineFull; i++) {
			if (board[j][i] == 7) lineFull = false;
		}

		if (lineFull) {
			rowList[linesCleared] = board[j];
			linesCleared++;
		} else {
			rowList[j+linesCleared] = board[j];
		}
	}

	return linesCleared;
}

double findBoardScore(TetrisBoard &board, AI_InstanceHandler &weightConfigHandler, int pieceCenter, int &linesCleared, bool &tetrisOccurs, int pieceNumber) {

	double crackiness = 0;		// Spaces that are empty and in the column directly adjacent to the current one
	double coveredHoles = 0;	// Holes which we can't ever fill. They are inaccessible
	double wellDepth = 0;		// Holes which we can only partially fill. 3 deep or deeper
	double boardClearScore = 0;
	int rightColumnCount = 0;

	// Count the number of blocks in the rightmost column

	int depthList[BOARD_WIDTH] = {};	// The row of the minimum depth block in a column
	std::fill_n(depthList, BOARD_WIDTH, BOARD_HEIGHT);
	int columnCounts[BOARD_WIDTH] = {};	// The counts of blocks in each column

	// To start, we need to look at what the result of dropping this piece would be
	// This means removing rows that would be cleared
	// The board we are given already has the piece dropped, we just remove rows

	int* newBoard[BOARD_HEIGHT] = {};
	linesCleared = getRowsLeftAfterClearing(board, newBoard);

	// Levels to apply different weight configs at
	// 0-8 Bottom
	// 9-15 Middle
	// 16-20 Top
	// We need to find that level
	// Do an initial scan down the board to find it
	AI_Configuration weightConfig;
	int lowestDepth = -1;
	for (int j = linesCleared; j < BOARD_HEIGHT && lowestDepth == -1; j++) {

		for (int i = 0; i < BOARD_WIDTH && lowestDepth == -1; i++) {
			if (newBoard[j][i] != 7) lowestDepth = j;
		}

	}
	// If the board is cleared, lowestDepth will still be -1
	lowestDepth = lowestDepth == -1 ? BOARD_HEIGHT : lowestDepth;
	const int highestBlock = BOARD_HEIGHT - lowestDepth;
	if (highestBlock + linesCleared < 9) {
		weightConfig = weightConfigHandler.bottomConfig;
	} else if (highestBlock + linesCleared < 16) {
		weightConfig = weightConfigHandler.middleConfig;
	} else {
		weightConfig = weightConfigHandler.topConfig;
	}

	// Yes, yes the following code might look a bit weird
	// I'm trying to avoid looping through columns in the board by just going through it line by line
	// It may not offer the speed benefits I think it does (given the small scale), but its fun to think like this

	// Score remaining areas
	// We can set j this way because we found where the highest block is
	// As of NOW, blocks above the highest block cannot affect any statistics
	for (int j = lowestDepth; j < BOARD_HEIGHT; j++) {

		// Do an initial scan to update non-holes
		for (int i = 0; i < BOARD_WIDTH; i++) {
			// Update heights if need be
			if (newBoard[j][i] != 7) {
				if (depthList[i] == BOARD_HEIGHT) depthList[i] = j;
				
				// Increase column counts
				columnCounts[i]++;
				
			}

		}

		for (int i = 0; i < BOARD_WIDTH; i++) {
			if (newBoard[j][i] == 7) { // Holes

				auto crackCount = 0;

				crackCount += (i != 0 && j >= depthList[i-1]);	// If column is higher left of us
				crackCount += (i != 9 && j >= depthList[i+1]);	// If column is higher right of us
				
				crackiness += crackCount * pow((BOARD_HEIGHT - j + 1), weightConfig.crackinessExponent);
				
				// Adjust unreachable holes
				if (j > depthList[i]) {
					coveredHoles += pow(BOARD_HEIGHT - j + 1, weightConfig.coveredExponent);
				}

			}
		}

		// Increment the right column counter if necessary
		if (newBoard[j][BOARD_WIDTH - 1] != 7) rightColumnCount++;

	}

	// Okay, after finding info, lets get some statistics


	// Wells are classified as a positive height difference from one column to
	// its sides (on both sides) of at least 3. The left and rightmost columns
	// both only require a height difference of 3 or more on one side
	// The height of the well itself is just the smaller of those 2 differences
	
	// Left
	if (depthList[0] - depthList[1] >= 3) wellDepth += pow(depthList[0] - depthList[1], weightConfig.wellExponent);
	
	// Middle
	for (int i = 1; i < BOARD_WIDTH - 1; i++) {
		const int leftDif = depthList[i] - depthList[i - 1];
		const int rightDif = depthList[i] - depthList[i + 1];

		if (leftDif >= 3 && rightDif >= 3) {
			const int min = (leftDif < rightDif) ? leftDif : rightDif;
			wellDepth += pow(min, weightConfig.wellExponent);

			// There cannot be another well adjacent to us. Then we would not be a well
			i++;
		}
	}

	// Right
	if (depthList[9] - depthList[8] >= 3) wellDepth += pow(depthList[9] - depthList[8], weightConfig.wellExponent);


	// Find height aggregateHeight
	const double heightAgg = (double) (BOARD_HEIGHT * BOARD_WIDTH - std::accumulate(depthList, depthList + BOARD_WIDTH, 0)) / BOARD_WIDTH;


	// Now bumpiness
	int bumpiness = 0;
	for (short i = 1; i < BOARD_WIDTH; i++) {
		bumpiness += abs(depthList[i] - depthList[i-1]);
	}

	// Average the amount in columns
	const double averageColumnLoad = std::accumulate(columnCounts, columnCounts + BOARD_WIDTH, 0) / BOARD_WIDTH;

	// Factor in the number of points the move would have given us
	const double scoreValue = pow(scoresForLines[linesCleared], weightConfig.scoreExponent);
	if (highestBlock == 0) boardClearScore = pow(perfectClearScores[linesCleared], weightConfig.perfectScoreExponent);

	const double iValue = (pieceNumber == 0) ? (20 - pieceCenter) : 0.0;

	tetrisOccurs = linesCleared == 4;

	// Account for different pieces
	// const double centerMovedDifference = pieceCenter - *weightConfig.pieceLevels[pieceNumber];
	double centerMovedDifference;
	switch(pieceNumber) {
		case 0:
			centerMovedDifference = pieceCenter - weightConfig.piece0Level;
			break;
		case 1:
			centerMovedDifference = pieceCenter - weightConfig.piece1Level;
			break;
		case 2:
			centerMovedDifference = pieceCenter - weightConfig.piece2Level;
			break;
		case 3:
			centerMovedDifference = pieceCenter - weightConfig.piece3Level;
			break;
		case 4:
			centerMovedDifference = pieceCenter - weightConfig.piece4Level;
			break;
		case 5:
			centerMovedDifference = pieceCenter - weightConfig.piece5Level;
			break;
		case 6:
			centerMovedDifference = pieceCenter - weightConfig.piece6Level;
			break;
	}

	double centerMultiplier;
	switch(pieceNumber) {
		case 0:
			centerMovedDifference = weightConfig.piece0DifferenceMultiplier;
			break;
		case 1:
			centerMovedDifference = weightConfig.piece1DifferenceMultiplier;
			break;
		case 2:
			centerMovedDifference = weightConfig.piece2DifferenceMultiplier;
			break;
		case 3:
			centerMovedDifference = weightConfig.piece3DifferenceMultiplier;
			break;
		case 4:
			centerMovedDifference = weightConfig.piece4DifferenceMultiplier;
			break;
		case 5:
			centerMovedDifference = weightConfig.piece5DifferenceMultiplier;
			break;
		case 6:
			centerMovedDifference = weightConfig.piece6DifferenceMultiplier;
			break;
	}

	// std::cout << "crackiness: " << crackiness << std::endl;
	// std::cout << "coveredHoles: " << coveredHoles << std::endl;
	// std::cout << "wellDepth: " << wellDepth << std::endl;
	// std::cout << "heightAgg: " << heightAgg << std::endl;
	// std::cout << "linesCleared: " << linesCleared << std::endl;
	// std::cout << "bumpiness: " << bumpiness << std::endl;
	// std::cout << "highestBlock: " << highestBlock << std::endl;
	// std::cout << "averageColumnLoad: " << averageColumnLoad << std::endl;
	// std::cout << "scoreValue: " << scoreValue << std::endl;
	// std::cout << "rightColumnCount: " << rightColumnCount << std::endl;
	// std::cout << "timeMovedDown: " << timeMovedDown << std::endl;
	// std::cout << "boardClearScore: " << boardClearScore << std::endl;

	const double finalScore = weightConfig.crackiness * crackiness +
				weightConfig.coveredHoles * coveredHoles +
				weightConfig.wellDepth * wellDepth +
				weightConfig.aggregateHeight * heightAgg +
				weightConfig.linesCleared * linesCleared +
				weightConfig.bumpiness * bumpiness +
				weightConfig.highestBlock * highestBlock +
				weightConfig.averageColumnLoad * averageColumnLoad +
				weightConfig.scoreValue * scoreValue +
				weightConfig.rightColumn * rightColumnCount +
				weightConfig.movedDown * pieceCenter +
				weightConfig.boardClearScore * boardClearScore +
				weightConfig.iPenalty * iValue +
				centerMovedDifference * centerMovedDifference;

	return finalScore;
}

bool tryAllOrientations(Piece piece, TetrisBoard &board, AI_InstanceHandler &weightConfigHandler, Possibility &bestPossibility, bool hasBestScoreAlready=false) {

	// Loop through each possible turn of the piece
	bool didFindGoodResults = false;
	double bestSeenScore = bestPossibility.score;
	for (int t = 0; t < 4; t++) {
		// Break out if pieces are symmetrical
		if (t == 1 && piece.pieceNum == 3) {
			break;
		} else if (t == 2 && piece.pieceNum == 0) {
			// Don't have to do this in js because there it is a structured
			// clone. Here it is not.
			piece.rotatePiece(true);
			piece.rotatePiece(true);
			break;
		}

		piece.position[0] = -2;	// Start it to the left because there are gaps in the matrix
		piece.position[1] = 0;

		/**
		 * I'm trying to reuse the board array every time I find the board score
		 * and only storing orientation info in each possibility. The creation
		 * of 10s of 1000s of almost identical matrices every second was the
		 * slowest part of my program although I haven't benchmarked it so take
		 * that with a grain of salt. I'm just trying to avoid writing to memory
		 * as much as possible because this thing fits entirely in cache as is
		 * and going to ram is a bottleneck.
		 *
		 * This is NOT implemented in the javascript version because it doesn't
		 * need to be this complex. I am not running it 24/7 on a machine to
		 * train for a week.
		*/
		
		// Keep trying until the piece won't fit AND we're at the edge of the board
		while (piece.canPieceFit(board) || piece.position[0] + piece.shape.size() < BOARD_WIDTH) {
			if (piece.canPieceFit(board)) {

				// Cast the piece shadow onto the board
				bool hasTetris = false;
				const int timesMovedDown = piece.castPieceShadow(board);

				// I can try to account for the different piece orientations
				double pieceCenter;
				if (piece.pieceNum == 0) { // I piece
					pieceCenter = timesMovedDown + (t % 2 == 0) ? 0.5 : 2;
				} else if (piece.pieceNum == 3) { // Square piece
					pieceCenter = timesMovedDown + 1;
				} else { // Piece has dimension of 3x3
					pieceCenter = timesMovedDown + (t % 2 == 0) ? 1 : 1.5;
				}
				// There is space above the piece on the third rotation
				if (t == 2) pieceCenter--;

				int linesCleared = 0;
				const double score = findBoardScore(board, weightConfigHandler, pieceCenter, linesCleared, hasTetris, piece.pieceNum);
				piece.undoShadow(board, timesMovedDown);

				if (!hasBestScoreAlready || score < bestSeenScore) {
					hasBestScoreAlready = true;
					if (!didFindGoodResults) didFindGoodResults = true;
					bestSeenScore = score;

					bestPossibility.resetTo(
						piece,
						score,
						linesCleared,
						hasTetris
					);
				}

			}
			piece.position[0]++;

		}
		
		piece.rotatePiece(true);
	}

	return didFindGoodResults;
}

void pieceCleanup(int linesCleared, int &score, bool &prevRowTetrisClearOut, TetrisBoard &board) {

	score += scoresForLines[linesCleared];

	bool clearBoard = true;
	for (int j = BOARD_HEIGHT - 1; j > 2; j--) {
		for (int i = 0; i < BOARD_WIDTH; i++) {
			if (board[j][i] != 7) {
				clearBoard = false;
				break;
			}
		}
		if (!clearBoard) break;
	}

	if (clearBoard) {
		if (linesCleared == 4) {
			if (prevRowTetrisClearOut) {
				// 2 tetris's in a row!
				score += perfectClearScores[5];
			} else {
				score += perfectClearScores[4];
			}
			prevRowTetrisClearOut = true;
		} else {
			prevRowTetrisClearOut = false;
			score += perfectClearScores[linesCleared];
		}
	} else {
		prevRowTetrisClearOut = false;
	}

}

void switchHeldPiece(Piece &currentPiece, Piece &heldPiece) {
	Piece temp = currentPiece;
	currentPiece = heldPiece;
	heldPiece = temp;
}

void runTetrisGame(AI_InstanceHandler weightConfigHandler, int maxPieces, int (&results)[4], std::mt19937 &generator) {

	TetrisBoard boardState = {
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
		{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 }
	};

	int lines = 0;
	int score = 0;
	int piecesPlaced = 0;
	int tetrisCount = 0;
	bool prevRowTetrisClearOut = false;
	
	Piece currentPiece = Piece(generator);
	Piece nextPiece    = Piece(generator);
	Piece heldPiece    = Piece(generator);

	bool running = true;

	while (running && piecesPlaced < maxPieces) {

		piecesPlaced++;


		// Find the best orientation to put a piece in
		Possibility bestPossibility = Possibility(currentPiece, 0, 0, false);

		// Find the best orientation of the current piece
		bool currentPieceHasValidOrientation = tryAllOrientations(currentPiece, boardState, weightConfigHandler, bestPossibility);

		// Of the held piece
		// Don't do extra work if we don't have to
		bool heldPieceHasValidOrientation = false;
		if (currentPiece.pieceNum != heldPiece.pieceNum) {
			heldPieceHasValidOrientation = tryAllOrientations(heldPiece, boardState, weightConfigHandler, bestPossibility, true);
		}

		
		// If neither try found valid moves, exit the game
		if (!currentPieceHasValidOrientation && !heldPieceHasValidOrientation) {
			running = false;

			continue;
		}

		// If at least one of them found a valid move, bestPossibility will have the best possibility. Most definitely
		int linesCleared = bestPossibility.linesCleared;

		if (heldPieceHasValidOrientation)
			switchHeldPiece(currentPiece, heldPiece);

		if (bestPossibility.hasTetris)
			tetrisCount++;

		// We need to reorient the piece so that it fits the best possibility
		for (int i = 0; i < bestPossibility.pieceRots; i++) {
			currentPiece.rotatePiece(true);
		}
		currentPiece.position[0] = bestPossibility.pieceX;
		currentPiece.position[1] = bestPossibility.pieceY;

		currentPiece.castPieceShadow(boardState);

		// Turn it the rest of the way
		for (int i = 4 - bestPossibility.pieceRots; i < 4; i++) {
			currentPiece.rotatePiece(true);
		}

		lines += linesCleared;

		getRidOfClearedRows(boardState);
		pieceCleanup(linesCleared, score, prevRowTetrisClearOut, boardState);

		// Get a new current piece
		currentPiece = nextPiece;
		nextPiece = Piece(generator);

		// printTetrisBoard(boardState);

	}

	results[0] = score;
	results[1] = lines;
	results[2] = piecesPlaced;
	results[3] = tetrisCount;

	return;
}

