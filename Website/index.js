var gamePaused = false;
var score = 0;
var lines = 0;
const DEFAULT_PROGRESS_SPEED = 30;
var progressSpeed = DEFAULT_PROGRESS_SPEED;
var instantPlace = false;
var hasPieceLanded = true;

var previousCheck;

var currentPiece = generateAPiece();
var nextPiece = generateAPiece();
var heldPiece = generateAPiece();

var usingCharts = false;

drawNextPiece();
drawHeldPiece();

drawBoard();

/*
	I have some Dad notes
	1) We should add more checks. Because we know what the next 2 blocks are (current piece, next piece, held piece)) we
	   should go through those and find what the best possible scenario to be in is. Probably only go 2 deep though.
	   On top of this we should also add a last check at the end for "what would happen if an I piece came next?". Because
	   that's what we're trying to maximize right? Number of tetrises. BUT if we do that, we should add a new gene that
	   Dad called "optimism" which gets applied to that additional I piece. The idea is that you want to be less
	   optimistic that an I piece will come when you are nearing the top of the board
	2) Split the board into 3 sections and each section should have it's own configuration for scoring. If we do this, get
	   rid of the exponent values. Just use multipliers. 
	3) Maybe add another part to the algorithm placing the piece. Say that there are a lot of scenarios where the AI knows
	   best, but there are also a lot where WE as Tetris players know a move is bad. How do I do this?
*/

function togglePause() {
	if (!gamePaused) {
		gamePaused = true;
	} else if (gamePaused) {
		gamePaused = false;
		if (instantPlace) {
			autoIncrementInstant();
		} else {
			autoIncrementNormal();
		}
	}
}

function generateAPiece() {
	const pieceNum = Math.floor(Math.random() * tetrominoes.length);
	return {
		pieceNum: pieceNum,
		shape: structuredClone(tetrominoes[pieceNum]),
		position: {
			x: 3, y: 0
		},
		rotations: 0,
	}
}

function canPieceFit(piece, board) {
	
	// Loop through rows of piece at locations in board and check to see if either or both are 7
	for (let j = 0; j < piece.shape.length; j++) {
		for (let i = 0; i < piece.shape[j].length; i++) {
			if (piece.shape[j][i] != 7 && (piece.position.y + j >= boardDimensions.height || (board[piece.position.y + j][piece.position.x + i] != 7))) 
				return false;
		}
	}
	return true;
}

function movePieceDown(piece, board) {
	if (!canPieceFit(piece, board)) return false;
	piece.position.y++;
	if (canPieceFit(piece, board)) {
		return true;
	} else {
		piece.position.y--;
		return false;
	}
}

function rotatePiece(piece, direction) {

	const size = piece.shape.length
	let rotatedMatrix = []

	// Initialize the rotatedMatrix with zeros
	for (let i = 0; i < size; i++) {
		rotatedMatrix.push(new Array(size).fill(0));
	}

	if (direction === "right") {
		for (let j = 0; j < size; j++) {
			for (let i = 0; i < size; i++) {
				rotatedMatrix[i][size - 1 - j] = piece.shape[j][i];
			}
		}
	} else if (direction === "left") {
		for (let j = 0; j < size; j++) {
			for (let i = 0; i < size; i++) {
				rotatedMatrix[size - 1 - i][j] = piece.shape[j][i];
			}
		}
	}

	piece.shape = rotatedMatrix;
}

function findPieceShadow(piece, board) {
	const pieceShadow = structuredClone(piece);
	const boardShadow = structuredClone(board);

	let movedDown = 0;

	while (movePieceDown(pieceShadow, boardShadow)) {
		movedDown++;
	}
	pieceShadow.movedDown = movedDown;

	// Draw the piece onto the board
	for (let j = 0; j < pieceShadow.shape.length && pieceShadow.position.y + j < boardDimensions.height; j++) {

		for (let i = 0; i < pieceShadow.shape[j].length && pieceShadow.position.x + i < boardDimensions.width; i++) {

			if (pieceShadow.shape[j][i] != 7) {
				boardShadow[pieceShadow.position.y + j][pieceShadow.position.x + i] = pieceShadow.shape[j][i]
			}
		}
	}

	return [pieceShadow, boardShadow];
}

