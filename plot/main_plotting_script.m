tic
clear

%% User Defined Parameters
optionCurrentDataSet            = '2020-08-10_17-11-21';
optionOutputDirectory           = '/home/philju/Development/In Progress/EATSM/EATSM/output/0.2/';

optionOutputVariablesFile      = 'Variables';
optionFileExtension             = '.csv';

optionPrintPlotsToFile          = 1; % yes = 1, no = anything else
optionOutputFileFormat          = 'png'; % EPS or PNG
optionPlotImageWidth            = 12; % cm
optionPlotImageHeight           = 10; % cm

optionMissingValue              = -9999;
optionTruncateTimeAt            = 0; % Index not value
optionResampleTimeTo            = 0; % Value not index

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