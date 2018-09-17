function p = rootdir()

%   ROOTDIR -- Get the absolute path to the eowu repository.
%
%     OUT:
%       - `p` (char)

p = eowu.util.get_outer_directory( fileparts(which('eowu.rootdir')), 3 );

end