q = [11 13 17 19 23 29 31];

T_i_30 = [0.1428571429 0.1506849315 0.1842105263 0.2571428571 0.3209876543 0.4712041885 0.4709677419];
T_i_16 = [0.2777777778 0.3412698413 0.4416666667 0.4751131222 0.5933609959 0.7094017094 0.7674418605];

bar(q,[T_i_30;T_i_16]',1)
legend('R=3','R=1.8')

xlabel('Alphabet Size (q)')
ylabel('Iteration Time (s)')