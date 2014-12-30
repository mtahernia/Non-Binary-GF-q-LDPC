SNR  =[18.2 18.5 19 19.5 20 21 22];
Iter_31 = [NaN 170 47 28 21 13 11];

Iter_19 = [150 45 24 16 12 9 8];

plot(SNR,Iter_31,SNR,Iter_19,'linewidth',2)
xlim([18,22])

xlabel('SNR(dB)')
ylabel('Iterations')
legend('q=31','q=19')