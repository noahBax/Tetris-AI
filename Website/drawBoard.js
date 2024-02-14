function drawBoard() {
	
	// Refresh board
	ctx.fillStyle = "black"
	ctx.fillRect(0, 0, boardDimensions.width*pixelDimension, boardDimensions.height*pixelDimension)
	
	// Draw grid
	ctx.strokeStyle = "grey";
	ctx.lineWidth = 0.5;
	for (let j = 0; j < boardDimensions.height; j++) {
		for (let i = 0; i < boardDimensions.width; i++) {

			ctx.strokeRect(i*pixelDimension, j*pixelDimension, pixelDimension, pixelDimension)

		}
	}

	// Draw current board state
	ctx.lineWidth = 5;
	ctx.strokeStyle = "white"
	for (let j = 0; j < boardDimensions.height; j++) {
		for (let i = 0; i < boardDimensions.width; i++) {

			if (boardState[j][i] == 7) continue;

			ctx.fillStyle = numbersToColors[boardState[j][i]];
			ctx.fillRect(i*pixelDimension, j*pixelDimension, pixelDimension, pixelDimension);

		}
	}
	
	// Draw piece on the board
	for (let j = 0; j < currentPiece.shape.length; j++) {
		for (let i = 0; i < currentPiece.shape.length; i++) {

			if ( currentPiece.shape[j][i] > 6) continue;
			
			ctx.fillStyle = numbersToColors[currentPiece.shape[j][i]];
			ctx.fillRect((currentPiece.position.x + i)*pixelDimension, (currentPiece.position.y + j)*pixelDimension, pixelDimension, pixelDimension);

		}
	}

	// Go through that loop again and draw all the borders
	for (let j = 0; j < boardDimensions.height; j++) {
		for (let i = 0; i < boardDimensions.width; i++) {

			if (boardState[j][i] > 6) continue;

			ctx.strokeRect(i*pixelDimension, j*pixelDimension, pixelDimension, pixelDimension)

		}
	}

	for (let j = 0; j < currentPiece.shape.length; j++) {
		for (let i = 0; i < currentPiece.shape.length; i++) {

			if ( currentPiece.shape[j][i] > 6) continue;
			
			ctx.strokeRect((currentPiece.position.x + i)*pixelDimension, (currentPiece.position.y + j)*pixelDimension, pixelDimension, pixelDimension)
		}
	}

	// Draw piece shadow
	if (!instantPlace) {
		const shadow = findPieceShadow(currentPiece, boardState)[0];
		for (let j = 0; j < shadow.shape.length; j++) {
			for (let i = 0; i < shadow.shape.length; i++) {
	
				if ( shadow.shape[j][i] > 6) continue;
				
				ctx.strokeRect((shadow.position.x + i)*pixelDimension, (shadow.position.y + j)*pixelDimension, pixelDimension, pixelDimension)
			}
		}
	}

	requestAnimationFrame(drawBoard);
}