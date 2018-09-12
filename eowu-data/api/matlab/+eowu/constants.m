function c = constants()

%   CONSTANTS -- Get type identifiers.
%
%     See also eowu.read
%
%     OUT:
%       - `c` (struct)

c = struct();

c.mask_double =           bitshift( uint32(1), uint32(1) );
c.mask_bool =             bitshift( uint32(1), uint32(2) );
c.mask_char =             bitshift( uint32(1), uint32(3) );
c.mask_array =            bitshift( uint32(1), uint32(4) );
c.mask_ndarray =          bitshift( uint32(1), uint32(5) );
c.mask_aggregate =        bitshift( uint32(1), uint32(6) );
c.mask_heterogeneous =    bitshift( uint32(1), uint32(7) );
c.unknown =               bitshift( uint32(1), uint32(8) );

c.type_double =         uint32(1);
c.type_bool =           uint32(2);
c.type_char =           uint32(3);
c.type_heterogeneous =  uint32(4);

end