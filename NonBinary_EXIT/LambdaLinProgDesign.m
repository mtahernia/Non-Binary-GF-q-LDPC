function lambda_wts = LambdaLinProgDesign(lambda_degs, rho_degs, rho_wts, SNR_dB, Mapping, gap, IA);

%-----------------------------------------------------------------------------------
% Prepare data
%-----------------------------------------------------------------------------------
if (exist('IA') ~= 1)
    IA = 0.01:0.01:0.99;
end

if (exist('gap') ~= 1)
      gap = zeros(size(IA));
end

for ideg = 1:length(lambda_degs)
    IVND(:, ideg) = Calc_VND_Empirical(lambda_degs(ideg), 1, SNR_dB, Mapping, IA)';
%    plot(IA, IVND(:, ideg), 'b'); hold on;
end;

% CND Minus 
IE = IA;        % switch axes
ICND = Calc_CND_Minus_Empirical(rho_degs, rho_wts, SNR_dB, Mapping, IE);

indices = find(~isnan(ICND));
ICND = ICND(indices);
gap = gap(indices);
IVND = IVND(indices, :);

% Require I_VND > I_CND + gap
ICND = ICND + gap;

%-----------------------------------------------------------------------------------
% Linear programming
%-----------------------------------------------------------------------------------

count_lambdas = length(lambda_degs);

% Constrain variables to range [0,1]
LB = zeros(1, count_lambdas);
UB = ones(1, count_lambdas);

% Target function:  maximize \Sigma lambda_i/i 
% equivalently, minimize Sigma -1/i * lambda_i
f = -1./lambda_degs;

% Equality constraint: sum lambdas must equal one
Aeq = ones(1, count_lambdas);
beq = 1;

% Inequality constraints:  enforce EXIT requirement:  
%                   sigma lambda_i * IVND(IA, i) >= ICND(IA)
% equivalently:     sigma -IVND(IA, i) lambda_i <= -ICND(IA);
A = -IVND;
b = -ICND';

% Initial guess: put all weight on maximum lambda (low rate but would satisfy requirement)
[M, I] = max(lambda_degs);
X0 = zeros(1, count_lambdas);
X0(I) = 1;

% Linear programming
disp('LambdaLinProgDesign')
options = optimoptions(@linprog,'Algorithm','active-set');
x = linprog(f, A, b, Aeq, beq, LB, UB, X0,options);

% Store results
lambda_wts = x';


