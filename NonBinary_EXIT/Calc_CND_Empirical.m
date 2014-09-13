function IE = Calc_CND(rho_degs, rho_wts, SNR_dB, Mapping, IA)

global Pol_J Pol_J_Minus IFirst Poly_J_R Poly_J_R_Minus Poly_CND

IE = zeros(size(IA));
indices = find(IA < IFirst);
IE(indices) = -inf;
indices = find(IA >= IFirst);

if (isempty(Poly_CND))
    disp('Error, Calc_CND_Empirical: Unhandled rho/Mapping/SNR');
    IA = [];
else
    for ind = 1:length(rho_degs)
        if ((rho_degs(ind) - 1) <= size(Poly_CND,1))
            Pol_For_Deg = Poly_CND(rho_degs(ind) - 1, :);
            IE(indices) = IE(indices) + rho_wts(ind) * polyval(Pol_For_Deg, IA(indices));
        end;
    end;    
end;

