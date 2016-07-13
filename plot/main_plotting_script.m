tic

clear

%% User Defined Parameters

optionCurrentDataSet            = '2016-07-12_16-40-00';
optionOutputDirectory           = '/home/philju/Dropbox/Development/EATSM/output/0.1/';

switchOnlyCountLevels           = 0;
switchQuickView                 = 1;
switchUseTitles                 = 1;
switchReverseColourMap          = 0;

optionMissingValue              = -9999;
optionTrophicThreshold          = 0.75;
optionMinimumHeterotrophVolume  = 10000;
optionLineWidth                 = 3;
optionFontSize                  = 10; % 10 = normal, 16 = presentation/publication
optionOutputParametersFile      = 'OutputControlParameters';
optionFileExtension             = '.csv';

optionImageWidth                = 16;
optionImageHeight               = 12;
optionColourMapName             = 'jet'; % 'jet', 'gray', 'bone', ...
optionOutputFileFormat          = 'png'; % EPS or PNG
optionCloseAfterPlotting        = 1; % 1 is 'yes', anything else is 'no'.

optionTruncateTimeAt            = 0;
optionResampleTimeTo            = 133;

if strcmp( optionOutputDirectory( end ), '/' ) == 0
    optionOutputDirectory = [ optionOutputDirectory '/' ];
end

if strcmp( optionCurrentDataSet( end ), '/' ) == 0
   optionCurrentDataSet = [ optionCurrentDataSet '/' ]; 
end

if exist( [ optionOutputDirectory optionCurrentDataSet ], 'dir' ) == 7
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    load_meta_data                   %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    plot_data                       %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %plot_matrices                   %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %plot_size_spectra               %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %plot_trophic_data               %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %plot_vectors                    %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
else
    disp( 'ERROR> Input directories do not exist.' );
end

toc