%% Create Meta Data

labelVolumeAxis = 'Equivalent Spherical Volume';
labelTimeAxis = 'Time Steps';

%% Load Meta Data

infile = fopen( [ optionOutputDirectory optionCurrentDataSet 'MetaAxisVectorNames' optionFileExtension ], 'r' );
CellAxisVectorNames = textscan( infile, '%s' );
fclose( infile );
CellAxisVectorNames = CellAxisVectorNames{ : };

infile = fopen( [ optionOutputDirectory optionCurrentDataSet 'MetaTrophicDatumNames' optionFileExtension ], 'r' );
CellTrophicDatumNames = textscan( infile, '%s' );
fclose( infile );
CellTrophicDatumNames = CellTrophicDatumNames{ : };

infile = fopen( [ optionOutputDirectory optionCurrentDataSet 'MetaVectorDatumNames' optionFileExtension ], 'r' );
CellVectorDatumNames = textscan( infile, '%s' );
fclose( infile );
CellVectorDatumNames = CellVectorDatumNames{ : };

infile = fopen( [ optionOutputDirectory optionCurrentDataSet 'MetaMatrixDatumNames' optionFileExtension ], 'r' );
CellMatrixDatumNames = textscan( infile, '%s' );
fclose( infile );
CellMatrixDatumNames = CellMatrixDatumNames{ : };

infile = fopen( [ optionOutputDirectory optionCurrentDataSet 'MetaMatrixEnumIndicies' optionFileExtension ], 'r' );
CellMatrixEnumIndicies = textscan( infile, '%d' );
fclose( infile );
CellMatrixEnumIndicies = CellMatrixEnumIndicies{ : };

infile = fopen( [ optionOutputDirectory optionCurrentDataSet 'MetaVectorEnumIndicies' optionFileExtension ], 'r' );
CellVectorEnumIndicies = textscan( infile, '%d' );
fclose( infile );
CellVectorEnumIndicies = CellVectorEnumIndicies{ : };

%% Load Data
for axisVectorIndex = 1:length( CellAxisVectorNames )
    load( strcat( [ optionOutputDirectory optionCurrentDataSet ], CellAxisVectorNames{ axisVectorIndex }, optionFileExtension ) ); % Abstract time
end

vectorVectorSizeAxis = eval( CellAxisVectorNames{ 2 } );
vectorMatrixSizeAxis = eval( CellAxisVectorNames{ 3 } );

numberOfPopulations = length( vectorVectorSizeAxis );

% Create an extended abstract time vector for matrix plots.
axisAbstractTime = eval( CellAxisVectorNames{ 1 } );
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