if (!(weightMultiConfig.catersToIndividualPieces ?? false)) {
	for (level in weightMultiConfig) {
		weightMultiConfig[level]["piece0Level"] = 0;
		weightMultiConfig[level]["piece1Level"] = 0;
		weightMultiConfig[level]["piece2Level"] = 0;
		weightMultiConfig[level]["piece3Level"] = 0;
		weightMultiConfig[level]["piece4Level"] = 0;
		weightMultiConfig[level]["piece5Level"] = 0;
		weightMultiConfig[level]["piece6Level"] = 0;
		weightMultiConfig[level]["piece0DifferenceMultiplier"] = 0;
		weightMultiConfig[level]["piece1DifferenceMultiplier"] = 0;
		weightMultiConfig[level]["piece2DifferenceMultiplier"] = 0;
		weightMultiConfig[level]["piece3DifferenceMultiplier"] = 0;
		weightMultiConfig[level]["piece4DifferenceMultiplier"] = 0;
		weightMultiConfig[level]["piece5DifferenceMultiplier"] = 0;
		weightMultiConfig[level]["piece6DifferenceMultiplier"] = 0;
	}
}