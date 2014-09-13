function IA = Calc_CND_Minus_Empirical(rho_degs, rho_wts, SNR_dB, Quant, IE)

for i = 1:length(IE)
    f = inline(sprintf('Calc_CND_Empirical(%s, %s, %f, %s, x) - %f', mat2str(rho_degs), mat2str(rho_wts), SNR_dB, mat2str(Quant), IE(i)));
    IA(i) = Myfzero(f, 0, 1, 0.0001);
end;
