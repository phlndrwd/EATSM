function[ nanMatrix ] = ConvertToNaN( matrix, missingValue )

nanMatrix = matrix;

dimensions = size( matrix );

x = dimensions( 2 );
y = dimensions( 1 );

for i = 1:x
    for j = 1:y
        if( matrix( j, i ) == missingValue )
            nanMatrix( j, i ) = NaN;
        end
    end
end