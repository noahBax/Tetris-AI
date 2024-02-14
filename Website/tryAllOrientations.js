function tryAllOrientations(piece, board, weightConfig) {

	let pieceCopy = structuredClone(piece);
	const boardCopy = structuredClone(board);
	
	const possibilities = [];
	
	for (t = 0; t < 4; t++) {
		// Break out if pieces are symmetrical
		if (t == 1 && piece.pieceNum == 3 || t == 2 && piece.pieceNum == 0) break;

		pieceCopy.position = { x: 0, y: 0}

		// Try to move it left, this is really just for the I piece
		pieceCopy.position.x--;
		// if (!canPieceFit(pieceCopy, boardCopy)) pieceCopy.position.x++;
		pieceCopy.position.x--;
		if (!canPieceFit(pieceCopy, boardCopy)) pieceCopy.position.x++;
		while (canPieceFit(pieceCopy, boardCopy) || pieceCopy.position.x + pieceCopy.shape.length < boardDimensions.width) {
			if (canPieceFit(pieceCopy, boardCopy)){
				const shadConfig = findPieceShadow(pieceCopy, boardCopy);
				const newBoard = shadConfig[1];

				let pieceCenter;
				if (piece.pieceNum == 0) { // I piece
					pieceCenter = shadConfig[0].movedDown + (t % 2 == 0) ? 0.5 : 2;
				} else if (piece.pieceNum == 3) { // Square piece
					pieceCenter = shadConfig[0].movedDown + 1;
				} else { // Piece has dimension of 3x3
					pieceCenter = shadConfig[0].movedDown + (t % 2 == 0) ? 1 : 1.5;
				}
				// There is space above the piece on the third rotation
				if (t == 2) pieceCenter--;

				// If we are at the top we want to know what the best position for the next piece is
				let possibility = findBoardScore(newBoard, weightConfig, shadConfig[0].movedDown, pieceCopy.pieceNum);
				// if (Math.abs(score - oldScore) > 0.1) {
				// 	console.log("Dif");
				// }
				
				possibilities.push({
					rotations: t,
					position: {...pieceCopy.position},
					board: newBoard,
					score: possibility[0],
					linesCleared: possibility[1],
					movedDown: shadConfig[0].movedDown,
					pieceNum: pieceCopy.pieceNum
				});
			}
			
			pieceCopy.position.x++;
		}
		rotatePiece(pieceCopy, "right")
	}

	return possibilities;
}