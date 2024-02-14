const bumpinessScoreHistory = [];
const crackinessScoreHistory = [];
const movedDownHistory = [];

const levels = [
	"piece0Level",
	"piece1Level",
	"piece2Level",
	"piece3Level",
	"piece4Level",
	"piece5Level",
	"piece6Level"
];

const pieceMults = [
	"piece0DifferenceMultiplier",
	"piece1DifferenceMultiplier",
	"piece2DifferenceMultiplier",
	"piece3DifferenceMultiplier",
	"piece4DifferenceMultiplier",
	"piece5DifferenceMultiplier",
	"piece6DifferenceMultiplier"
]

function findBoardScore(board, weightConfigList, timeMovedDown, pieceNumber, logPlz, updateChartPlease=false) {

	let crackiness = 0;			// Spaces that are empty and in the column directly adjacent to the current one
	let coveredHoles = 0;	// Holes which we can't ever fill. They are inaccessible
	let wellDepth = 0;	// Holes which we can only partially fill. 3 deep or deeper
	let boardClearScore = 0;
	let rightColumnCount = 0;
	let linesCleared = 0;

	// Count the number of blocks in the rightmost column
	// The row of the minimum depth block in a column
	let depthList = [boardDimensions.height, boardDimensions.height, boardDimensions.height, boardDimensions.height, boardDimensions.height, boardDimensions.height, boardDimensions.height, boardDimensions.height, boardDimensions.height, boardDimensions.height];
	let columnCounts = [0,0,0,0,0,0,0,0,0,0];	// The counts of blocks in each column

	// Dropping this piece means lines could be cleared

	// The board that gets passed into this function already has the piece
	// dropped but lines have not been removed. So we remove them
	
	if (usingCharts) {
		board = structuredClone(board);
	}
	
	linesCleared = getRidOfClearedRows(board);

	// Yes, yes the following code might look a bit weird
	// I'm trying to avoid looping through columns in the board by just going through it line by line
	// It may not offer the speed benefits I think it does (given the scale), but its fun to think like this

	// I also wrote this originally for the C++ stuff so it might look weird because of that

	// Levels to apply different weight configs at
	// 0-8 Bottom
	// 9-15 Middle
	// 16-19(20) Top
	// We need to find that level
	// Do an initial scan down the board to find it

	// Find highest block
	let highestBlock = -1;
	for (let j = linesCleared; j < boardDimensions.height && highestBlock == -1; j++) {

		if (board[j].some( b => b != 7)) highestBlock = j;
		
	}

	// If the board is empty, highestBlock will still be -1
	highestBlock = highestBlock != -1 ? boardDimensions.height - highestBlock : 0;


	let weightConfig;
	if (weightConfigList.levelControlsSectionsBeneath ?? false) highestBlock += linesCleared;

	if (highestBlock < 9) {
		weightConfig = weightConfigList.bottom;
	} else if (highestBlock < 16) {
		weightConfig = weightConfigList.middle;
	} else {
		weightConfig = weightConfigList.top;
	}
	if (weightConfigList.levelControlsSectionsBeneath ?? false) highestBlock -= linesCleared;

	for (let j = 0; j < boardDimensions.height; j++) {
		let blocksInLine = 0;

		// Do an initial scan to update non-holes
		for (let i = 0; i < boardDimensions.width; i++) {
			// Update heights if need be
			if (board[j][i] != 7) {
				if (depthList[i] == boardDimensions.height) depthList[i] = j;
				
				// Increase column counts
				columnCounts[i]++;
				
				// Increase the number of blocks in the line
				blocksInLine++;

			}

		}

		for (let i = 0; i < boardDimensions.width; i++) {
			if (board[j][i] == 7) {		// Holes

				let crackCount = 0;

				crackCount += (i != 0 && j >= depthList[i-1]);	// If column is higher left of us
				crackCount += (i != 9 && j >= depthList[i+1]);	// If column is higher right of us
				
				crackiness += crackCount * (boardDimensions.height - j + 1) ** weightConfig.crackinessExponent;
				
				// Adjust unreachable holes
				if (j > depthList[i]) {
					coveredHoles += (boardDimensions.height - j + 1) ** weightConfig.coveredExponent;
				}


			}
		}

		// Increment the right column counter if necessary
		if (board[j][boardDimensions.width - 1] != 7) rightColumnCount++;

	}


	// Okay, after finding info, lets get some statistics
	// Wells
	// Left
	if (depthList[0] - depthList[1] >= 3) wellDepth += (depthList[0] - depthList[1]) ** weightConfig.wellExponent;

	// Middle
	for (let i = 1; i < boardDimensions.width - 1; i++) {
		const leftDif = depthList[i] - depthList[i - 1];
		const rightDif = depthList[i] - depthList[i + 1];

		if (leftDif >= 3 && rightDif >= 3) {
			const min = (leftDif < rightDif) ? leftDif : rightDif;
			wellDepth += min ** weightConfig.wellExponent;

			// There cannot be another well adjacent to us. Then we would not be a well
			i++;
		}
	}

	// Right
	if (depthList[9] - depthList[8] >= 3) wellDepth += (depthList[9] - depthList[8]) ** weightConfig.wellExponent;

	
	
	// Find height aggregateHeight
	let heightAgg = (boardDimensions.height * boardDimensions.width - depthList.reduce((a, b) => a + b)) / boardDimensions.width;
	
	// Now bumpiness
	let bumpiness = 0;
	for (let i = 1; i < boardDimensions.width; i++) {
		bumpiness += Math.abs(depthList[i] - depthList[i-1]);
	}
		
	// Average the amount in columns
	// It has come to my awareness that this is just a modified way of saying "fullness"
	let averageColumnLoad = columnCounts.reduce((a, b) => a + b) / boardDimensions.width;

	// Factor in the number of points the move would have given us
	let scoreValue = scoresForLines[linesCleared] ** weightConfig.scoreExponent;
	if (highestBlock == 0) boardClearScore = perfectClearScore[linesCleared] ** weightConfig.perfectScoreExponent;

	// Assign iPenalty?
	const iValue = (pieceNumber == 0) ? (20 - timeMovedDown) : 0;

	if (highestBlock >= 9 && highestBlock < 16) {
		bumpinessScoreHistory.push(Math.round(1000 * weightConfig.bumpiness * bumpiness) / 1000);
		crackinessScoreHistory.push(Math.round(1000 * weightConfig.crackiness * crackiness) / 1000);
		movedDownHistory.push(Math.round(1000 * weightConfig.movedDown * timeMovedDown) / 1000);
	}

	const centerMovedDifference = timeMovedDown - weightConfig[levels[pieceNumber]];


	// console.log(depthList);

	// console.log("hey hey",highestBlock, perfectClearScore[linesCleared], highestBlock - linesCleared, weightConfig.perfectScoreExponent);

	// console.log("crackiness: " + crackiness);
	// console.log("coveredHoles: " + coveredHoles);
	// console.log("wellDepth: " + wellDepth);
	// console.log("heightAgg: " + heightAgg);
	// console.log("linesCleared: " + linesCleared);
	// console.log("bumpiness: " + bumpiness);
	// console.log("highestBlock: " + highestBlock);
	// console.log("averageColumnLoad: " + averageColumnLoad);
	// console.log("scoreValue: " + scoreValue);
	// console.log("rightColumnCount: " + rightColumnCount);
	// console.log("timeMovedDown: " + timeMovedDown);
	// console.log("boardClearScore: " + boardClearScore);

	if (logPlz) {
		console.log(
			Math.round(100 * weightConfig.crackiness * crackiness) / 100,
			Math.round(100 * weightConfig.coveredHoles * coveredHoles) / 100,
			Math.round(100 * weightConfig.wellDepth * wellDepth) / 100,
			Math.round(100 * weightConfig.aggregateHeight * heightAgg) / 100,
			Math.round(100 * weightConfig.linesCleared * linesCleared) / 100,
			Math.round(100 * weightConfig.bumpiness * bumpiness) / 100,
			Math.round(100 * weightConfig.highestBlock * highestBlock) / 100,
			Math.round(100 * weightConfig.averageColumnLoad * averageColumnLoad) / 100,
			Math.round(100 * weightConfig.scoreValue * scoreValue) / 100,
			Math.round(100 * weightConfig.rightColumn * rightColumnCount) / 100,
			Math.round(100 * weightConfig.movedDown * timeMovedDown) / 100,
			Math.round(100 * weightConfig.boardClearScore * boardClearScore) / 100,
			Math.round(100 * weightConfig.iPenalty * iValue) / 100
		)
	}
	if (updateChartPlease) {
		updateChartData = {
			"crackiness": Math.round(1000 * weightConfig.crackiness * crackiness) / 1000,
			"coveredHoles": Math.round(1000 * weightConfig.coveredHoles * coveredHoles) / 1000,
			"wellDepth": Math.round(1000 * weightConfig.wellDepth * wellDepth) / 1000,
			"aggregateHeight": Math.round(1000 * weightConfig.aggregateHeight * heightAgg) / 1000,
			"linesCleared": Math.round(1000 * weightConfig.linesCleared * linesCleared) / 1000,
			"bumpiness": Math.round(1000 * weightConfig.bumpiness * bumpiness) / 1000,
			"highestBlock": Math.round(1000 * weightConfig.highestBlock * highestBlock) / 1000,
			"averageColumnLoad": Math.round(1000 * weightConfig.averageColumnLoad * averageColumnLoad) / 1000,
			"scoreValue": Math.round(1000 * weightConfig.scoreValue * scoreValue) / 1000,
			"rightColumn": Math.round(1000 * weightConfig.rightColumn * rightColumnCount) / 1000,
			"movedDown": Math.round(1000 * weightConfig.movedDown * timeMovedDown) / 1000,
			"boardClearScore": Math.round(1000 * weightConfig.boardClearScore * boardClearScore) / 1000,
			"iPenalty": Math.round(1000 * weightConfig.iPenalty * iValue) / 1000
		}
	}

	let finalScore = weightConfig.crackiness * crackiness +
				weightConfig.coveredHoles * coveredHoles +
				weightConfig.wellDepth * wellDepth +
				weightConfig.aggregateHeight * heightAgg +
				weightConfig.linesCleared * linesCleared +
				weightConfig.bumpiness * bumpiness +
				weightConfig.highestBlock * highestBlock +
				weightConfig.averageColumnLoad * averageColumnLoad +
				weightConfig.scoreValue * scoreValue +
				weightConfig.rightColumn * rightColumnCount +
				weightConfig.movedDown * timeMovedDown +
				weightConfig.boardClearScore * boardClearScore +
				weightConfig.iPenalty * iValue +
				weightConfig[pieceMults[pieceNumber]] * centerMovedDifference;

	return [finalScore, linesCleared];
}