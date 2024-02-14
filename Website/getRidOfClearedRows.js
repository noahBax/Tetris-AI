// This is just a direct parallel of my C++ code but in JavaScript
// Purely for testing purposes at this point

// If this is still in use after I'm done testing it, make sure to replace it
// with the splicing code

function getRidOfClearedRows(board) {

	let linesCleared = 0;

	let newBoard = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
	let lowestClearedRow = -1;

	for (let j = boardDimensions.height - 1; j >= 0; j--) {

		let lineFull = board[j].every( i => i != 7);

		if (lineFull) {
			if (linesCleared == 0) lowestClearedRow = j;
			linesCleared++;
		} else {
			newBoard[j + linesCleared] = board[j];
		}
	}

	// Now construct the board again with the rows removed
	// We do it this way because we are referring to the original board the
	// whole time and we don't want to alter values before we use them

	// None of the following will run if no rows were cleared

	for (let j = lowestClearedRow; j >= linesCleared; j--) {
		for (let i = 0; i < boardDimensions.width; i++) {
			board[j][i] = newBoard[j][i];
		}
	}

	for (let j = 0; j < linesCleared; j++) {
		for (let i = 0; i < boardDimensions.width; i++) {
			board[j][i] = 7;
		}
	}

	return linesCleared;

}