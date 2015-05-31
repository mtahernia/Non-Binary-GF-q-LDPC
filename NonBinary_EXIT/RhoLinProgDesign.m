function rho_wts = RhoLinProgDesign(lambda_degs, lambda_wts, rho_degs, SNR_dB, Mapping, gap, IE)

if (exist('IE','var') ~= 1)
    IE = 0.01:0.01:0.99;
end

if (exist('gap','var') ~= 1)
      gap = zeros(size(IE));
end

%-----------------------------------------------------------------------------------
% Prepare data
%-----------------------------------------------------------------------------------
% VND EXIT chart 
IE = 0.05:0.05:0.9;
IVND = Calc_VND_Minus_Empirical(lambda_degs, lambda_wts, SNR_dB, Mapping, IE);

% CND EXIT chart 
IA = IE;            % switch axes
for ideg = 1:length(rho_degs)
     ICND(:, ideg) = Calc_CND_Empirical(rho_degs(ideg), 1, SNR_dB, Mapping, IA)';
end;

indices = find(~isnan(sum(ICND,2)') & ~isnan(IVND));          % sum, so that if any element in row is Nan, whole row is disqualified 
ICND = ICND(indices, :);
IVND = IVND(indices) + gap(indices);

%-----------------------------------------------------------------------------------
% Linear programming
%-----------------------------------------------------------------------------------

count_rhos = length(rho_degs);

% Constrain variables to range [0,1]
LB = zeros(1, count_rhos);
UB = ones(1, count_rhos);

% Target function:  minimize \Sigma rho_i/i 
f = 1./rho_degs;

% Equality constraint: sum rhos must equal one
Aeq = ones(1, count_rhos);
beq = 1;

% Inequality constraints:  enforce EXIT requirement:  
%                   sigma rho_i * ICND(IA, i) >= IVND(IA)
% equivalently:     sigma -ICND(IA, i) rho_i <= -IVND(IA);
A = -ICND;
b = -IVND';

% Initial guess: put all weight on minimum rho (low rate but would satisfy requirement)
[M, I] = min(rho_degs);
X0 = zeros(1, count_rhos);
X0(I) = 1;

% Linear programming
x = linprog(f, A, b, Aeq, beq, LB, UB, X0);

% Store results
rho_wts = x';


