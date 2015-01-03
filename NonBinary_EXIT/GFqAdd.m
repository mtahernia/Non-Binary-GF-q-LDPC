function r = GFqAdd(x1, x2, q)

persistent last_q ConversionTable

if isempty(last_q)
    last_q = -1;
end;

% First time -----------------------------
if (q ~= last_q)
    last_q = q;
    
    ConversionTable = zeros(q,q);
    
    if (isprime(q))
        for i = 0:q-1
            for j = 0:q-1
                ConversionTable(i+1,j+1) = mod(i + j, q);
            end;
        end;
    else
        if (floor(log2(q)) ~= log2(q))
            disp('q not prime or power of 2');
            return;
        end;
        
        m = log2(q);
        for i = 0:q-1
            Binary(i+1, :) = de2bi(i,m);
        end;
        
        for i = 0:q-1
             binx1 = Binary(i+1, :);
             for j = i:q-1
                binx2 = Binary(j+1, :);
                binresult = xor(binx1,binx2);
                
                ConversionTable(i+1,j+1) = bi2de(binresult +0); %+0 to overcome Linux Matlab bug
                ConversionTable(j+1,i+1) = ConversionTable(i+1,j+1);
            end;
        end;
    end;
end;

% Compute -----------------------------
r = ConversionTable(x1+1, x2+1);
