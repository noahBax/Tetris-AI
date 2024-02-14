const scoreEle = document.getElementById("score");
const linesEle = document.getElementById("lines");
const ratioEle = document.getElementById("ratio");
// const fitnessEle = document.getElementById("fitness");
const board = document.getElementById("gameBoard");
const next_ele = document.getElementsByClassName("next");
const held_ele = document.getElementsByClassName("held");
const ctx = board.getContext("2d", { alpha: false });
const pixelDimension = 100;
const boardDimensions = {
	width: 10,
	height: 20
};
const OGweightConfig = {
	crackiness : 12.334284194900137,
	coveredHoles : 13.061927428806257,
	wellDepth : 15.306537913043336,
	aggregateHeight : -17.07030019908656,
	linesCleared : 15.128122414362249,
	bumpiness: 0,
	highestBlock : 15.316930994684759,
	scoreValue: 0,
	averageColumnLoad: 0,
	wellExponent: 2,
	rightColumn : 0,
	crackinessExponent: 2,
	coveredExponent: 3,
	movedDown: 0,
	boardClearScore: 0,
	scoreExponent: 1,
	perfectScoreExponent: 1
}
const OGOGweightConfig = {
	crackiness : 6.29609204496402,
	coveredHoles : 6.6885647433507565,
	wellDepth : 2.312951618647077,
	aggregateHeight : 0.24010987618705315,
	linesCleared : 0.019120552084399858,
	bumpiness: 0,
	highestBlock : 2.5780557546950127,
	scoreValue : -5.800741647528431,
	averageColumnLoad : -1.3772646335537115,
	wellExponent : -0.16626569655959308,
	rightColumn : 1.0039819278170223,
	crackinessExponent : 0.7862391593243911,
	coveredExponent : 5.770615441940043,
	movedDown : -2.5302772589270486,
	boardClearScore : -0.744632468470819,
	scoreExponent : 1.5210840302729025,
	perfectScoreExponent : 0.22865744206938926,
}
const weightMultiConfig = {
	"bottom": {
		"crackiness":                 2.462177,
		"coveredHoles":               4.213561,
		"wellDepth":                  1.974837,
		"aggregateHeight":            1.051255,
		"linesCleared":               1.125282,
		"bumpiness":                  3.457410,
		"highestBlock":               0.075452,
		"scoreValue":                 4.073953,
		"averageColumnLoad":          0.882835,
		"wellExponent":               -0.706753,
		"rightColumn":                1.142039,
		"crackinessExponent":         -1.178477,
		"coveredExponent":            3.321470,
		"movedDown":                  -4.855410,
		"boardClearScore":            3.498482,
		"scoreExponent":              2.160657,
		"perfectScoreExponent":       1.556205,
		"iPenalty":                   4.400545,
		"piece0Level":                7.716495,
		"piece1Level":                7.113437,
		"piece2Level":                8.877099,
		"piece3Level":                8.016982,
		"piece4Level":                7.581215,
		"piece5Level":                8.763940,
		"piece6Level":                7.112513,
		"piece0DifferenceMultiplier": 2.214318,
		"piece1DifferenceMultiplier": 1.230938,
		"piece2DifferenceMultiplier": 1.591849,
		"piece3DifferenceMultiplier": 1.665181,
		"piece4DifferenceMultiplier": 0.040143,
		"piece5DifferenceMultiplier": 2.260007,
		"piece6DifferenceMultiplier": -0.411186
	  },
	  "middle": {
		"crackiness":                 5.443155,
		"coveredHoles":               8.702331,
		"wellDepth":                  5.179860,
		"aggregateHeight":            3.964215,
		"linesCleared":               5.023315,
		"bumpiness":                  2.490996,
		"highestBlock":               1.815573,
		"scoreValue":                 -2.525520,
		"averageColumnLoad":          2.478013,
		"wellExponent":               0.640240,
		"rightColumn":                0.732688,
		"crackinessExponent":         -0.779876,
		"coveredExponent":            2.825051,
		"movedDown":                  -1.993646,
		"boardClearScore":            1.437563,
		"scoreExponent":              0.673677,
		"perfectScoreExponent":       0.780340,
		"iPenalty":                   4.368075,
		"piece0Level":                13.373053,
		"piece1Level":                8.862014,
		"piece2Level":                7.786785,
		"piece3Level":                7.609111,
		"piece4Level":                4.591571,
		"piece5Level":                7.978232,
		"piece6Level":                8.057349,
		"piece0DifferenceMultiplier": 1.232564,
		"piece1DifferenceMultiplier": 1.478242,
		"piece2DifferenceMultiplier": 2.445657,
		"piece3DifferenceMultiplier": 0.650289,
		"piece4DifferenceMultiplier": 1.685982,
		"piece5DifferenceMultiplier": 0.509533,
		"piece6DifferenceMultiplier": 1.031855
	  },
	  "top": {
		"crackiness":                 6.286393,
		"coveredHoles":               2.511931,
		"wellDepth":                  0.936958,
		"aggregateHeight":            -3.835306,
		"linesCleared":               1.427845,
		"bumpiness":                  -1.947792,
		"highestBlock":               0.768784,
		"scoreValue":                 -5.661859,
		"averageColumnLoad":          -0.112371,
		"wellExponent":               0.815619,
		"rightColumn":                1.685756,
		"crackinessExponent":         3.363692,
		"coveredExponent":            3.209662,
		"movedDown":                  0.094367,
		"boardClearScore":            0.648644,
		"scoreExponent":              2.503868,
		"perfectScoreExponent":       1.370793,
		"iPenalty":                   -0.273263,
		"piece0Level":                7.964552,
		"piece1Level":                7.205371,
		"piece2Level":                6.839992,
		"piece3Level":                6.853848,
		"piece4Level":                7.646610,
		"piece5Level":                11.496201,
		"piece6Level":                8.380904,
		"piece0DifferenceMultiplier": 1.611664,
		"piece1DifferenceMultiplier": 0.618231,
		"piece2DifferenceMultiplier": 0.597321,
		"piece3DifferenceMultiplier": 2.086420,
		"piece4DifferenceMultiplier": 2.060068,
		"piece5DifferenceMultiplier": 2.112654,
		"piece6DifferenceMultiplier": 1.119208
	  },
	  "levelControlsSectionsBeneath": true,
	  "catersToIndividualPieces": false
	  
}

