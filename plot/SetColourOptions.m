function SetColourOptions( optionColourMapName, switchReverseColourMap, optionFontSize, numberOfPopulations, matrixIndex )

if matrixIndex == 8
    labelColourBar = 'Individual Frequency';
elseif matrixIndex < 10 && matrixIndex ~= 9
    labelColourBar = 'log_{10}( Individual Frequency )';
elseif matrixIndex == 9 || matrixIndex < 12
    labelColourBar = 'log_{10}( Volume )';
elseif matrixIndex == 12
    labelColourBar = 'Growth Ratio';
elseif matrixIndex == 13
    labelColourBar = 'Coupled Population Index';
    caxis( [ 0 ( numberOfPopulations - 1 ) ] );
elseif matrixIndex == 14
    labelColourBar = 'log_{10}( Prey Volume Ratio )';
elseif matrixIndex == 15
    labelColourBar = 'Feeding Probability';
elseif matrixIndex == 16
    labelColourBar = 'Trophic Classification';
elseif matrixIndex == 17
    labelColourBar = 'Average Age';
end

colormap( optionColourMapName );

if switchReverseColourMap == 1
    colormap( flipud( colormap ) );
end

c = colorbar;
ylabel( c, labelColourBar, 'fontsize', optionFontSize );
set( c, 'fontsize', optionFontSize );