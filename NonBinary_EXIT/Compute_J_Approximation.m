function [Pol_J, Pol_J_Minus] = Compute_J_Approximation(q, filename)
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
dx = 0.01;
CountSamples = 50000;

%------------------------------------------------------------------
% Load data
%------------------------------------------------------------------
sigma = [];  II = [];
if (exist('filename'))          % if the variable exists
    if (exist(filename))        % if the file exists
        load(filename, 'sigma', 'II'); % load data
    end;
end;

%--------------------------------------------------------------------
% Generate data
%--------------------------------------------------------------------
if (isempty(sigma) | isempty(II))
    sigma = 0:dx:7;
    
    for i = 1:length(sigma)
        II(i) = J_Empirical(sigma(i), q, CountSamples);
        disp(sprintf('i = %d, sigma = %f, I = %f', i, sigma(i), II(i)));
    end;
    
    if (exist('filename'))          % if the variable exists
       save(filename, 'sigma', 'II');      % save data
    end;
    
end;


%--------------------------------------------------------------------
% Approximation
%--------------------------------------------------------------------
% Approximate J()
Pol_J = polyfit(sigma, II, 20);

% Approximate J_minus()
indices = find(sigma  < 6.5);
Pol_J_Minus = polyfit(II(indices), sigma(indices), 20);

%--------------------------------------------------------------------
% Store results
%--------------------------------------------------------------------
Store_J_Pols(q, Pol_J, Pol_J_Minus)

%--------------------------------------------------------------------
% Plot (for debugging) 
%--------------------------------------------------------------------
% figure(1);
% plot(sigma, II, 'k');   hold on;
% I = polyval(Pol_J, sigma);
% plot(sigma, I, 'r--');   hold on;
% axis([0,7,0,1]);
% figure(2);
% plot(II, sigma, 'k');   hold on;
% sample_I = 0:0.01:1;
% sig = polyval(Pol_J_Minus, sample_I); hold on;
% sig = Calc_J_Minus(sample_I);
% plot(sample_I, sig, 'r--');  hold on;
% axis([0,1,0,7]);

