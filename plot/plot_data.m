
optionMatrixTitles{ 1 } = [ optionCurrentDataSet( 1:end - 1 ) ' Size class sizes over time' ];
optionMatrixTitles{ 2 } = [ optionCurrentDataSet( 1:end - 1 )  ' Frequencies of vegetarians over time' ];
optionMatrixTitles{ 3 } = [ optionCurrentDataSet( 1:end - 1 ) ' Frequencies of carnivores over time' ];
optionMatrixTitles{ 4 } = [ optionCurrentDataSet( 1:end - 1 ) ' Frequencies of prey over time' ];
optionMatrixTitles{ 5 } = [ optionCurrentDataSet( 1:end - 1 ) ' Frequencies of starved individuals over time' ];
optionMatrixTitles{ 6 } = [ optionCurrentDataSet( 1:end - 1 ) ' Frequencies of parent organisms over time' ];
optionMatrixTitles{ 7 } = [ optionCurrentDataSet( 1:end - 1 ) ' Frequencies of child organisms over time' ];
optionMatrixTitles{ 8 } = [ optionCurrentDataSet( 1:end - 1 ) ' Frequencies of volume gene mutants over time' ];
optionMatrixTitles{ 9 } = [ optionCurrentDataSet( 1:end - 1 ) ' Size class volumes over time' ];
optionMatrixTitles{ 10 } = [ optionCurrentDataSet( 1:end - 1 ) ' Approximate size class volumes over time' ];
optionMatrixTitles{ 11 } = [ optionCurrentDataSet( 1:end - 1 ) ' Size class effective prey volume over time' ];
optionMatrixTitles{ 12 } = [ optionCurrentDataSet( 1:end - 1 ) ' Size class growth ratio over time' ];
optionMatrixTitles{ 13 } = [ optionCurrentDataSet( 1:end - 1 ) ' Size class coupling over time' ];
optionMatrixTitles{ 14 } = [ optionCurrentDataSet( 1:end - 1 ) ' Actual prey volume ratio over time' ];
optionMatrixTitles{ 15 } = [ optionCurrentDataSet( 1:end - 1 ) ' Feeding probabilities over time' ];
optionMatrixTitles{ 16 } = [ optionCurrentDataSet( 1:end - 1 ) ' Mean trophic classification over time' ];
optionMatrixTitles{ 17 } = [ optionCurrentDataSet( 1:end - 1 ) ' Mean age of size class over time' ];
optionMatrixTitles{ 18 } = [ optionCurrentDataSet( 1:end - 1 ) ' Frequencies of organisms'' ESV at end of model run' ];

optionVectorTitles{ 1 } = [ optionCurrentDataSet( 1:end - 1 ) ' Population size over time' ];
optionVectorTitles{ 2 } = [ optionCurrentDataSet( 1:end - 1 ) ' Volume of heterotrophic population over time' ];
optionVectorTitles{ 3 } = [ optionCurrentDataSet( 1:end - 1 ) ' Approximation for volume of heterotrophic population over time' ];
optionVectorTitles{ 4 } = [ optionCurrentDataSet( 1:end - 1 ) ' Flux from autotroph to animal population over time' ];
optionVectorTitles{ 5 } = [ optionCurrentDataSet( 1:end - 1 ) ' Intra-population flux from consumption by carnivores' ];
optionVectorTitles{ 6 } = [ optionCurrentDataSet( 1:end - 1 ) ' Volume of autotroph population over time' ];
optionVectorTitles{ 7 } = [ optionCurrentDataSet( 1:end - 1 ) ' Flux from nutrient to autotroph population over time' ];
optionVectorTitles{ 8 } = [ optionCurrentDataSet( 1:end - 1 ) ' Volume of nutrient pool over time' ];
optionVectorTitles{ 9 } = [ optionCurrentDataSet( 1:end - 1 ) ' Flux from animal to nutrient pool over time' ];
optionVectorTitles{ 10 } = [ optionCurrentDataSet( 1:end - 1 ) ' Matter pool volumes over time' ];

