tic
clear

%% User Defined Parameters
optionCurrentDataSet            = '2016-08-02_14-55-33';
optionOutputDirectory           = '/home/philju/Dropbox/Development/EATSM/output/0.1/';

optionOutputParametersFile      = 'OutputControlParameters';
optionFileExtension             = '.csv';

optionPrintPlotsToFile          = 0; % yes = 1, no = anything else
optionOutputFileFormat          = 'png'; % EPS or PNG
optionPlotImageWidth            = 12; % cm
optionPlotImageHeight           = 10; % cm

optionMissingValue              = -9999;
optionTruncateTimeAt            = 0;
optionResampleTimeTo            = 0;

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
    disp( 'ERROR> Input directories do not exist.' );
end
toc