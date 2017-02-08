tic
clear

%% User Defined Parameters
optionCurrentDataSet            = '2017-02-08_15-47-34';
optionOutputDirectory           = '/home/philju/Dropbox/Development/EATSM/output/0.1/';

optionOutputParametersFile      = 'Variables';
optionFileExtension             = '.csv';

optionPrintPlotsToFile          = 0; % yes = 1, no = anything else
optionOutputFileFormat          = 'png'; % EPS or PNG
optionPlotImageWidth            = 12; % cm
optionPlotImageHeight           = 10; % cm

optionMissingValue              = -9999;
optionTruncateTimeAt            = 0; % Index, not value
optionResampleTimeTo            = 0; % Index, not value

%% Input Formatting
if strcmp( optionOutputDirectory( end ), '/' ) == 0
    optionOutputDirectory = [ optionOutputDirectory '/' ];
end

if strcmp( optionCurrentDataSet( end ), '/' ) == 0
    optionCurrentDataSet = [ optionCurrentDataSet '/' ];
end

%% Plot Script Execution
if exist( [ optionOutputDirectory optionCurrentDataSet ], 'dir' ) == 7
    plot_data
else
    disp( [ 'ERROR> Input directory "' optionOutputDirectory optionCurrentDataSet '" does not exist.' ] );
end
toc