function sigma = Calc_J_Minus(I)

global Pol_J_Minus

sigma = zeros(size(I));

indices = find(I > 0);
sigma(indices) = polyval(Pol_J_Minus, I(indices));

