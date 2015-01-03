function IE = Calc_VND_Empirical(ldeg, lwgt, SNR_dB, Quant, IA)

IE = zeros(size(IA));
for i = 1:length(ldeg)
    IE = IE + lwgt(i) * Calc_J_R(sqrt( (ldeg(i) - 1) * Calc_J_Minus(IA).^2 ), SNR_dB, Quant);
end;
