function p = unLLR(l)

q = size(l, 1) + 1;
CountSamples = size(l,2);

p = zeros(q, CountSamples);
if (q == 2)
    p(1, :) = 1./(1 + exp(-l)) ;
else
    p(1, :) = 1./(1 + sum(exp(-l)));
end;

for i = 1:q-1
    p(i + 1, :) = p(1,:) .* exp(-l(i, :));
end;