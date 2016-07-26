% Vector plots
optionPlotTitles{ 1 } = 'Population size over time';
optionPlotTitles{ 2 } = 'Volume of heterotrophic population over time';
optionPlotTitles{ 3 } = 'Approximation for volume of heterotrophic population over time';
optionPlotTitles{ 4 } = 'Flux from autotroph to animal population over time';
optionPlotTitles{ 5 } = 'Intra-population flux from consumption by carnivores';
optionPlotTitles{ 6 } = 'Volume of autotroph population over time';
optionPlotTitles{ 7 } = 'Flux from nutrient to autotroph population over time';
optionPlotTitles{ 8 } = 'Volume of nutrient pool over time';
optionPlotTitles{ 9 } = 'Flux from animal to nutrient pool over time';
optionPlotTitles{ 10 } = 'Matter pool volumes over time';
% Matrix plots
optionPlotTitles{ 11 } = 'Size class sizes over time';
optionPlotTitles{ 12 } = 'Frequencies of vegetarians over time';
optionPlotTitles{ 13 } = 'Frequencies of carnivores over time';
optionPlotTitles{ 14 } = 'Frequencies of prey over time';
optionPlotTitles{ 15 } = 'Frequencies of starved individuals over time';
optionPlotTitles{ 16 } = 'Frequencies of parent organisms over time';
optionPlotTitles{ 17 } = 'Frequencies of child organisms over time';
optionPlotTitles{ 18 } = 'Frequencies of volume gene mutants over time';
optionPlotTitles{ 19 } = 'Size class volumes over time';
optionPlotTitles{ 20 } = 'Approximate size class volumes over time';
optionPlotTitles{ 21 } = 'Size class effective prey volume over time';
optionPlotTitles{ 22 } = 'Size class growth ratio over time';
optionPlotTitles{ 23 } = 'Size class coupling over time';
optionPlotTitles{ 24 } = 'Actual prey volume ratio over time';
optionPlotTitles{ 25 } = 'Feeding probabilities over time';
optionPlotTitles{ 26 } = 'Mean trophic classification over time';
optionPlotTitles{ 27 } = 'Mean age of size class over time';
optionPlotTitles{ 28 } = 'Frequencies of organisms'' ESV at end of model run';
% Trophic plots
optionPlotTitles{ 29 } = 'Frequencies of trophic levels over time';
optionPlotTitles{ 30 } = 'Volumes of trophic levels over time';
optionPlotTitles{ 31 } = 'Average age of trophic levels over time';
% Size-spectra plots
optionPlotTitles{ 32 } = 'Platt and Denman (1977) Size Spectra';
optionPlotTitles{ 33 } = 'Jennings and Blanchard (2004) Size Spectra';
optionPlotTitles{ 34 } = 'Rossberg (2012) Size Spectra';


optionFileNames{ 1 } = 'line_population_size';
optionFileNames{ 2 } = 'line_volume_population';
optionFileNames{ 3 } = 'line_volume_population_approximation';
optionFileNames{ 4 } = 'line_volume_flux_from_autotroph';
optionFileNames{ 5 } = 'line_volume_flux_from_carnivores';
optionFileNames{ 6 } = 'line_volume_autotroph';
optionFileNames{ 7 } = 'line_volume_flux_from_nutrient';
optionFileNames{ 8 } = 'line_volume_nutrient';
optionFileNames{ 9 } = 'line_volume_flux_from_heterotrophs';
optionFileNames{ 10 } = 'bar_volumes';

optionFileNames{ 11 } = 'size_class_sizes';
optionFileNames{ 12 } = 'size_class_frequencies_vegetarian';
optionFileNames{ 13 } = 'size_class_frequencies_carnivore';
optionFileNames{ 14 } = 'size_class_frequencies_prey';
optionFileNames{ 15 } = 'size_class_frequencies_starved';
optionFileNames{ 16 } = 'size_class_frequencies_parent';
optionFileNames{ 17 } = 'size_class_frequencies_child';
optionFileNames{ 18 } = 'size_class_frequencies_mutant_volume';
optionFileNames{ 19 } = 'size_class_volumes';
optionFileNames{ 20 } = 'size_class_volumes_approximate';
optionFileNames{ 21 } = 'size_class_volumes_effective_prey';
optionFileNames{ 22 } = 'size_class_growth_ratios';
optionFileNames{ 23 } = 'size_class_couplings';
optionFileNames{ 24 } = 'size_class_prey_volume_ratios';
optionFileNames{ 25 } = 'size_class_feeding_probabilities';
optionFileNames{ 26 } = 'size_class_trophic_classifications';
optionFileNames{ 27 } = 'size_class_ages';
optionFileNames{ 28 } = 'line_size_class_frequency_end';

optionFileNames{ 29 } = 'line_trophic_frequencies';
optionFileNames{ 30 } = 'line_trophic_volumes';
optionFileNames{ 31 } = 'line_trophic_ages';

optionFileNames{ 32 } = 'size_spectra_platt_denman';
optionFileNames{ 33 } = 'size_spectra_jennings_blanchard';
optionFileNames{ 34 } = 'size_spectra_rossberg';

for datumIndex = 1:length( optionFileNames )
    if datumIndex < 11
        disp( [ 'datumIndex> ' num2str( datumIndex ) ' line plot...' ] );
    elseif datumIndex < 29
        disp( [ 'datumIndex> ' num2str( datumIndex ) ' matrix plot...' ] );
    elseif datumIndex < 32
        disp( [ 'datumIndex> ' num2str( datumIndex ) ' trophic plot...' ] );
    elseif datumIndex < 35
        disp( [ 'datumIndex> ' num2str( datumIndex ) ' spectra plot...' ] );
    end
end