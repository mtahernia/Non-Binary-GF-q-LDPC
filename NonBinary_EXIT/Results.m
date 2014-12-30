clc;clear;close all

% ----------------------------------------------------------------------------
% Symbol Rate = 0.575233 Bit Rate = 2.84982 Iterations = 5000
% Sum Lambda = 1 Sum Rhos = 1 Block length = 50000 Channel = G
% Seed = 1417140547
% SNR(dB) = 18.6 SNR = 72.4436 Noise Sigma = 0.11749
% Capacity at SNR (symbols per channel use) = 0.625587
% Capacity at SNR (bits per channel use) = 3.09928
% Minimum SNR for rate (dB) = 17.0732 (absolute value) = 50.9709
% ----------------------------------------------------------------------------
q = 31;
Des_SNR = 18.5;
Min_SNR = 17.0732;
SNR = [17 17.5 18 18.1 18.5 18.6 18.7 ];
SER = [0.6382 0.6026 0.509  0.49 0.06 0.0000400016 1e-7 ];

semilogy(SNR,SER,'k-*','linewidth',2);grid on
line([Min_SNR,Min_SNR],[1e-10,1],'linewidth',2,'color','red')
xlabel('SNR(dB)');ylabel('Symbol Error Rate')
xlim([7 20])
ylim([1e-8,1])
% ----------------------------------------------------------------------------
% Symbol Rate = 0.32031 Bit Rate = 1.58688 Iterations = 5000
% Sum Lambda = 1 Sum Rhos = 1 Block length = 50000 Channel = G
% Seed = 1417142951
% SNR(dB) = 11 SNR = 12.5893 Noise Sigma = 0.281838
% Capacity at SNR (symbols per channel use) = 0.37992
% Capacity at SNR (bits per channel use) = 1.8822
% Minimum SNR for rate (dB) = 9.04388 (absolute value) = 8.02395
% ----------------------------------------------------------------------------
q = 31;
Des_SNR = 10.0;
Min_SNR = 9.04388;
SNR = [9.05 9.5 10 10.5 11];
SER = [0.728 0.693 0.6508 0.2114 1e-7];

hold on
semilogy(SNR,SER,'k-o','linewidth',2);grid on;
 line([Min_SNR,Min_SNR],[1e-10,1],'linewidth',2,'color','r')


% ----------------------------------------------------------------------------
% Symbol Rate = 0.372041 Bit Rate = 1.5804 Iterations = 5000
% Sum Lambda = 1 Sum Rhos = 1 Block length = 50000 Channel = G
% Seed = 1417149697
% SNR(dB) = 9 SNR = 7.94328 Noise Sigma = 0.354813
% Capacity at SNR (symbols per channel use) = 0.372041
% Capacity at SNR (bits per channel use) = 1.5804
% Minimum SNR for rate (dB) = 9.00001 (absolute value) = 7.9433
% ----------------------------------------------------------------------------

q = 19;
Des_SNR = 10.0;
Min_SNR = 9.00001;
SNR = [9.0 9.5 10 10.5];
SER = [ 0.6633010641 0.622349788 0.0406072486 1e-7 ];

hold on
semilogy(SNR,SER,'b--o','linewidth',2);grid on;

% ----------------------------------------------------------------------------
% Symbol Rate = 0.661529 Bit Rate = 2.81013 Iterations = 5000
% Sum Lambda = 1 Sum Rhos = 1 Block length = 50000 Channel = G
% Seed = 1417151593
% SNR(dB) = 17 SNR = 50.1187 Noise Sigma = 0.141254
% Capacity at SNR (symbols per channel use) = 0.668065
% Capacity at SNR (bits per channel use) = 2.83789
% Minimum SNR for rate (dB) = 16.8295 (absolute value) = 48.1887
% ----------------------------------------------------------------------------

q = 19;
Des_SNR = 18.5;
Min_SNR = 9.00001;
SNR = [17 17.5 18.0 18.1 18.2];
SER = [0.4604384175 0.4227769111 0.3589623585 0.33 1e-7 ];

hold on
semilogy(SNR,SER,'b-.*','linewidth',2);grid on;

legend('q=31,R=1.586','MinSNR for rate','q=31,R=2.85','MinSNR for rate','q=19,R=1.580','q=19,R=2.810');
