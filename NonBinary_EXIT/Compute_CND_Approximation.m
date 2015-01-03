function Poly_CND = Compute_CND_Approximation(rho_max, SNR_dB, Mapping, filename)
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

%------------------------------------------------------------------
% Constants
%------------------------------------------------------------------
q = length(Mapping);
dx = 0.01;

%------------------------------------------------------------------
% Load data
%------------------------------------------------------------------
IA = [];  IE = [];
if (exist('filename','var'))          % if the variable exists
    if (exist(filename,'file'))        % if the file exists
        load(filename, 'IA', 'IE'); % load data
    end;
end;

%------------------------------------------------------------------
% Data needs to be created
%------------------------------------------------------------------
if (isempty(IA) || isempty(IE))
    % Params
    CountSamples = 10000;
    
    % Loop------------------------------
    IA = 0:dx:1;
    sigma = Calc_J_R_Minus(IA, SNR_dB, Mapping);
    
    LastZeroIndex = max(find(sigma <= 0));
    indices = find(sigma > 0);
    indices = [LastZeroIndex, indices];
    
    sigma = sigma(indices);
    IA = IA(indices);
    
%     flogid = fopen('Log.txt','a');
%     fprintf(flogid, ['SNR_dB = ', num2str(SNR_dB), ';\n']);
%     fprintf(flogid, ['Mapping_1 = ', mat2str(Mapping), ';\n']);
    
    for k = 1:length(sigma)
        
        if (k == 1) disp('Before InitialSamples');  end;
        InitialMessages = InitialSamples(SNR_dB, Mapping, q, CountSamples);
        if (k == 1) disp('After InitialSamples');   end;
        
        % Evaluate Leftbound 
        LLRSamples = GaussianSamples(sigma(k), q, CountSamples);
        LLRSamples = LLRSamples + InitialMessages;
        IA(k) = Expected_log(LLRSamples);            % Improve IA(k) to be more exact (not really necessary)
        
        % First loop samples
        ProbSamples = unLLR(LLRSamples);
        NewSamples = ProbSamples;
        
        for i = 1:(rho_max-1)
            
            if (i > 1)
                LLRSamples = GaussianSamples(sigma(k), q, CountSamples);
                LLRSamples = LLRSamples + InitialMessages(:,randperm(size(InitialMessages,2)));
                NewSamples = unLLR(LLRSamples);
                ProbSamples = GFqConv(ProbSamples, NewSamples);
            end;
            
            % Record results
            LLRSamples = LLR(ProbSamples);
            IE(k, i) = Expected_log(LLRSamples);
            
            message = sprintf('sigma = %s; IA(%d) = %s; IE(%d, %d) = %s; %% %s', num2str(sigma(k)), k, num2str(IA(k)), k, i, num2str(IE(k, i)), datestr(now));
%            fprintf(flogid, '%s\n\r', message);
            disp(message);
        end;    
    end;
    
%    fclose(flogid);
    
    % Save data
    if (exist('filename','var'))          % if the variable exists
       save(filename, 'IA', 'IE');      % save data
    end;
end;

%------------------------------------------------------------------
% Approximation
%------------------------------------------------------------------
%figure(1);
for j = 1:(rho_max - 1)
    IAFirst = min(IA);
    IEFirst =  IE(1, j);

    % Combat Gibbs phenomenon
    CurIA = [0:dx:(IAFirst-dx),IA];
    indices = find(CurIA >= IAFirst);
    CurIE(indices) = IE(:, j)';
    indices = find(CurIA < IAFirst);
    CurIE(indices) = CurIA(indices) * (IEFirst / IAFirst);

    % Approximate
    Poly_CND(j, :) = polyfit(CurIA, CurIE, 20);
    
%   plot(CurIA, CurIE, 'r');  hold on;
end;

%axis([0,1,0,1]);

%------------------------------------------------------------------
% Save the approximation
%------------------------------------------------------------------
Store_CND_and_JR_Data_To_File(SNR_dB, Mapping, [], [], [], Poly_CND);
