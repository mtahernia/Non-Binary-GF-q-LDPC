function sigma = Calc_J_R_Minus(I, SNR_dB, Mapping)

global Pol_J Pol_J_Minus IFirst Poly_J_R Poly_J_R_Minus Poly_CND

sigma = [];

if (isempty(Poly_J_R_Minus))
    disp('Error, Calc_J_R_Minus: Unhandled Mapping/SNR');
else
    indices = (I < IFirst);
    sigma(indices) = 0;
    
    indices = find(I >= IFirst);
    sigma(indices) = polyval(Poly_J_R_Minus, I(indices));
    
    sigma(sigma > 7) = 7;
    sigma(sigma < 0) = 0;
end;