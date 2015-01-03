function Samples = InitialSamples(SNR_dB, Quant, q, CountSamples)

SNR = 10^(SNR_dB/10);
Energy = sum(Quant.^2) / length(Quant);
sigma = sqrt(Energy / SNR);

% Generate matrix for handling v
for i = 1:q-1
    for v = 0:q-1
        Mat(v + 1, i) =  Quant(v + 1) - Quant(GFqAdd(v, i, q) + 1);
    end;
end;

% Randomly select values v between 0 and q-1;
Sample_v = floor(rand(CountSamples, 1) * q);
MatAll = Mat(Sample_v + 1, :);

% Make noise
noise = sigma * randn(CountSamples, 1);

% Create samples
for i = 1:q-1
    Samples(:, i) = noise .* MatAll(:, i) / (sigma^2)+ (1/(2*sigma^2))*MatAll(:,i).^2;
end;

Samples = Samples';                 % Make standard

SaveSamples = Samples;
% Random permute
labels = floor(rand(1,CountSamples)*(q-1)) + 1;
for i = 1:q-1
    indices = gfmul(i*ones(size(labels)), labels, q);
    Samples(i, :) = SaveSamples(sub2ind(size(Samples), indices, 1:CountSamples));
end;