function[ logMatrix ] = LogMatrix( matrix, missingValue )

logMatrix = matrix;

dimensions = size( matrix );

x = dimensions( 2 );
y = dimensions( 1 );

for i = 1:x
    for j = 1:y
        if( matrix( j, i ) == 0 || matrix( j, i ) == missingValue )
            logMatrix( j, i ) = NaN;
        else
            logMatrix( j, i ) = log10( matrix( j, i ) );
        end
    end
end