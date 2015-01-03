function r = GFqMul(x1, x2, q)

persistent last_q BinaryToAlpha AlphaToBinary

if isempty(last_q)
    last_q = -1;
end;

% First time -----------------------------
if (q ~= last_q)
    last_q = q;
    
    if (isprime(q))
       disp('GFMul does not handle prime q');
       return;
    else
        if (floor(log2(q)) ~= log2(q))
            disp('q not prime or power of 2');
            return;
        end;
        
        m = floor(log2(q));
        
        BinaryToAlpha(0 + 1) = -1;          % Handle zero

        for alpha = 0:q-2
            AlphaToBinary(alpha + 1) = bi2de(gftuple(alpha, m, 2));
            BinaryToAlpha(AlphaToBinary(alpha + 1) + 1) = alpha;
        end;
    end;
end;

% Compute -----------------------------
if ((x1 == 0) | (x2 == 0))
    r = 0;
else
    r = AlphaToBinary(mod(BinaryToAlpha(x1 + 1) + BinaryToAlpha(x2 + 1), q-1) + 1);
end;
