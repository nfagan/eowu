function p = apidir()

%   APIDIR -- Get the absolute path to the eowu api directory.
%
%     OUT:
%       - `p` (char)

p = eowu.util.get_outer_directory( fileparts(which('eowu.apidir')), 1 );

end