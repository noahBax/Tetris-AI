function autoIncrementInstant() {

	if (gamePaused) return;

	iterCount++;
	// if (iterCount == 100) return;

	let currBest = Infinity;
	let currInd = 0;
	let heldBest = Infinity;
	let heldInd = 0;

	// Try to find best orientation
	const allOrientationsCurrent = tryAllOrientations(currentPiece, boardState, weightMultiConfig);
	if (allOrientationsCurrent.length > 0) {
		for (let i = 0; i < allOrientationsCurrent.length; i++) {
			if (allOrientationsCurrent[i].score < currBest) {
				currInd = i;
				currBest = allOrientationsCurrent[i].score;
			}
		}
	} else {
		console.log("No current piece orientations available");
	}

	// Also try to find best orientation of held piece
	const allOrientationsHeld = tryAllOrientations(heldPiece, boardState, weightMultiConfig);
	if (allOrientationsHeld.length > 0) {
		for (let i = 0; i < allOrientationsHeld.length; i++) {
			if (allOrientationsHeld[i].score < heldBest) {
				heldInd = i;
				heldBest = allOrientationsHeld[i].score;
			}
		}
	} else {
		console.log("No held piece orientations available");
	}

	previousCheck = [allOrientationsCurrent, allOrientationsHeld];

	// Check to see if the game is over
	if (allOrientationsCurrent.length == 0 && allOrientationsHeld.length == 0) {
		console.log("Game complete");
		gamePaused = true;
		return;
	}

	
	let linesCleared = getRidOfClearedRows(boardState);

	// Find the best orientation
	if (heldBest < currBest) {
		for (let i = 0; i < boardState.length; i++) {
			boardState[i] = allOrientationsHeld[heldInd].board[i];
		}
		linesCleared = allOrientationsHeld[heldInd].linesCleared;
		switchHeldPiece();
		hasPieceLanded = true;
	} else {
		for (let i = 0; i < boardState.length; i++) {
			boardState[i] = allOrientationsCurrent[currInd].board[i];
		}
		linesCleared = allOrientationsCurrent[currInd].linesCleared;
		hasPieceLanded = true;
	}

	lines += linesCleared;

	// Test to see if there is a clear board
	let noneLeft = linesCleared > 0 && !boardState.some( row => row.some( block => block != 7));
	
	if (noneLeft) {
		if (linesCleared == 4) {
			if (prevRowTetrisClearOut) {
				// We got 2 tetris's in a row
				score += perfectClearScore[5];
			} else {
				score += perfectClearScore[4];
			}
			prevRowTetrisClearOut = true;
		} else {
			prevRowTetrisClearOut = false;
			score += perfectClearScore[linesCleared];
		}
	} else {
		prevRowTetrisClearOut = false;
	}


	score += scoresForLines[linesCleared];

	// Refresh score line
	scoreEle.innerHTML = score;
	linesEle.innerHTML = lines;
	ratioEle.innerHTML = lines != 0 ? Math.round(score / lines * 100) / 100 : 0;
	// fitnessEle.innerHTML = score * lines;

	// Pick a new piece
	currentPiece = {...nextPiece};
	nextPiece = generateAPiece();

	drawNextPiece();

	setTimeout(autoIncrementInstant, progressSpeed);
}