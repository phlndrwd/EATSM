warning( 'off', 'all' );

if switchOnlyCountLevels ~= 1
    
    optionDistributionTitles{ 1 } = [ optionCurrentDataSet( 1:end - 1 ) ' Platt and Denman (1977) Size Spectra' ];
    optionDistributionTitles{ 2 } = [ optionCurrentDataSet( 1:end - 1 ) ' Jennings and Blanchard (2004) Size Spectra' ];
    optionDistributionTitles{ 3 } = [ optionCurrentDataSet( 1:end - 1 ) ' Rossberg (2012) Size Spectra' ];
    
    optionDistributionPlotFiles{ 1 } = 'plot_size_spectra_platt_denman';
    optionDistributionPlotFiles{ 2 } = 'plot_size_spectra_jennings_blanchard';
    optionDistributionPlotFiles{ 3 } = 'plot_size_spectra_rossberg';
    
    datumName = CellMatrixDatumNames{ 9 };
    load( strcat( [ optionOutputDirectory optionCurrentDataSet ], datumName, optionFileExtension ) );
    matrix = eval( datumName );
    
    vectorLength = length( vectorMatrixSizeAxis );
    indexHalfwayThrough = round( lengthAxisAbstractTime / 2 );
    
    sizeSpectra = zeros( 3, vectorLength - 1 );
    biomassVolume = zeros( 1, vectorLength - 1 );
    vectorIntervalWidths = biomassVolume;
    
    for index = 1:vectorLength
        
        if index < vectorLength
            biomassVolume( index ) = matrix( index, indexHalfwayThrough );
            vectorIntervalWidths( index ) = vectorMatrixSizeAxis( index + 1 ) - vectorMatrixSizeAxis( index );
            
            meanHalfBiomassThisTimeStep = mean( matrix( index, indexHalfwayThrough:end ) );
            
            sizeSpectra( 1, index ) = ConvertZeroToNaN( biomassVolume( index ) / vectorIntervalWidths( index ) );
            sizeSpectra( 2, index ) = ConvertZeroToNaN( log10( meanHalfBiomassThisTimeStep ) );
            sizeSpectra( 3, index ) = ConvertZeroToNaN( ( meanHalfBiomassThisTimeStep * vectorVectorSizeAxis( index ) ) /  vectorIntervalWidths( index ) );
        end
    end
    
    labelYAxis = '\beta(i)';
    
    spectraIndex = 1;
    handlePlot = figure;
    loglog( vectorVectorSizeAxis, sizeSpectra( spectraIndex, : ), 'k-', 'LineWidth', optionLineWidth );
    xlim( [ 1 vectorMatrixSizeAxis( length( vectorMatrixSizeAxis ) ) ] );
    AddPlotLabels( switchUseTitles, optionDistributionTitles{ spectraIndex }, labelVolumeAxis, labelYAxis );
    PrintToFile( handlePlot, optionOutputDirectory, optionCurrentDataSet, optionImageWidth, optionImageHeight, optionDistributionPlotFiles{ spectraIndex }, optionOutputFileFormat );
    if optionCloseAfterPlotting == 1
        close;
    end
    
    if switchQuickView ~= 1
        spectraIndex = 2;
        handlePlot = figure;
        loglog( vectorVectorSizeAxis, sizeSpectra( spectraIndex, : ), 'k-', 'LineWidth', optionLineWidth );
        xlim( [ 1 vectorMatrixSizeAxis( length( vectorMatrixSizeAxis ) ) ] );
        AddPlotLabels( switchUseTitles, optionDistributionTitles{ spectraIndex }, labelVolumeAxis, 'log_{10} v(i)' );
        PrintToFile( handlePlot, optionOutputDirectory, optionCurrentDataSet, optionImageWidth, optionImageHeight, optionDistributionPlotFiles{ spectraIndex }, optionOutputFileFormat );
        if optionCloseAfterPlotting == 1
            close;
        end
        
        spectraIndex = 3;
        handlePlot = figure;
        loglog( vectorVectorSizeAxis, sizeSpectra( spectraIndex, : ), 'k-', 'LineWidth', optionLineWidth );
        xlim( [ 1 vectorMatrixSizeAxis( length( vectorMatrixSizeAxis ) ) ] );
        AddPlotLabels( switchUseTitles, optionDistributionTitles{ spectraIndex }, labelVolumeAxis, labelYAxis );
        PrintToFile( handlePlot, optionOutputDirectory, optionCurrentDataSet, optionImageWidth, optionImageHeight, optionDistributionPlotFiles{ spectraIndex }, optionOutputFileFormat );
        if optionCloseAfterPlotting == 1
            close;
        end
    end
end

warning( 'on', 'all' );