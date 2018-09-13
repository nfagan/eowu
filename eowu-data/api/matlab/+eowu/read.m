function res = read(file)

%   READ -- Read binary file contents.
%
%     C = eowu.read( file ) reads the contents of `file` as a cell array
%     `C`. 
%
%     `C` is a cell array of struct or containers.Map objects. If a
%     map-like object in `file` contains a key that is not a valid Matlab
%     variable name, it will be returned as a containers.Map object;
%     otherwise it will be a struct.
%
%     For example, if the Lua table: { a = 10, b = true }  is saved, it 
%     will be loaded as a struct with fields 'a' and 'b'. 
%
%     But if the Lua table: { a = 10, ['1'] = true } is saved, it will be
%     loaded as a containers.Map object with keys 'a' and '1'.
%
%     See also containers.Map
%
%     IN:
%       - `file` (char)
%     OUT:
%       - `res` (cell)

constants = eowu.constants();

assert( ischar(file), 'Specify the file as a character vector.', file );
assert( exist(file, 'file') == 2, 'The file "%s" does not exist.', file );

s = dir( file );
assert( numel(s) == 1, 'Too many files matched.' );
eof = s.bytes;

fid = fopen( file , 'r' );
state = configure_warn_state();

err = 0;

try
  res = read_impl( fid, constants, eof );
catch err
  % 
end

cleanup( fid, state );

if ( isa(err, 'MException') )
  throw( err );
end

end

function s = configure_warn_state()

s = warning();
warning( 'off', 'backtrace' );

end

function cleanup(fid, state)

fclose( fid );
warning( state );

end

function chunks = read_impl(fid, constants, eof)

chunks = {};
warnings = containers.Map();

while ( ftell(fid) ~= eof )
  chunks{end+1} = read_chunk( fid, constants, warnings );
  
  print_progress( ftell(fid), eof );
end

end

function print_progress(pos, eof)

persistent last_n;

perc = pos / eof * 100;

if ( isempty(last_n) )
  fprintf( '\n\n' );
else
  fprintf( repmat('\b', 1, last_n) );
end

str = sprintf( 'Loading ... %d%%', round(perc) );
fprintf( '%s\n', str );

last_n = numel( str ) + 1;

end

function [chunk, is_aggregate] = read_chunk(fid, constants, warnings)

id = fread( fid, 1, 'uint32=>uint32' );

is_array =      bitand( id, constants.mask_array ) ~= 0;
is_ndarray =    bitand( id, constants.mask_ndarray ) ~= 0;
is_aggregate =  bitand( id, constants.mask_aggregate ) ~= 0;

if ( is_aggregate )
  if ( is_ndarray )
    chunk = read_nested_aggregate( fid, constants, warnings );
  else
    chunk = read_aggregate( fid, constants, warnings );
  end
  
  return;
end

t = typeid( id, constants );

switch ( t )
  case constants.type_char
    if ( is_array )
      chunk = read_char_array( fid );
    elseif ( is_ndarray )
      chunk = read_cellstr_array( fid );
    end
  case constants.type_double
    if ( is_array )
      chunk = read_double_array( fid );
    else
      chunk = read_double( fid );
    end    
  case constants.type_bool
    assert( ~is_array, 'Not yet implemented' );
    chunk = read_boolean( fid );
  otherwise
    error( 'Not yet implemented' );
end

end

function chunk = read_nested_aggregate( fid, constants, warnings )

len = fread( fid, 1, 'uint64=>uint64' );

chunk = struct();

for i = 1:len
  v = read_chunk( fid, constants, warnings );
  
  if ( isstruct(v) )
    f = fieldnames( v );
    ff = f{1};
      
    if ( isstruct(chunk) )
      chunk.(ff) = v.(ff);
    else
      chunk(ff) = v.(ff);      
    end
    
  else
    f = keys( v );
    ff = f{1};
    
    if ( isstruct(chunk) )
      tmp = containers.Map();
      chunk = struct_to_map( tmp, chunk );
    end
    
    chunk(ff) = v(ff);
  end
end

end

function map = struct_to_map(map, s)

f = fieldnames( s );

for i = 1:numel(f)
  map(f{i}) = s.(f{i});
end

end

function chunk = read_aggregate( fid, constants, warnings )

fieldname = read_char_array( fid );

if ( isvarname(fieldname) )
  chunk = struct();
  chunk.(fieldname) = read_chunk( fid, constants, warnings );
else
  if ( ~isKey(warnings, fieldname) )
    warning( ['The fieldname "%s" is not a valid Matlab variable name;' ...
      , ' this data aggregate will be loaded as a containers.Map object,' ...
      , ' instead of a struct.'], fieldname );
    warnings(fieldname) = true;
  end
  
  chunk = containers.Map();
  chunk(fieldname) = read_chunk( fid, constants, warnings );
end

end

function vals = read_cellstr_array(fid)

len = fread( fid, 1, 'uint64=>uint64' );

vals = cell( len, 1 );

for i = 1:len
  fread( fid, 1, 'uint32=>uint32' );  % ignore type
  vals{i} = read_char_array( fid );
end

end

function val = read_char_array(fid)

len = fread( fid, 1, 'uint64=>uint64' );
val = fread( fid, len, 'char=>char' )';

end

function val = read_double_array(fid)

len = fread( fid, 1, 'uint64=>uint64' );
val = fread( fid, len, 'double=>double' );

end

function v = read_double(fid)

v = fread( fid, 1, 'double' );

end

function v = read_boolean(fid)

v = logical( fread(fid, 1, 'uint8') );

end

function t = typeid(id, constants)

is_char = bitand( id, constants.mask_char ) ~= 0;
is_bool = bitand( id, constants.mask_bool ) ~= 0;
is_double = bitand( id, constants.mask_double ) ~= 0;

assert( xor(xor(is_char, is_bool), is_double), 'Multiple types matched.' );

if ( is_char )
  t = constants.type_char;
  return;
end

if ( is_bool )
  t = constants.type_bool;
  return;
end

if ( is_double )
  t = constants.type_double;
  return;
end

error( 'Unrecognized type.' );

end
