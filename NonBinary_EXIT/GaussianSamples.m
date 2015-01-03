function Samples = GaussianSamples(sigma, q, CountSamples)

% Determine covariance matrix and mean
mean = (sigma^2)/2;
mean_vec = ones(q-1, 1) * mean;
C = ones(q-1, q-1) * (sigma^2)/2 + eye(q-1) * (sigma^2)/2;

% Random generation
Samples = (C^(1/2)) * randn(q-1, CountSamples) + mean;

% Test emprical covariance matrix
% for i = 1:q-1
%     for j = 1:q-1
%         E(i,j) = sum((Samples(i, :) - mean) .* (Samples(j, :) - mean)) / CountSamples;
%     end;
% end;
