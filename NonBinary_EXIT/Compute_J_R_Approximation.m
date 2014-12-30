function [Poly_J_R, Poly_J_R_Minus, IFirst] = Compute_J_R_Approximation(SNR_dB, Mapping, filename)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Remark: The function accepts an optional parameter filename.
% This parameter is useful for debugging.  You may want to change 
% the way the function computes its approximation, for given data.
% While you 'play' with the approximation options, you can avoid
% tedious computation of data by specifying a filename where
% the data will be stored.  'filename', if given, is assumed to
% be a '.mat' file.  The first time the function is called, the data
% is computed and stored to the file.  In subsequent calls, the data 
% is retrieved and computation is avoided
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%--------------------------------------------------------------------
% Constants 
%--------------------------------------------------------------------
q = length(Mapping);
CountSamples = 50000;
dx = 0.01;

%------------------------------------------------------------------
% Load data
%------------------------------------------------------------------
sigma = [];  II = [];
if (exist('filename','var'))          % if the variable exists
    if (exist(filename,'file'))        % if the file exists
        load(filename, 'sigma', 'II'); % load data
    end;
end;

%--------------------------------------------------------------------
% Generate data
%--------------------------------------------------------------------
if (isempty(sigma) || isempty(II))
    sigma = 0:dx:7;
    
%    flogid = fopen('Log.txt','w');
%    fprintf(flogid, ['SNR_dB = ', num2str(SNR_dB), ';\n']);
%    fprintf(flogid, ['Mapping_1 = ', mat2str(Mapping), ';\n']);
    
    disp('Before InitialSamplesVariable');
    InitialSamplesVariable = InitialSamples(SNR_dB, Mapping, q, CountSamples);
    disp('Before AfterSamples');
    
    for i = 1:length(sigma)
        SumLeftboundSamples = GaussianSamples(sigma(i), q, CountSamples);
        II(i) = Expected_log(InitialSamplesVariable + SumLeftboundSamples);
        
        message = sprintf('i = %d, sigma = %f, I = %f', i, sigma(i), II(i));
%        fprintf(flogid, message);
        disp(message);
    end;
    
%    fclose(flogid);
    
    % Save data
    if (exist('filename','var'))          % if the variable exists
       save(filename, 'sigma', 'II');      % save data
    end;
end;

%--------------------------------------------------------------------------
% Approximate J_R and J_R_Minus
%--------------------------------------------------------------------------
Poly_J_R = polyfit(sigma, II, 50);
IFirst = II(1);

% Approximate J_R_minus()
Poly_J_R_Minus = polyfit(II, sigma, 50);

%--------------------------------------------------------------------------
% Store for future use
%--------------------------------------------------------------------------
Store_CND_and_JR_Data_To_File(SNR_dB, Mapping, IFirst, Poly_J_R, Poly_J_R_Minus, []);

%--------------------------------------------------------------------------
% Plot (for debugging)
%--------------------------------------------------------------------------
% figure(1);
% plot(sigma, II, 'b');   hold on;
% I = polyval(Poly_J_R, sigma);
% %I = Calc_J_R(sigma, SNR_dB, Mapping);
% plot(sigma, I, 'k--');   hold on;
% axis([0 7 0 1])
% 
% % Plot------------------------------
% figure(2);
% plot(II, sigma, 'b');   hold on;
% sample_I = 0:0.01:1;
% 
% indices = find(sample_I < IFirst);
% sig(indices) = 0;
% indices = find(sample_I >= IFirst);
% sig(indices) = polyval(Poly_J_R_Minus, sample_I(indices)); hold on;
% 
% %sig = Calc_J_R_Minus(sample_I, SNR_dB, Mapping);
% 
% plot(sample_I, sig, 'k--');
% axis([0 1 0 7])
