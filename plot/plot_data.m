%% Create Meta Data
labelVolumeAxis = 'Equivalent Spherical Volume';
labelTimeAxis = 'Time Steps';

%% Reserved Variable Names
searchTermApproxVolume = 'approxvolume';
searchTermCouplings = 'couplings';
searchTermMidPoint = 'midpoint';
searchTermPopulation = 'population';
searchTermTimeAxis = 'time';
searchTermTrophic = 'trophic';
searchTermValues = 'values';
searchTermVolume = 'volume';

%% Trophic Level Names
trophicLevelNames{ 1 } = 'Secondary';
trophicLevelNames{ 2 } = 'Tertiary';
trophicLevelNames{ 3 } = 'Quaternary';
trophicLevelNames{ 4 } = 'Quinary';
trophicLevelNames{ 5 } = 'Senary';
trophicLevelNames{ 6 } = 'Septenary';
trophicLevelNames{ 7 } = 'Octonary';
trophicLevelNames{ 8 } = 'Nonary';
trophicLevelNames{ 9 } = 'Denary';

%% Load Meta Data
OutputParameters = ReadTable( [ optionOutputDirectory optionCurrentDataSet optionOutputParametersFile optionFileExtension ], ',' );


%% Load Data
numberOfDatums = length( OutputParameters( :, 1 ) );

volumeMatrixIndex = 3;

for datumIndex = 1:numberOfDatums
    dataSetName = OutputParameters{ datumIndex, 1 };
    dataInputFile = [ optionOutputDirectory optionCurrentDataSet dataSetName optionFileExtension ];
    
    if exist( dataInputFile, 'file' ) == 2
        %% Load data
        load( dataInputFile, '-ascii' );
        dataSet = eval( dataSetName );
        %% Truncation and resampling
        if isempty( strfind( lower( dataSetName ), searchTermValues ) ) % Data is not size class vectors
            if optionTruncateTimeAt > 0 && optionTruncateTimeAt < length( dataSet )
                if prod( size( dataSet ) ) == length( dataSet ) % Data is 1-dimensional
                    dataSet = dataSet( 1:optionTruncateTimeAt );
                else                                            % Data is n-dimensional
                    dataSet = dataSet( :, 1:optionTruncateTimeAt );
                end
            end
            if optionResampleTimeTo > 0
                resamplingMethod = OutputParameters{ datumIndex, 4 };
                if strcmpi( resamplingMethod, 'cumulative' ) == 1
                    dataSet = ResampleCumulativeMatrix( dataSet, optionResampleTimeTo );
                elseif strcmpi( resamplingMethod, 'standard' ) == 1
                    dataSet = ResampleMatrix( dataSet, optionResampleTimeTo );
                end
            end
            if ~isempty( strfind( lower( dataSetName ), searchTermTimeAxis ) ) % Is time axis
                AxisTimeSteps = dataSet;
                AxisTimeStepsExtended = ExtendVector( AxisTimeSteps );
                maximumTime = max( AxisTimeSteps );
                volumeMatrix = zeros( length( AxisTimeSteps ), volumeMatrixIndex );
            else                                                               % Is data for plotting
                %% Plotting
                plotType = OutputParameters{ datumIndex, 2 };
                dataLabel = OutputParameters{ datumIndex, 3 };
                
                handle = figure;
                
                if strcmp( plotType, 'vector' ) == 1
                    %% Line plots
                    plot( AxisTimeSteps, dataSet );
                    ylabel( dataLabel );
                    
                    if ~isempty( strfind( lower( dataSetName ), searchTermVolume ) ) && isempty( strfind( lower( dataSetName ), searchTermApproxVolume ) )
                        % Data collection for volume area plot
                        volumeMatrix( :, volumeMatrixIndex ) = dataSet;
                        volumeMatrixIndex = volumeMatrixIndex - 1;
                    end
                    
                elseif strcmp( plotType, 'matrix' ) == 1
                    dataSet( dataSet == optionMissingValue ) = NaN;
                    
                    if isempty( strfind( lower( dataSetName ), searchTermCouplings ) )
                        dataSet( dataSet == 0 ) = NaN;
                    end
                    if ~isempty( strfind( lower( dataLabel ), 'log' ) )
                        dataSet = log10( dataSet );
                    end
                    
                    if strfind( lower( dataSetName ), searchTermTrophic ) == 1
                        %% Trophic plots
                        dataSet( all( isnan( dataSet ), 2 ), : ) = []; % Remove rows where all data are missing
                        sizeDataSet = size( dataSet );
                        numberOfTrophicLevels = sizeDataSet( 1 ) - 1;
                        dataSet = dataSet( 2:sizeDataSet( 1 ), : );
                        plot( AxisTimeSteps, dataSet' ), shading flat;
                        ylabel( dataLabel );
                        legend( trophicLevelNames( 1:numberOfTrophicLevels ) );
                    else
                        %% Size class plots
                        dataSet = PadMatrix( dataSet ); % Extend matrix for pcolor plot

                        pcolor( AxisTimeStepsExtended, AxisSizeClassBoundaryValues, dataSet ), shading flat;
                        
                        set( gca, 'YScale', 'log' );
                        c = colorbar;
                        
                        minVal = min( min( dataSet ) );
                        maxVal = max( max( dataSet ) );
                        
                        if ~isempty( strfind( lower( dataSetName ), searchTermCouplings ) )
                            caxis( [ 0, traitResolution ] )
                        elseif minVal < maxVal
                            caxis( [ minVal, maxVal ] )
                        end
                        ylabel( c, dataLabel );
                        ylabel( labelVolumeAxis );
                    end
                end
                xlim( [ 0 maximumTime ] );
                title( dataSetName );
                xlabel( labelTimeAxis );
                if optionPrintPlotsToFile == 1
                    printPlotToFile( handle, [ optionPlotImageWidth optionPlotImageHeight ], [ optionOutputDirectory optionCurrentDataSet dataSetName ], optionOutputFileFormat );
                end
            end
            eval( [ dataSetName ' = dataSet;' ] ); % Necessary to overwrite original data.
        elseif ~isempty( strfind( lower( dataSetName ), searchTermMidPoint ) ) % Data is mid-point values vector
            traitResolution = length( AxisSizeClassMidPointValues );
        end
    end
end

%% Additional plots

% Stock Volumes
if volumeMatrixIndex == 0
    dataSetName = 'StockVolumes';
    handle = figure;
    area( AxisTimeSteps, volumeMatrix );
    legend( 'Nutrient', 'Autotrophs', 'Heterotrophs');
    xlim( [ 0 maximumTime ] );
    ylim( [ 0 max( sum( volumeMatrix, 2 ) ) ] );
    title( dataSetName );
    xlabel( labelTimeAxis );
    ylabel( 'Volume' );
    if optionPrintPlotsToFile == 1
        printPlotToFile( handle, [ optionPlotImageWidth optionPlotImageHeight ], [ optionOutputDirectory optionCurrentDataSet dataSetName ], optionOutputFileFormat );
    end
end