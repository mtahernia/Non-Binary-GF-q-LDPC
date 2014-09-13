function IA = Calc_VND_Minus_Empirical(lambda_degs, lambda_wts, SNR_dB, Quant, IE)

for i = 1:length(IE)
    f = inline(sprintf('Calc_VND_Empirical(%s, %s, %f, %s, x) - %f', mat2str(lambda_degs), mat2str(lambda_wts), SNR_dB, mat2str(Quant), IE(i)));
    if (IE(i) <= 0.01)
        IA(i) = 0;
    else
        IA(i) = Myfzero(f, 0, 1, 0.001);
    end;
end;
