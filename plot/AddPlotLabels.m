function AddPlotLabels( useTitle, titleLabel, xAxisLabel, yAxisLabel )

if useTitle == 1
    title( titleLabel, 'Interpreter', 'none' );
end

xlabel( xAxisLabel );
ylabel( yAxisLabel );