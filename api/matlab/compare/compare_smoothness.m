function compare_smoothness()

[W, win_size] = Screen( 'OpenWindow', 1, [0, 0, 0] );
n_stim = 10;
rects = get_rects( n_stim, 100, win_size );

stop_key = KbName( 'escape' );

cstate = 0;

while ( true )
  
  %   fixation
  if ( cstate == 0 )    
    draw_circles( W, rects, [0, 0, 255] );
    ts = Screen( 'Flip', W );
  end
  
  [key_pressed, ~, key_code] = KbCheck();

  if ( key_pressed )
    if ( key_code(stop_key) ) , break; end
  end  
end

Screen( 'Close', W );

end

function draw_circles(W, rects, colors)
Screen('FillOval', W, colors, rects );
end

function z = get_rects(N, sz, win_size)

z = zeros( 4, N );

for i = 1:N
  x = rand() * win_size(3);
  y = rand() * win_size(4);
  
  r1 = x - (sz/2);
  r2 = x + (sz/2);
  r3 = y - (sz/2);
  r4 = y + (sz/2);
  
  z(:, i) = [ r1, r3, r2, r4 ];
end

end