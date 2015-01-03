function r = Myfzero(f, min_x, max_x, tol)

persistent level

if (isempty(level))
   level = 1;
else
   level = level + 1;
end;

sgn_f_min = sign(f(min_x));
sgn_f_max = sign(f(max_x));

if (sgn_f_min == sgn_f_max)			% error
   
%    f_min_x = f(min_x);
%    f_max_x = f(max_x);
%    
%    if (abs(f_min_x) < abs(f_max_x))
%       r = min_x;
%       distance = abs(f_min_x);
%    else
%       r = max_x;
%       distance = abs(f_max_x);
%    end;
%    
%    if (distance > 0.01)
% %      disp('Signs of min and max not different at Myfzero - returning closest to zero');
% %      disp(sprintf('level = %d, min and max are: f(%g) = %g, f(%g) = %g', level, min_x, f(min_x), max_x, f(max_x)));
%    end;

   r = nan;
   level = level - 1;
   return
end;


while ((max_x - min_x) > tol)
   mid_x = (min_x + max_x) / 2;
   if (f(mid_x)*sgn_f_min < 0)
      max_x = mid_x;
   else
      min_x = mid_x;
   end;
   
%   if (level == 1)
%      disp(mid_x);
%   end;
end;

level = level - 1;

if (abs(f(mid_x)) == inf)
    r = nan;
else
    r = mid_x;
end;
   
   
   