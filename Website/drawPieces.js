function drawNextPiece() {
	for (let n = 0; n < next_ele.length; n++) {
		next_ctx = next_ele[n].getContext("2d")

		next_ctx.fillStyle = "black";
		next_ctx.fillRect(0, 0, 90, 90);

		const blockWidth = 90 / (nextPiece.shape.length + 2);
		
		// Draw piece
		for (let j = 0; j < nextPiece.shape.length; j++) {
			for (let i = 0; i < nextPiece.shape.length; i++) {

				if ( nextPiece.shape[j][i] > 6) continue;
				
				next_ctx.fillStyle = numbersToColors[nextPiece.shape[j][i]];
				next_ctx.fillRect((1 + i)*blockWidth, (1 + j)*blockWidth, blockWidth, blockWidth);

			}
		}

		// Draw borders
		next_ctx.lineWidth = 1;
		next_ctx.strokeStyle = "white";
		for (let j = 0; j < nextPiece.shape.length; j++) {
			for (let i = 0; i < nextPiece.shape.length; i++) {

				if ( nextPiece.shape[j][i] > 6) continue;
				
				next_ctx.strokeRect((1 + i)*blockWidth, (1 + j)*blockWidth, blockWidth, blockWidth);

			}
		}
	}
}

function drawHeldPiece() {
	for (let n = 0; n < held_ele.length; n++) {
		held_ctx = held_ele[n].getContext("2d")

		held_ctx.fillStyle = "black";
		held_ctx.fillRect(0, 0, 90, 90);

		const blockWidth = 90 / (heldPiece.shape.length + 2);
		
		// Draw piece
		for (let j = 0; j < heldPiece.shape.length; j++) {
			for (let i = 0; i < heldPiece.shape.length; i++) {

				if ( heldPiece.shape[j][i] > 6) continue;
				
				held_ctx.fillStyle = numbersToColors[heldPiece.shape[j][i]];
				held_ctx.fillRect((1 + i)*blockWidth, (1 + j)*blockWidth, blockWidth, blockWidth);

			}
		}

		// Draw borders
		held_ctx.lineWidth = 1;
		held_ctx.strokeStyle = "white";
		for (let j = 0; j < heldPiece.shape.length; j++) {
			for (let i = 0; i < heldPiece.shape.length; i++) {

				if ( heldPiece.shape[j][i] > 6) continue;
				
				held_ctx.strokeRect((1 + i)*blockWidth, (1 + j)*blockWidth, blockWidth, blockWidth);

			}
		}
	}
}