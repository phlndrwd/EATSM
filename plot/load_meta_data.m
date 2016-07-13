%% Create Meta Data

labelVolumeAxis = 'Equivalent Spherical Volume';
labelTimeAxis = 'Time Steps';

%% Load Meta Data
OutputParameters = readtable( [ optionOutputDirectory optionCurrentDataSet optionOutputParametersFile optionFileExtension ] );

%% Load Data
numberOfPlots = zeros( 1, length( OutputParameters{ :, 1 } ) );
for datumIndex = 1:length( OutputParameters{ :, 1 } )
    dataInputFile = [ optionOutputDirectory optionCurrentDataSet OutputParameters{ datumIndex, 1 }{ : } optionFileExtension ];
    if exist( dataInputFile, 'file' ) == 2
        load( dataInputFile );
        numberOfPlots( datumIndex ) = 1;
    end
end

vectorVectorSizeAxis = OutputParameters{ 2, 1 }{ : };
vectorMatrixSizeAxis = OutputParameters{ 3, 1 }{ : };

numberOfPopulations = length( vectorVectorSizeAxis );

% Create an extended abstract time vector for matrix plots.
axisAbstractTime = OutputParameters{ 1, 1 }{ : };
axisAbstractTimeExtended = ExtendVector( axisAbstractTime );

lengthAxisAbstractTime = length( axisAbstractTime );

if optionTruncateTimeAt > 0 && optionTruncateTimeAt < lengthAxisAbstractTime
    axisAbstractTime = axisAbstractTime( 1:optionTruncateTimeAt );
    axisAbstractTimeExtended = axisAbstractTimeExtended( 1:optionTruncateTimeAt + 1 );
end

if optionResampleTimeTo > 0
    axisAbstractTime = ResampleMatrix( axisAbstractTime, optionResampleTimeTo );
    axisAbstractTimeExtended = ResampleMatrix( axisAbstractTimeExtended, optionResampleTimeTo + 1 );
end
