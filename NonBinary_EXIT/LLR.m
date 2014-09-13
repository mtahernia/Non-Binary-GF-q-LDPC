function l = LLR(p)

q = size(p, 1);

lnp0 = myln(p(1, :));

l = zeros(q-1, size(p,2));          % initialize array size
for i = 1:q-1
    l(i, :) = lnp0 - myln(p(i + 1, :));
end;