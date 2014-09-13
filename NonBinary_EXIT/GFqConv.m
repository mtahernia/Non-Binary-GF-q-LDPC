function OutSamples = GFqConv(Samples1, Samples2)

CountSamples = size(Samples1, 2);
q = size(Samples1, 1);

OutSamples = zeros(q, CountSamples);
for i = 0:q-1
    for j = 0:q-1
        k = GFqAdd(i, j, q);
        OutSamples(k+1, :) = OutSamples(k+1, :) + Samples1(i+1,:) .* Samples2(j+1,:);
    end;
end;

