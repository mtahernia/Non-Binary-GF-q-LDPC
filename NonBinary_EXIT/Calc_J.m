function I = Calc_J(sigma)

global Pol_J 

I = ones(size(sigma));

indices = find(sigma < 7);
I(indices) = polyval(Pol_J, sigma(indices));