optionDistributionTitles{ 1 } = [ optionCurrentDataSet( 1:end - 1 ) ' Platt and Denman (1977) Size Spectra' ];
optionDistributionTitles{ 2 } = [ optionCurrentDataSet( 1:end - 1 ) ' Jennings and Blanchard (2004) Size Spectra' ];
optionDistributionTitles{ 3 } = [ optionCurrentDataSet( 1:end - 1 ) ' Rossberg (2012) Size Spectra' ];

optionTrophicTitles{ 1 } = [ optionCurrentDataSet( 1:end - 1 )  ' Frequencies of trophic levels over time' ];
optionTrophicTitles{ 2 } = [ optionCurrentDataSet( 1:end - 1 )  ' Volumes of trophic levels over time' ];
optionTrophicTitles{ 3 } = [ optionCurrentDataSet( 1:end - 1 )  ' Average age of trophic levels over time' ];


optionMatrixPlotFiles{ 1 } = 'size_class_sizes';
optionMatrixPlotFiles{ 2 } = 'size_class_frequencies_vegetarian';
optionMatrixPlotFiles{ 3 } = 'size_class_frequencies_carnivore';
optionMatrixPlotFiles{ 4 } = 'size_class_frequencies_prey';
optionMatrixPlotFiles{ 5 } = 'size_class_frequencies_starved';
optionMatrixPlotFiles{ 6 } = 'size_class_frequencies_parent';
optionMatrixPlotFiles{ 7 } = 'size_class_frequencies_child';
optionMatrixPlotFiles{ 8 } = 'size_class_frequencies_mutant_volume';
optionMatrixPlotFiles{ 9 } = 'size_class_volumes';
optionMatrixPlotFiles{ 10 } = 'size_class_volumes_approximate';
optionMatrixPlotFiles{ 11 } = 'size_class_volumes_effective_prey';
optionMatrixPlotFiles{ 12 } = 'size_class_growth_ratios';
optionMatrixPlotFiles{ 13 } = 'size_class_couplings';
optionMatrixPlotFiles{ 14 } = 'size_class_prey_volume_ratios';
optionMatrixPlotFiles{ 15 } = 'size_class_feeding_probabilities';
optionMatrixPlotFiles{ 16 } = 'size_class_trophic_classifications';
optionMatrixPlotFiles{ 17 } = 'size_class_ages';
optionMatrixPlotFiles{ 18 } = 'line_size_class_frequency_end';

optionVectorPlotFiles{ 1 } = 'line_population_size';
optionVectorPlotFiles{ 2 } = 'line_volume_population';
optionVectorPlotFiles{ 3 } = 'line_volume_population_approximation';
optionVectorPlotFiles{ 4 } = 'line_volume_flux_from_autotroph';
optionVectorPlotFiles{ 5 } = 'line_volume_flux_from_carnivores';
optionVectorPlotFiles{ 6 } = 'line_volume_autotroph';
optionVectorPlotFiles{ 7 } = 'line_volume_flux_from_nutrient';
optionVectorPlotFiles{ 8 } = 'line_volume_nutrient';
optionVectorPlotFiles{ 9 } = 'line_volume_flux_from_heterotrophs';
optionVectorPlotFiles{ 10 } = 'bar_volumes';

optionTrophicPlotFiles{ 1 } = 'line_trophic_frequencies';
optionTrophicPlotFiles{ 2 } = 'line_trophic_volumes';
optionTrophicPlotFiles{ 3 } = 'line_trophic_ages';

optionDistributionPlotFiles{ 1 } = 'plot_size_spectra_platt_denman';
optionDistributionPlotFiles{ 2 } = 'plot_size_spectra_jennings_blanchard';
optionDistributionPlotFiles{ 3 } = 'plot_size_spectra_rossberg';

for plotIndex = 1:numberOfPlots
    
end