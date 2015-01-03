close all
clear all

%-------------------------------------------------------------------------------
%
% Initial constants
%
% Mapping, SNR_dB = As obvious.
% lambda_degs, lambda_wts, rho_degs, rho_wts = The initial code which will
%   be used in the design process (see ISIT paper for details). lambda_degs
%   and rho_degs should contain all the degrees that you want the 
%   linear-programming design process to consider in its design.  Since
%   design process begins by designing lambda, it is not necessary to
%   initialize lambda_wts.
%
%-------------------------------------------------------------------------------
%Mapping = [ -1.7096 -1.473 -1.2896 -1.1362 -1.0022 -0.88161 -0.77061 -0.66697 -0.569 -0.47523 -0.38474 -0.29689 -0.21075 -0.12592 -0.041887 0.041887 0.12592 0.21075 0.29689 0.38474 0.47523 0.569 0.66697 0.77061 0.88161 1.0022 1.1362 1.2896 1.473 1.7096 2.0701];    % 32 nonuniform
Mapping = linspace (-10,10,7);
Mapping = Mapping/sqrt(sum(Mapping'.^2));
sum(Mapping'.^2)
SNR_dB = 8.0;
lambda_degs = 2:80;
rho_degs = 3:20; 
rho_wts = zeros(size(rho_degs)); rho_wts(rho_degs == 7) = 1;

%-------------------------------------------------------------------------------
% Initialization
%-------------------------------------------------------------------------------
global Pol_J Pol_J_Minus IFirst Poly_J_R Poly_J_R_Minus Poly_CND
q = length(Mapping);

% if (floor(log2(q)) ~= log2(q))
%     disp('This design software does not handle alphabet sizes that are not powers of 2.');
%     return;
% end;
    
[Pol_J, Pol_J_Minus] = Load_J_Pols(q);
[IFirst, Poly_J_R, Poly_J_R_Minus, Poly_CND] = Load_CND_and_JR_Data_From_File(SNR_dB, Mapping);

if (isempty(Pol_J) || isempty(Pol_J_Minus) || ...
    isempty(IFirst) || isempty(Poly_J_R) || isempty(Poly_J_R_Minus) || isempty(Poly_CND))
    Answer = input(sprintf('Polynomial approximation of J, J_R or I_CND does not exist for requested SNR, mapping or alphabet size (q).  Create? (Y/N) ', q), 's');
    if (Answer ~= 'Y')
        return;
    end;
    
    if (isempty(Pol_J) || isempty(Pol_J_Minus))
        [Pol_J, Pol_J_Minus] = Compute_J_Approximation(q);
    end;
    
    if (isempty(IFirst) || isempty(Poly_J_R) || isempty(Poly_J_R_Minus))
        [Poly_J_R, Poly_J_R_Minus, IFirst] = Compute_J_R_Approximation(SNR_dB, Mapping);
    end;
    
    rho_max = max(rho_degs);
    if (isempty(Poly_CND))
        Poly_CND = Compute_CND_Approximation(rho_max, SNR_dB, Mapping);
    end;
end;

rho_max = max(rho_degs);
MaxAllowedRightDeg = size(Poly_CND,1) + 1;
if (MaxAllowedRightDeg < rho_max)
    Answer = input(sprintf('Current polynomial approximation of I_CND does not include the right-degree requested.  Create? (Y/N) ', q), 's');
    if (Answer ~= 'Y')
        return;
    end;

    Poly_CND = Compute_CND_Approximation(rho_max, SNR_dB, Mapping);
end;

%-------------------------------------------------------------------------------
%
% Design 
%
% Parameters that can be set in the design:
% DesignIterations = The number of linear programming iterations where
%       rho is fixed and lambda is optimized.
% IA = The grid along which optimization of Lambda is performed.
% IE = The grid along which optimization of Rho is performed.
% Lambdas_gap = At short block lengths, it is useful, instead of requiring
%   I_VND > I_CND^{-1}, to require, in part of the range, I_VND >
%   I_CND^{-1} + gap, where gap > 0.  That is, impose a stricter requirement.
% Rhos_gap = The same with Rho.
%
%-------------------------------------------------------------------------------

DesignIterations = 3;
IA = 0.01:0.01:0.9;   
Lambdas_gap = zeros(size(IA));
Lambdas_gap(IA < 0.5) = 0.005;
Lambdas_gap((0.5 <= IA) & (IA < 0.6)) = 0.004;

IE = (IFirst + 0.04):0.01:0.9;   
Rhos_gap = zeros(size(IE));
Rhos_gap(IE < 0.95) = 0.005;

for i = 1:DesignIterations
    lambda_wts = LambdaLinProgDesign(lambda_degs, rho_degs, rho_wts, SNR_dB, Mapping, Lambdas_gap, IA);
    if (i == DesignIterations)           % Don't optimize rhos on last iteration
        break;
    end;
    rho_wts = RhoLinProgDesign(lambda_degs, lambda_wts, rho_degs, SNR_dB, Mapping, Rhos_gap, IE);
end;

%-------------------------------------------------------------------------------
% Plot EXIT 
%-------------------------------------------------------------------------------
dx = 0.01;
IA = 0:dx:1;

IVND = Calc_VND_Empirical(lambda_degs, lambda_wts, SNR_dB, Mapping, IA);
plot(IA, IVND, 'k');   hold on;

IA = 0:dx:1;

ICND_Min = Calc_CND_Minus_Empirical(rho_degs, rho_wts, SNR_dB, Mapping, IA);
plot(IA, ICND_Min, 'k:');   hold on;

axis([0,1,0,1]);
legend('VND curve', 'CND curve');

%-------------------------------------------------------------------------------
% Statistics
%-------------------------------------------------------------------------------

ldegs = lambda_degs(lambda_wts > 2e-6);
lwts  = lambda_wts(lambda_wts > 2e-6);
rdegs = rho_degs(rho_wts > 2e-5);
rwts  = rho_wts(rho_wts > 2e-5);
rwts = rwts / sum(rwts); 
lwts = lwts / sum(lwts);
TestRate = 1 - sum(rwts./rdegs) / sum(lwts./ldegs);
disp(['Rate (symbols) = ', num2str(TestRate), ', rate (bits)  = ', num2str(TestRate * log2(q), 7)]);
disp(['lambda_degs = ', mat2str(ldegs), '; lambda_wts = ', mat2str(lwts, 8), ';']);
disp(['rho_degs = ', mat2str(rdegs), '; rho_wts = ', mat2str(rwts, 8), ';']);
disp('%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%')
disp(' ')

disp(['mapping(' num2str(q) ') = ' mat2str(Mapping,7) ';'])
disp(' ')
for i = 1:numel(ldegs)
    disp(['lambda_' num2str(ldegs(i)) ' = ' num2str(lwts(i), 7) ]);
end
for i = 1:numel(rdegs)
    disp(['rho_' num2str(rdegs(i)) ' = ' num2str(rwts(i), 7) ]);
end