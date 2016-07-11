%% Plot trophic datums

% Trophic line plots

optionTrophicTitles{ 1 } = [ optionCurrentDataSet( 1:end - 1 )  ' Frequencies of trophic levels over time' ];
optionTrophicTitles{ 2 } = [ optionCurrentDataSet( 1:end - 1 )  ' Volumes of trophic levels over time' ];
optionTrophicTitles{ 3 } = [ optionCurrentDataSet( 1:end - 1 )  ' Average age of trophic levels over time' ];

optionTrophicPlotFiles{ 1 } = 'line_trophic_frequencies';
optionTrophicPlotFiles{ 2 } = 'line_trophic_volumes';
optionTrophicPlotFiles{ 3 } = 'line_trophic_ages';

optionlabelYAxis{ 1 } = 'log_{10}(Individual Frequency)';
optionlabelYAxis{ 2 } = 'Volume';
optionlabelYAxis{ 3 } = 'Age';

trophicLevelNames{ 1 } = 'Unclassified';
trophicLevelNames{ 2 } = 'Primary (Erroneous)';
trophicLevelNames{ 3 } = 'Secondary';
trophicLevelNames{ 4 } = 'Tertiary';
trophicLevelNames{ 5 } = 'Quaternary';
trophicLevelNames{ 6 } = 'Quinary';
trophicLevelNames{ 7 } = 'Senary';
trophicLevelNames{ 8 } = 'Septenary';
trophicLevelNames{ 9 } = 'Octonary';
trophicLevelNames{ 10 } = 'Nonary';
trophicLevelNames{ 11 } = 'Denary';

datumNameTrophicFrequencies = CellTrophicDatumNames{ 1 };
datumNameTrophicVolumes = CellTrophicDatumNames{ 2 };
datumNameTrophicAges = CellTrophicDatumNames{ 3 };

load( strcat( [ optionOutputDirectory optionCurrentDataSet ], datumNameTrophicFrequencies, optionFileExtension ) );
load( strcat( [ optionOutputDirectory optionCurrentDataSet ], datumNameTrophicVolumes, optionFileExtension ) );
load( strcat( [ optionOutputDirectory optionCurrentDataSet ], datumNameTrophicAges, optionFileExtension ) );

datumTrophicFrequencies = eval( datumNameTrophicFrequencies );
datumTrophicVolumes = eval( datumNameTrophicVolumes );
datumTrophicAges = eval( datumNameTrophicAges );

vectorTrophicMatrixSize = size( eval( datumNameTrophicFrequencies ) );

if optionTruncateTimeAt > 0 && optionTruncateTimeAt < vectorTrophicMatrixSize( 2 )
    datumTrophicFrequencies = datumTrophicFrequencies( :, 1:optionTruncateTimeAt );
    datumTrophicVolumes = datumTrophicVolumes( :, 1:optionTruncateTimeAt );
    datumTrophicAges = datumTrophicAges( :, 1:optionTruncateTimeAt );
    vectorTrophicMatrixSize( 2 ) = optionTruncateTimeAt;
end

if optionResampleTimeTo > 0 && optionResampleTimeTo < vectorTrophicMatrixSize( 2 )
    datumTrophicFrequencies = ResampleMatrix( datumTrophicFrequencies, optionResampleTimeTo );
    datumTrophicVolumes = ResampleMatrix( datumTrophicVolumes, optionResampleTimeTo );
    datumTrophicAges = ResampleMatrix( datumTrophicAges, optionResampleTimeTo );
    vectorTrophicMatrixSize( 2 ) = optionResampleTimeTo;
end

activeTrophicLevelCount = 0;

trophicIndices = zeros( 1, vectorTrophicMatrixSize( 1 ) );

% Start counting at secondary producers - MATLAB indexes from 1
for trophicLevelIndex = 3:vectorTrophicMatrixSize( 1 )
    if sum( datumTrophicFrequencies( trophicLevelIndex, : ) ) > 0
        activeTrophicLevelCount = activeTrophicLevelCount + 1;
        trophicIndices( activeTrophicLevelCount ) = trophicLevelIndex;
    end
end

cellTrophicLevelNames = cell( 1, activeTrophicLevelCount );

trophicMatrices = zeros( length( CellTrophicDatumNames ), activeTrophicLevelCount,  vectorTrophicMatrixSize( 2 ) );

