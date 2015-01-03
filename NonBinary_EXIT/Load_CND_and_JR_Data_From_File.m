function [IFirst, Poly_J_R, Poly_J_R_Minus, Poly_CND] = Load_CND_and_JR_Data_From_File(SNR_dB, Mapping)

% Load old data
% Constants
SNR_COL = 1; MAPPING_COL = 2; IFIRST_COL = 3; POLY_J_R_COL = 4; POLY_J_R_MINUS_COL = 5; POLY_CND_COL = 6;
FILENAME = 'CND_and_JR_Pols.mat';

%--------------------------------------------------------------------------------------------------------------------------------------------
% Find ID of quantization and SNR
%--------------------------------------------------------------------------------------------------------------------------------------------
if (exist(FILENAME,'file') == 2)
    load(FILENAME, 'CND_and_JR_Pols');
end;
if (exist('CND_and_JR_Pols','var') == 0)
    CND_and_JR_Pols = {};
end;

CountRows = size(CND_and_JR_Pols,1);
CurrRow = -1;
for row = 1:CountRows
    if ((CND_and_JR_Pols{row, SNR_COL} == SNR_dB) & ...
           (isequal(CND_and_JR_Pols{row, MAPPING_COL}, Mapping)))
           CurrRow = row;
           break;
   end;
end;

%--------------------------------------------------------------------------------------------------------------------------------------------
% If not found, create
%--------------------------------------------------------------------------------------------------------------------------------------------
if (CurrRow == -1)       
    CurrRow = CountRows + 1;
    CND_and_JR_Pols{CurrRow, SNR_COL} = SNR_dB;
    CND_and_JR_Pols{CurrRow, MAPPING_COL} = Mapping;
    CND_and_JR_Pols{CurrRow, IFIRST_COL} = [];
    CND_and_JR_Pols{CurrRow, POLY_J_R_COL} = [];
    CND_and_JR_Pols{CurrRow, POLY_J_R_MINUS_COL} = [];
    CND_and_JR_Pols{CurrRow, POLY_CND_COL} = [];
end;

% Replace data that has changed
IFirst = CND_and_JR_Pols{CurrRow, IFIRST_COL};
Poly_J_R = CND_and_JR_Pols{CurrRow, POLY_J_R_COL};
Poly_J_R_Minus = CND_and_JR_Pols{CurrRow, POLY_J_R_MINUS_COL};
Poly_CND = CND_and_JR_Pols{CurrRow, POLY_CND_COL};
