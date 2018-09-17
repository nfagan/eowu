function p = get_outer_directory(p, n)

%   GET_OUTER_DIRECTORY -- Get containing directory.
%
%     p2 = ... get_outer_directory( p1 ) is the same as `fileparts( p1 )`
%
%     p2 = ... get_outer_directory( p1, N ) is the N-th containing folder
%     of `p1`; by default, N = 1.
%
%     IN:
%       - `p` (char)
%       - `n` (double) |SCALAR|

if ( nargin < 2 ), n = 1; end

for i = 1:n
  p = fileparts( p );
end

end