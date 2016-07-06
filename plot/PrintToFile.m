function PrintToFile( handle, path, dataSet, imageWidth, imageHeight, fileName, fileFormat )

set( handle,'PaperUnits','centimeters', 'PaperSize', [ imageWidth imageHeight ], 'PaperPosition', [ 0 0 imageWidth imageHeight ] );
filePath = [ path dataSet dataSet( 1:end - 1 ) '_plots/' ];

if exist( filePath, 'dir' ) ~= 7
   mkdir( filePath ); 
end

filePath = [ filePath fileName ];

if strcmpi( fileFormat, 'eps' ) == 1
    print( handle, filePath, '-depsc', '-loose'  );
elseif strcmpi( fileFormat, 'png' ) == 1
    print( handle, filePath, '-dpng', '-r300' );
end
