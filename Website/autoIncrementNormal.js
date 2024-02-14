function autoIncrementNormal() {

	if (gamePaused) return;

    if (movePieceDown(currentPiece, boardState)) {
        setTimeout(autoIncrementNormal, progressSpeed);
        return;
    }

    // Write existing piece to board
	if (!instantPlace) {
		for (let j = 0; j < currentPiece.shape.length; j++) {
			for (let i = 0; i < currentPiece.shape[j].length; i++) {
	
				if (currentPiece.shape[j][i] == 7) continue;

				boardState[currentPiece.position.y + j][currentPiece.position.x + i] = currentPiece.shape[j][i];

			}
		}
	}

	// let linesCleared = 0;
	// for (let j = boardDimensions.height-1; j > 0; j--) {
	// 	if (boardState[j].every(block => block != 7)) {
	// 		// Full line
	// 		boardState.splice(j, 1);
	// 		boardState.unshift( [7, 7, 7, 7, 7, 7, 7, 7, 7, 7] );
			
	// 		j++;
	// 		lines++;
	// 		linesCleared++;
	// 	}
	// }
	
	// First check to see if any lines were cleared
	let linesCleared = getRidOfClearedRows(boardState);
	lines += linesCleared;
	

	// Test to see if there is a clear board
	let noneLeft = !boardState.some( row => row.some( block => block != 7));
	
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

	iterCount++

    // Find the best way to place the next piece
	let currBest = Infinity;
	let currInd = 0;
	let heldBest = Infinity;
	let heldInd = 0;
    
	// Try to find best orientation of the current piece
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

	// Find the best orientation
	if (heldBest < currBest && allOrientationsHeld.length > 0) {
        switchHeldPiece();
        applyOrientation(currentPiece, allOrientationsHeld[heldInd]);
		// findBoardScore(allOrientationsHeld[heldInd].board, weightMultiConfig, 0, 0, true);
		// findBoardScore(allOrientationsHeld[heldInd].board, weightMultiConfig, 0, 0, true);
		if (usingCharts) {
			const thingy = allOrientationsHeld[heldInd];
			findBoardScore(thingy.board, weightMultiConfig, thingy.movedDown, thingy.pieceNum, false, true);
			updateCharts();
		}
        hasPieceLanded = false;
	} else {
        applyOrientation(currentPiece, allOrientationsCurrent[currInd]);
		// findBoardScore(allOrientationsCurrent[currInd].board, weightMultiConfig, 0, 0, true);
		// findBoardScore(allOrientationsCurrent[currInd].board, weightMultiConfig, 0, 0, true);
		if (usingCharts) {
			const thingy = allOrientationsCurrent[currInd];
			findBoardScore(thingy.board, weightMultiConfig, thingy.movedDown, thingy.pieceNum, false, true);
			updateCharts();
		}
        hasPieceLanded = false;
	}

	setTimeout(autoIncrementNormal, progressSpeed);
}