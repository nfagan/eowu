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
  chunks{end+1} = read_chunk( fid, constants );
end

end

function chunk = read_chunk(fid, constants)

id = fread( fid, 1, 'uint32=>uint32' );

is_array =      bitand( id, constants.mask_array ) ~= 0;
is_ndarray =    bitand( id, constants.mask_ndarray ) ~= 0;
is_aggregate =  bitand( id, constants.mask_aggregate ) ~= 0;

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
  otherwise
    error( 'Not yet implemented' );
end

if ( is_aggregate )
  next = read_chunk( fid, constants );
  val = struct();
  val.(chunk) = next;
  
  chunk = val;
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
