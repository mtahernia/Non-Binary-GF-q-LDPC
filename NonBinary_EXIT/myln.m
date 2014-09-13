function l = myln(p, q)

indices = find(p > 0);
l = -1000000000 * ones(size(p));
l(indices) = log(p(indices));           % natural log(p)