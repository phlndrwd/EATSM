tic

clear

%% User Defined Parameters

optionCurrentDataSet            = '2016-07-06_15-39-13';
optionOutputDirectory           = '/home/philju/Dropbox/Development/EATSM/output/1.0/Development/';

switchOnlyCountLevels           = 0;
switchQuickView                 = 0;
switchUseTitles                 = 1;
switchReverseColourMap          = 0;

optionTrophicThreshold          = 0.75;
optionMinimumHeterotrophVolume  = 10000;
optionLineWidth                 = 3;
optionFontSize                  = 10; % 10 = normal, 16 = presentation/publication
optionFileExtension             = '.dat';

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
    plot_matrices                   %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    plot_size_spectra               %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    plot_trophic_data               %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    plot_vectors                    %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
else
    disp( 'ERROR> Input directories do not exist.' );
end

toc