for activeTrophicLevelIndex = 1:length( trophicIndices )
    if trophicIndices( activeTrophicLevelIndex ) ~= 0
        
        cellTrophicLevelNames{ activeTrophicLevelIndex } = trophicLevelNames{ trophicIndices( activeTrophicLevelIndex ) };
        
        trophicMatrices( 1, activeTrophicLevelIndex, : ) = datumTrophicFrequencies( trophicIndices( activeTrophicLevelIndex ), : );
        trophicMatrices( 2, activeTrophicLevelIndex, : ) = datumTrophicVolumes( trophicIndices( activeTrophicLevelIndex ), : );
        trophicMatrices( 3, activeTrophicLevelIndex, : ) = datumTrophicAges( trophicIndices( activeTrophicLevelIndex ), : );
    end
end

if switchOnlyCountLevels ~= 1
    numberOfPlots = length( CellTrophicDatumNames );
    
    if switchQuickView == 1
        numberOfPlots = 1;
    end
    
    for trophicIndex = 1:numberOfPlots
        
        handlePlot = figure;
        
        if strcmp( optionColourMapName, 'jet' )
            set( handlePlot, 'DefaultAxesColorOrder', jet( activeTrophicLevelCount ), 'DefaultAxesLineStyleOrder', '-' );
        else
            set( 0, 'DefaultAxesColorOrder', [ 0 0 0 ], 'DefaultAxesLineStyleOrder', '-|--|-.|:' );
        end
        
        trophicMatrix = squeeze( trophicMatrices( trophicIndex, :, : ) );
        
        if trophicIndex == 1
            trophicMatrix = LogMatrix( trophicMatrix, optionMissingValue );
        end
        
        set( gca,'FontSize', optionFontSize );
        plot( axisAbstractTime, trophicMatrix, 'LineWidth', optionLineWidth );
        legend( cellTrophicLevelNames, 'Location', 'NorthEast' );
        
        xlim( [ min( axisAbstractTime ) max( axisAbstractTime )  ] );
        
        AddPlotLabels( switchUseTitles, optionTrophicTitles{ trophicIndex }, labelTimeAxis, optionlabelYAxis{ trophicIndex } );
        PrintToFile( handlePlot, optionOutputDirectory, optionCurrentDataSet, optionImageWidth, optionImageHeight, optionTrophicPlotFiles{ trophicIndex }, optionOutputFileFormat );
        if optionCloseAfterPlotting == 1
            close;
        end
    end
end

%% Count trophic levels

datumName = CellVectorDatumNames{ 2 };
load( strcat( [ optionOutputDirectory optionCurrentDataSet ], datumName, optionFileExtension ) );
heterotrophVolume = eval( datumName );

numberOfTrophicLevels = 0;

% If the system exited as a result of reaching minimum volume
if heterotrophVolume( length( heterotrophVolume ) ) > optionMinimumHeterotrophVolume
    
    indexToCountFrom = 1;
    
    while( datumTrophicFrequencies( indexToCountFrom, 1 ) > 0 )
        indexToCountFrom = indexToCountFrom + 1;
    end
    
    indexToCountFrom = indexToCountFrom + indexToCountFrom;
    
    trophicFrequencies = squeeze( trophicMatrices( 1, :, : ) );
    
    maximumTimesPresent = vectorTrophicMatrixSize( 2 ) - indexToCountFrom;
    
    indexToCountFrom = indexToCountFrom + 1;
    
    foundBrokenLevel = 0;
    
    if indexToCountFrom < vectorTrophicMatrixSize( 2 )
        
        for trophicIndex = 1:activeTrophicLevelCount
            
            timesPresent = 0;
            
            for timeIndex = indexToCountFrom:vectorTrophicMatrixSize( 2 )
                if trophicFrequencies( trophicIndex, timeIndex ) > 0
                    timesPresent = timesPresent + 1;
                end
            end
            
            fractionPresent = timesPresent / maximumTimesPresent;
            
            if fractionPresent >= optionTrophicThreshold
                numberOfTrophicLevels = numberOfTrophicLevels + 1;
                foundBrokenLevel = 0;
            elseif foundBrokenLevel == 0
                numberOfTrophicLevels = numberOfTrophicLevels + fractionPresent;
                foundBrokenLevel = 1;
            end
        end
    end
end

disp( [ 'Number of trophic levels for dataset "' optionCurrentDataSet( 1:end - 1 ) '" is ' num2str( numberOfTrophicLevels ) '.' ] );