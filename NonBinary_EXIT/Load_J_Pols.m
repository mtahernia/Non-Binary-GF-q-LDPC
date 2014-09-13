function [Pol_J, Pol_J_Minus] = Load_J_Pols(q)

COL_Q = 1;
COL_POL_J = 2;
COL_POL_J_MINUS = 3;
if (exist('JPols.mat', 'file') ~= 0)
    load('JPols.mat');
end;

if (exist('JPols') == 0)
    JPols = {};
end;

CountRows = size(JPols, 1);
CurrRow = -1;
for row = 1:CountRows
    if (JPols{row, COL_Q} == q)
        CurrRow = row;
        break;
    end;
end;

if (CurrRow == -1)      % if not found
    CurrRow = CountRows + 1;
    JPols{CurrRow, COL_Q} = q;
    JPols{CurrRow, COL_POL_J} = [];
    JPols{CurrRow, COL_POL_J_MINUS} = [];
end;

Pol_J = JPols{CurrRow, COL_POL_J};
Pol_J_Minus = JPols{CurrRow, COL_POL_J_MINUS};
