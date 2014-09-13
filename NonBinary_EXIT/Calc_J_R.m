function I = Calc_J_R(sigma, SNR_dB, Mapping)

global Pol_J Pol_J_Minus IFirst Poly_J_R Poly_J_R_Minus Poly_CND

sigma_last = 7;
I = [];

if (isempty(Poly_J_R))
    disp('Error, Calc_J_R: Unhandled Mapping/SNR');
else
    I = polyval(Poly_J_R, sigma);
    I(find(sigma > sigma_last)) = 1;
    I(find(I < 0)) = 0;
end;
