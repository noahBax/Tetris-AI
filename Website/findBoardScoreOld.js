function findBoardScoreOG(board, weightConfig, lastPiece) {
	
	// Penalize holes
	// Sweep from left to right looking to see if the column to the left of us is below. Not cache efficient but meh

	let crackiness = 0;			// Spaces that are empty and in the column directly adjacent to the current one  
	let coveredHoles = 0;	// Holes which we can't ever fill. They are inaccessible
	let wellDepth = 0;	// Holes which we can only partially fill. 3 deep or deeper
	let rightColumnCount = 0;
	let boardClearScore = 0;

	let prevRowTetrisClear = false;
	
	const heightList = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
	const columnCounts = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0];

	// Go through and count the number of blocks in the rightmost column
	for (let i = 0; i < boardDimensions.height; i++) {
		if (board[i][boardDimensions.width - 1] != 7) rightColumnCount++;
	}
	
	for (let j = 0; j < boardDimensions.width; j++) {
		let lineChecked = false;

		for (let i = 0; i < boardDimensions.height; i++) {
			
			// Skip to the next spot if empty
			if (board[i][j] == 7) continue

			
			columnCounts[j]++;

			if (lineChecked) continue
			lineChecked = true;
			
			// Record height
			heightList[j] = boardDimensions.height - i;

			for (let k = i; k < boardDimensions.height; k++) {
				// Go down through the column left of us
				if (board[k][j-1] == 7) {
					crackiness += (boardDimensions.height - k + 1)**(weightConfig.crackinessExponent);
				}
				// Go down right
				if (board[k][j+1] == 7) {
					crackiness += (boardDimensions.height - k + 1)**(weightConfig.crackinessExponent);
				}
			}
			// Look down beneath us
			maxHeight = 0;
			for (let k = i; k < boardDimensions.height; k++) {
				if (board[k][j] == 7) {
					coveredHoles += (boardDimensions.height - k + 1)**(weightConfig.coveredExponent);	// We want to penalize new holes that are farther up
					maxHeight++;
				} else {
					if (maxHeight >= 3) {
						// Check for columns of 3 or more in our row, penalize based on height
						// scoreBoard[k][j] += maxHeight*2;
						wellDepth += maxHeight**(weightConfig.wellExponent);
					}
					maxHeight = 0;
				}
			}
			// break;
		}
	}


	// Compute height aggregateHeight
	let agg = heightList.reduce( (a, b) => a + b) / 10;

	// Find number of completed lines
	let linesCleared = 0;
	for (let j = 0; j < boardDimensions.height; j++) {
		if (board[j].every(color => color != 7)) {
			linesCleared++;
		}
	}

	// Now bumpiness
	let bumpiness = 0;
	for (let i = 1; i < heightList.length; i++) {
		bumpiness += Math.abs(heightList[i] - heightList[i-1]);
	}

	let highestBlock = heightList.reduce( (a, b) => a > b ? a : b);
	let averageColumnLoad = columnCounts.reduce( (a,b) => a + b) / 10;
	let scoreValue = scoresForLines[linesCleared]**(weightConfig.scoreExponent);

	if (highestBlock - linesCleared == 0) {
		if (linesCleared == 4) {
			if (prevRowTetrisClear) {
				// We got 2 tetris's in a row
				boardClearScore = perfectClearScore[5]**(weightConfig.perfectScoreExponent);
			} else {
				boardClearScore = perfectClearScore[4]**(weightConfig.perfectScoreExponent);
			}
			prevRowTetrisClear = true;
		} else {
			prevRowTetrisClear = false;
			boardClearScore = perfectClearScore[linesCleared]**(weightConfig.perfectScoreExponent);
		}
	} else {
		prevRowTetrisClear = false;
	}

	
	let finalScore = weightConfig.crackiness * crackiness +
					weightConfig.coveredHoles * coveredHoles +
					weightConfig.wellDepth * wellDepth +
					weightConfig.aggregateHeight * agg +
					weightConfig.linesCleared * linesCleared +
					weightConfig.bumpiness * bumpiness +
					weightConfig.highestBlock * highestBlock +
					weightConfig.averageColumnLoad * averageColumnLoad +
					weightConfig.scoreValue * scoreValue +
					weightConfig.rightColumn * rightColumnCount +
					weightConfig.movedDown * lastPiece.movedDown +
					weightConfig.boardClearScore * boardClearScore;

	// console.log("crackiness: ", crackiness);
	// console.log("coveredHoles: ", coveredHoles);
	// console.log("wellDepth: ", wellDepth);
	// console.log("agg: ", agg);
	// console.log("linesCleared: ", linesCleared);
	// console.log("bumpiness: ", bumpiness);
	// console.log("highestBlock: ", highestBlock);
	// console.log("averageColumnLoad: ", averageColumnLoad);
	// console.log("scoreValue: ", scoreValue);
	// console.log("rightColumnCount: ", rightColumnCount);
	// console.log("movedDown: ", lastPiece.movedDown);
	// console.log("boardClearScore: ", boardClearScore);

	return finalScore;
}