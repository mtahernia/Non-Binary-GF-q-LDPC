function lambda_wts = CompleteLambdas(lambda_degs, lambda_wts, rho_degs, rho_wts, Rate)

% Plan degree distribution -----------------------------
K = lambda_degs(1);
L = lambda_degs(2);
M = lambda_degs(3);

integ_rho = sum(rho_wts./rho_degs);
lambda_wts(2) = (1/(1 - Rate) * integ_rho - (1/K - 1/M) * lambda_wts(1) - 1/M) / (1/L - 1/M);
lambda_wts(3) = 1 - lambda_wts(1) - lambda_wts(2);

