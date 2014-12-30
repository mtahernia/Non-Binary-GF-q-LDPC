function I = Expected_log(Samples)

CountSamples = size(Samples, 2);
q = size(Samples, 1) + 1;

% Determine E log
if q > 2
    I = 1 - (sum(log(1 + sum(exp(-Samples))) / (log(q) * CountSamples)));
else                    % Handle case of q = 2 separately, because of Matlab inconsistency
    I = 1 - (sum(log(1 + exp(-Samples)) / log(q)) / CountSamples);
end;
    