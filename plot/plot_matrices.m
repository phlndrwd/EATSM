%% Plot matrices

% Titles and plot file names should be in the same order as the datum names
% in the metadata file. The indicies here are defined by the enum in the
% model code.

if switchOnlyCountLevels ~= 1
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
    
    numberOfPlots = length( CellMatrixDatumNames );
    
    if switchQuickView == 1
        numberOfPlots = 2;
    end
    
    for matrixIndex = 1:numberOfPlots
        
        plotIndex = matrixIndex;
        
        if switchQuickView == 1 && matrixIndex == 2
            plotIndex = 9;
        end
        
        datumName = CellMatrixDatumNames{ plotIndex };
        
        load( strcat( [ optionOutputDirectory optionCurrentDataSet ], datumName, optionFileExtension ) );
        matrix = eval( datumName );
        
        if optionTruncateTimeAt > 0 && optionTruncateTimeAt < lengthAxisAbstractTime
            matrix = matrix( :, 1:optionTruncateTimeAt );
        end
        
        if optionResampleTimeTo > 0
            if plotIndex == 1 || plotIndex > 9
                matrix = ResampleMatrix( matrix, optionResampleTimeTo );
            else
                matrix = ResampleCumulativeMatrix( matrix, optionResampleTimeTo );
            end
        end
        
        matrix = ExtendMatrix( matrix );
        
        if plotIndex < 12 && plotIndex ~= 8 || plotIndex == 14
            matrix = LogMatrix( matrix, optionMissingValue );
        elseif plotIndex == 8 || plotIndex == 9
            matrix = ConvertZeroToNaN( matrix );
        elseif plotIndex == 10
            matrix = ConvertToNaN( matrix, optionMissingValue );
        else
            matrix = ConvertToNaN( matrix, optionMissingValue );
        end
        
        %% Collect data for population frequencies vector plot
        if plotIndex == 1
            vector = matrix( :, length( axisAbstractTime ) );
        end
        
        handlePlot = figure;
        
        set( gca,'FontSize', optionFontSize );
        pcolor( axisAbstractTimeExtended, vectorMatrixSizeAxis, matrix ), shading flat, colorbar
        set( gca, 'YScale', 'log' );
        
        SetColourOptions( optionColourMapName, switchReverseColourMap, optionFontSize, numberOfPopulations, plotIndex );
        AddPlotLabels( switchUseTitles, optionMatrixTitles{ plotIndex }, 'Time Steps', labelVolumeAxis );
        PrintToFile( handlePlot, optionOutputDirectory, optionCurrentDataSet, optionImageWidth, optionImageHeight, optionMatrixPlotFiles{ plotIndex }, optionOutputFileFormat );
        if optionCloseAfterPlotting == 1
            close;
        end
    end
    
    %% ESV frequencies at end of model run
    
    if switchQuickView ~= 1
        plotIndex = 18;
        
        labelYAxis = 'Individual Frequency Index';
        
        handlePlot = figure;
        
        set( gca,'FontSize', optionFontSize );
        semilogx( vectorMatrixSizeAxis, vector, 'k-', 'LineWidth', optionLineWidth );
        
        xlim( [ 0 vectorMatrixSizeAxis( length( vectorMatrixSizeAxis ) ) ] );
        AddPlotLabels( switchUseTitles, optionMatrixTitles{ plotIndex }, labelVolumeAxis, labelYAxis );
        PrintToFile( handlePlot, optionOutputDirectory, optionCurrentDataSet, optionImageWidth, optionImageHeight, optionMatrixPlotFiles{ plotIndex }, optionOutputFileFormat );
        if optionCloseAfterPlotting == 1
            close;
        end
    end
end
