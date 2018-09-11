function res = read(file)

constants = eowu.constants();

s = dir( file );
eof = s.bytes;

fid = fopen( file , 'r' );

try
  res = read_impl( fid, constants, eof );
catch err
  fclose( fid );
  
  throw( err );
end

fclose( fid );

end

function chunks = read_impl(fid, constants, eof)

chunks = {};

while ( ftell(fid) ~= eof )
  chunks{end+1} = read_chunk( fid, constants, {} );
end

end

function [chunk, is_aggregate] = read_chunk(fid, constants, assign_to)

id = fread( fid, 1, 'uint32=>uint32' );

is_array =      bitand( id, constants.mask_array ) ~= 0;
is_ndarray =    bitand( id, constants.mask_ndarray ) ~= 0;
is_aggregate =  bitand( id, constants.mask_aggregate ) ~= 0;

if ( is_aggregate )
  if ( is_ndarray )
    chunk = read_nested_aggregate( fid, constants );
  else
    chunk = read_aggregate( fid, constants );
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

function chunk = read_nested_aggregate( fid, constants )

len = fread( fid, 1, 'uint64=>uint64' );

chunk = struct();

for i = 1:len
  v = read_chunk( fid, constants );
  
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

function chunk = read_aggregate( fid, constants )

fieldname = read_char_array( fid );

if ( isvarname(fieldname) )
  chunk = struct();
  chunk.(fieldname) = read_chunk( fid, constants );
else
  warning( ['The fieldname "%s" is not a valid Matlab variable name;' ...
    , ' this data aggregate will be loaded as a containers.Map object,' ...
    , ' instead of a struct.'], fieldname );
  
  chunk = containers.Map();
  chunk(fieldname) = read_chunk( fid, constants );
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

v = fread( fid, 1, 'uint8' );

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
