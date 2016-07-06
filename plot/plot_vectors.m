%% Plot vectors

if switchOnlyCountLevels ~= 1
    optionVectorTitles{ 1 } = [ optionCurrentDataSet( 1:end - 1 ) ' Population size over time' ];
    optionVectorTitles{ 2 } = [ optionCurrentDataSet( 1:end - 1 ) ' Volume of heterotrophic population over time' ];
    optionVectorTitles{ 3 } = [ optionCurrentDataSet( 1:end - 1 ) ' Approximation for volume of heterotrophic population over time' ];
    optionVectorTitles{ 4 } = [ optionCurrentDataSet( 1:end - 1 ) ' Flux from phytoplankton to animal population over time' ];
    optionVectorTitles{ 5 } = [ optionCurrentDataSet( 1:end - 1 ) ' Intra-population flux from consumption by carnivores' ];
    optionVectorTitles{ 6 } = [ optionCurrentDataSet( 1:end - 1 ) ' Volume of phytoplankton population over time' ];
    optionVectorTitles{ 7 } = [ optionCurrentDataSet( 1:end - 1 ) ' Flux from nutrient to phytoplankton population over time' ];
    optionVectorTitles{ 8 } = [ optionCurrentDataSet( 1:end - 1 ) ' Volume of nutrient pool over time' ];
    optionVectorTitles{ 9 } = [ optionCurrentDataSet( 1:end - 1 ) ' Flux from animal to nutrient pool over time' ];
    optionVectorTitles{ 10 } = [ optionCurrentDataSet( 1:end - 1 ) ' Matter pool volumes over time' ];
    
    
    optionVectorPlotFiles{ 1 } = 'line_population_size';
    optionVectorPlotFiles{ 2 } = 'line_volume_population';
    optionVectorPlotFiles{ 3 } = 'line_volume_population_approximation';
    optionVectorPlotFiles{ 4 } = 'line_volume_flux_from_phytoplankton';
    optionVectorPlotFiles{ 5 } = 'line_volume_flux_from_carnivores';
    optionVectorPlotFiles{ 6 } = 'line_volume_phytoplankton';
    optionVectorPlotFiles{ 7 } = 'line_volume_flux_from_nutrient';
    optionVectorPlotFiles{ 8 } = 'line_volume_nutrient';
    optionVectorPlotFiles{ 9 } = 'line_volume_flux_from_heterotrophs';
    optionVectorPlotFiles{ 10 } = 'bar_volumes';
    
    volumeMatrixIndex = 3;
    volumeMatrix = zeros( length( axisAbstractTime ), volumeMatrixIndex );
    
    numberOfPlots = length( CellVectorDatumNames );
    
    for vectorIndex = 1:length( CellVectorDatumNames )
        
        datumName = CellVectorDatumNames{ vectorIndex };
        
        load( strcat( [ optionOutputDirectory optionCurrentDataSet ], datumName, optionFileExtension ) );
        vector = eval( datumName );
        
        if optionTruncateTimeAt > 0 && optionTruncateTimeAt < lengthAxisAbstractTime
            vector = vector( 1:truncateTimeDimensionTo );
        end
        
        if optionResampleTimeTo > 0
            if vectorIndex == 4 || vectorIndex == 5 || vectorIndex == 7 || vectorIndex == 9
                vector = ResampleCumulativeMatrix( vector, optionResampleTimeTo );
            else
                vector = ResampleMatrix( vector, optionResampleTimeTo );
            end
        end
        
        labelYAxis = 'Volume';
        
        %% Collect data for material closure plot
        if vectorIndex == 1
            labelYAxis = 'Individual Frequency';
        elseif vectorIndex == 2 || vectorIndex == 6 || vectorIndex == 8
            volumeMatrix( :, volumeMatrixIndex ) = vector;
            volumeMatrixIndex = volumeMatrixIndex - 1;
        end
        
        if switchQuickView ~= 1 || vectorIndex == 1
            if vectorIndex ~= 2 && vectorIndex ~= 3 && vectorIndex ~= 6 && vectorIndex ~= 8
                handlePlot = figure;
                
                set( gca,'FontSize', optionFontSize );
                plot( axisAbstractTime, vector, 'k-', 'LineWidth', optionLineWidth );
                xlim( [ min( axisAbstractTime ) max( axisAbstractTime )  ] );
                
                AddPlotLabels( switchUseTitles, optionVectorTitles{ vectorIndex }, labelTimeAxis, labelYAxis );
                PrintToFile( handlePlot, optionOutputDirectory, optionCurrentDataSet, optionImageWidth, optionImageHeight, optionVectorPlotFiles{ vectorIndex }, optionOutputFileFormat );
                if optionCloseAfterPlotting == 1
                    close;
                end
            end
        end
    end
    
    %% Post-process plots
    
    % Volume stacked bar chart
    
    vectorIndex = 10;
    
    handlePlot = figure;
    
    set( gca,'FontSize', optionFontSize );
    area( axisAbstractTime, volumeMatrix );
    legend( 'Nutrient', 'Phytoplankton', 'Population');
    xlim( [ min( axisAbstractTime ) max( axisAbstractTime )  ] );
    ylim( [ 0 ( sum( sum( volumeMatrix ) ) / length( axisAbstractTime ) ) + 1 ] );
    
    AddPlotLabels( switchUseTitles, optionVectorTitles{ vectorIndex }, labelTimeAxis, labelYAxis );
    PrintToFile( handlePlot, optionOutputDirectory, optionCurrentDataSet, optionImageWidth, optionImageHeight, optionVectorPlotFiles{ vectorIndex }, optionOutputFileFormat );
    if optionCloseAfterPlotting == 1
        close;
    end
end
