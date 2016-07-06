function[ extendedMatrix ] = ExtendMatrix( matrix )

vectorMatrixDimensions = size( matrix );

extendedMatrix = matrix;

extendedMatrix( vectorMatrixDimensions( 1 ) + 1, vectorMatrixDimensions( 2 ) + 1 ) = 0;