function tryFitShape() {
	if (currentPiece.pieceNum != 0) {
		for (let i = 0; i < 4; i++) {
			currentPiece.position.x += tryTurnOrdersNotI[currentPiece.rotations][i][0]
			currentPiece.position.y += tryTurnOrdersNotI[currentPiece.rotations][i][1]
			if (canPieceFit(currentPiece, boardState)) {
				console.log("fit on", i);
				return true;
			}
			currentPiece.position.x -= tryTurnOrdersNotI[currentPiece.rotations][i][0]
			currentPiece.position.y -= tryTurnOrdersNotI[currentPiece.rotations][i][1]
		}
		return false;
	} else {
		for (let i = 0; i < 4; i++) {
			currentPiece.position.x += tryTurnOrdersI[currentPiece.rotations][i][0]
			currentPiece.position.y += tryTurnOrdersI[currentPiece.rotations][i][1]
			if (canPieceFit(currentPiece, boardState)) {
				console.log("fit on", i);
				return true;
			}
			currentPiece.position.x -= tryTurnOrdersI[currentPiece.rotations][i][0]
			currentPiece.position.y -= tryTurnOrdersI[currentPiece.rotations][i][1]
		}
		return false;
	}
}

function switchHeldPiece() {
	const temp = heldPiece;
	heldPiece = {...currentPiece};
	currentPiece = {...temp};
	drawHeldPiece();
}

function applyOrientation(piece, orientation, reverse) {
	// console.log(piece, orientation);
	if (reverse) {
		piece.position = orientation.position;
		for (let i = 0; i < orientation.rotations; i++) rotatePiece(piece, "left");
	} else {
		piece.position = orientation.position;
		for (let i = 0; i < orientation.rotations; i++) rotatePiece(piece, "right");
	}

	return piece;
}

var iterCount = 0;
var prevRowTetrisClearOut = false;


if (instantPlace) {
	autoIncrementInstant();
} else {
	autoIncrementNormal();
}

document.addEventListener("keydown", event => {
	
	switch(event.key) {
		// case "ArrowUp":
		// 	rotatePiece(currentPiece, "right");
		// 	if (!tryFitShape()) {
		// 		rotatePiece(currentPiece, "left");
		// 	} else {
		// 		currentPiece.rotations = (currentPiece.rotations + 1) % 4;
		// 	}
		// 	// drawBoard();
		// 	break;
		// case "ArrowLeft":
		// 	currentPiece.position.x--;
		// 	if (!canPieceFit(currentPiece, boardState)) currentPiece.position.x++;
		// 	// drawBoard();
		// 	break;
		// case "ArrowRight":
		// 	currentPiece.position.x++;
		// 	if (!canPieceFit(currentPiece, boardState)) currentPiece.position.x--;
		// 	// drawBoard();
		// 	break;
		// case "ArrowDown":
		// 	movePieceDown(currentPiece, boardState);
		// 	// drawBoard();
		// 	break;
		// case "z":
		// 	switchHeldPiece();
		// 	// drawBoard();
		// 	break;
		case "1":
			progressSpeed = 10;
			break;
		case "2":
			progressSpeed = 20;
			break;
		case "3":
			progressSpeed = 30;
			break;
		case "4":
			progressSpeed = 40;
			break;
		case "5":
			progressSpeed = 50;
			break;
		case "6":
			progressSpeed = 60;
			break;
		case "7":
			progressSpeed = 70;
			break;
		case "8":
			progressSpeed = 80;
			break;
		case "9":
			progressSpeed = 90;
			break;
		case "0":
			progressSpeed = 0;
			break;
		case "Escape":
			togglePause();
			break;
	}
});
// document.addEventListener("keyup", event => {
// 	switch(event.key) {
// 		case " ":
// 			instantPlace = false;
// 			break;
// 	}
// });