const weightConfig = {
	crackiness:              7.01361,
	coveredHoles:        5.16102,
	wellDepth:         0.92767,
	aggregateHeight:               2.32184,
	linesCleared:            -3.41262,
	bumpiness: 0,
	highestBlock:            4.43476,
	scoreValue:              -4.69787,
	averageColumnLoad:       3.6851,
	wellExponent:         -2.04528,
	rightColumn:             15.7326,
	crackinessExponent:   0.121492,
	coveredExponent:  2.83857,
	movedDown:               -3.2087,
	boardClearScore:         -1.34793,
	scoreExponent:        0.541196,
	perfectScoreExponent: -0.564741,
}

const ActuallyGoodweightConfig = {
	crackiness:              7.01361,
	coveredHoles:        5.16102,
	wellDepth:         0.92767,
	aggregateHeight:               2.32184,
	linesCleared:            -3.41262,
	bumpiness: 0,
	highestBlock:            4.43476,
	scoreValue:              -4.69787,
	averageColumnLoad:       3.6851,
	wellExponent:         -2.04528,
	rightColumn:             15.7326,
	crackinessExponent:   0.121492,
	coveredExponent:  2.83857,
	movedDown:               -3.2087,
	boardClearScore:         -1.34793,
	scoreExponent:        0.541196,
	perfectScoreExponent: -0.564741,
}
const RiskyButGoodweightConfig = {
	crackiness : 6.436394578695133,
	coveredHoles : 6.176589757833838,
	wellDepth : 7.6888741939238425,
	aggregateHeight : 0.4145672859499802,
	linesCleared : -0.08514015314571571,
	bumpiness: 0,
	highestBlock : 2.0472780316811154,
	scoreValue : -5.774021161590162,
	averageColumnLoad : -0.618991999697075,
	wellExponent : -0.6632149380931032,
	rightColumn : 0.07561636414610629,
	crackinessExponent : 0.5389183176716,
	coveredExponent : 5.127902425202187,
	movedDown : -2.331194452395347,
	boardClearScore : -0.9409588346461982,
	scoreExponent : 0.21370379365281167,
	perfectScoreExponent : -0.0648265595598337,
}
const colorsToNumbers = {
	"cyan": 0,
	"blue": 1,
	"orange": 2,
	"yellow": 3,
	"green": 4,
	"purple": 5,
	"red": 6,
	"white": 7,
	"black": 8
};
const numbersToColors = [
	"rgb(0,240,240)",	/* cyan */
	"rgb(0,0,240)",		/* blue */
	"rgb(240,160,0)",	/* orange */
	"rgb(240,240,0)",	/* yellow */
	"rgb(0,240,0)",		/* green */
	"rgb(160,0,240)",	/* purple */
	"rgb(240,0,0",		/* red */
	"rgb(120,120,120)",	/* white */
	"rgb(0,0,0)",		/* black */
];
const tetrominoes = [
	[	/* cyan */
		[ 7, 7, 7, 7 ],
		[ 0, 0, 0, 0 ],
		[ 7, 7, 7, 7 ],
		[ 7, 7, 7, 7 ],
	],
	[	/* "blue" */
		[ 1, 7, 7 ],
		[ 1, 1, 1 ],
		[ 7, 7, 7 ],
	],
	[	/* "orange" */
		[ 7, 7, 2 ],
		[ 2, 2, 2 ],
		[ 7, 7, 7 ],
	],
	[	/* "yellow" */
		[ 3, 3 ], 
		[ 3, 3 ], 
	],
	[	/* "green" */
		[ 7, 4, 4 ],
		[ 4, 4, 7 ],
		[ 7, 7, 7 ],
	],
	[	/* "purple" */
		[ 7, 5, 7 ],
		[ 5, 5, 5 ],
		[ 7, 7, 7 ],
	],
	[	/* "red" */
		[ 6, 6, 7 ],
		[ 7, 6, 6 ],
		[ 7, 7, 7 ],
	]
];
const boardState = [
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,],
	[ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,]
];
const tryTurnOrdersNotI = [
	[ [ 0, 0], [-1, 0], [-1,+1], [ 0,-2], [-1,-2] ],
	[ [ 0, 0], [ 1, 0], [ 1,-1], [ 0, 2], [ 1, 2] ],
	[ [ 0, 0], [ 1, 0], [ 1, 1], [ 0,-2], [ 1,-2] ],
	[ [ 0, 0], [-1, 0], [-1,-1], [ 0, 2], [-1, 2] ]
]
const tryTurnOrdersI = [
	[ [ 0, 0], [-2, 0], [ 1, 0], [-2,-1], [ 1, 2] ],
	[ [ 0, 0], [-1, 0], [ 2, 0], [-1, 2], [ 2,-1] ],
	[ [ 0, 0], [ 2, 0], [-1, 0], [ 2, 1], [-1,-2] ],
	[ [ 0, 0], [ 1, 0], [-2, 0], [ 1,-2], [-2,+1] ],
]
const scoresForLines = [0, 100, 300, 500, 800];
const perfectClearScore = [0,800, 1200, 1800, 2000, 3200];