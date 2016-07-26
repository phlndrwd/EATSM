%% Create Meta Data

labelVolumeAxis = 'Equivalent Spherical Volume';
labelTimeAxis = 'Time Steps';

%% Load Meta Data
OutputParameters = readtable( [ optionOutputDirectory optionCurrentDataSet optionOutputParametersFile optionFileExtension ] );

%% Load Data
numberOfDatums = length( OutputParameters{ :, 1 } );
loadedDataFiles = zeros( 1, numberOfDatums );

for datumIndex = 1:numberOfDatums
    dataSetName = OutputParameters{ datumIndex, 1 }{ : };
    dataInputFile = [ optionOutputDirectory optionCurrentDataSet dataSetName optionFileExtension ];
    
    if exist( dataInputFile, 'file' ) == 2
        %% Load data
        load( dataInputFile, '-ascii' );
        
        dataSet = eval( dataSetName );
        
        %% Truncate and/or resample
        if optionTruncateTimeAt > 0 && optionTruncateTimeAt < length( dataSet )
            if prod( size( dataSet ) ) == length( dataSet )
                dataSet = dataSet( 1:optionTruncateTimeAt );
            else
                dataSet = dataSet( :, 1:optionTruncateTimeAt );
            end
        end
        
        if optionResampleTimeTo > 0
            if datumIndex == 8 || datumIndex == 9 || datumIndex == 11 || datumIndex == 13 || datumIndex > 14 && datumIndex < 22
                dataSet = ResampleCumulativeMatrix( dataSet, optionResampleTimeTo );
            else
                dataSet = ResampleMatrix( dataSet, optionResampleTimeTo );
            end
        end
        
        if datumIndex == 1
            AxisAbstractTimeExtended = ExtendVector( dataSet );
        end
        
        %% Plot?
        if datumIndex > 0
            plotType = OutputParameters{ datumIndex, 2 }{ : };
            
            figure;
            if strcmp( plotType, 'vector' ) == 1
                plot( dataSet );
            elseif strcmp( plotType, 'matrix' ) == 1
                dataSet( dataSet == optionMissingValue ) = NaN;
                pcolor( dataSet ), shading flat, colorbar;
            end
        end
        
        %% Finish up
        
        eval( [ dataSetName ' = dataSet;' ] ); % Necessary to overwrite original data.
        loadedDataFiles( datumIndex ) = 1;
    end
end

numberOfPopulations = length( AxisAbstractTimeExtended ) - 1;