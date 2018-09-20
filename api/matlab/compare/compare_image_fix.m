function outs = compare_image_fix(n_trials, n_stim, state_dur)

Screen( 'Preference', 'SkipSyncTests', 1 );

win_size = 400;
rect_frac = 0.01;

rect_size = win_size * rect_frac;
W = Screen( 'OpenWindow', 0, [0, 0, 0], [0, 0, win_size, win_size] );

stop_key = KbName( 'escape' );

time_stamps = nan( n_trials, 1 );
iter = 1;

main_time = tic();
state_timer = tic();

cstate = 0;
first_entry = true;

img_times = nan( n_trials, 2 );
fix_times = nan( n_trials, 2 );

img_frames = nan( size(img_times, 1), 1 );
fix_frames = nan( size(img_frames) );

TN = 0;

while ( TN <= n_trials )
  
  %   fixation
  if ( cstate == 0 )
    if ( first_entry )
      state_timer = tic();
      TN = TN + 1;
      fix_times(TN, 1) = toc( main_time );
      logged_time = false;
      first_entry = false;
      frame = 0;
      
      disp( TN );
    end
    
    draw_circles( W, n_stim, rect_size, win_size, [0, 0, 255] );
    ts = Screen( 'Flip', W );
    frame = frame + 1;
    
    if ( ~logged_time )
      time_stamps(iter) = ts;
      iter = iter + 1;
      logged_time = true;
    end
    
    if ( toc(state_timer) >= (state_dur/1e3) )
      fix_times(TN, 2) = toc( main_time );
      fix_frames(TN) = frame;
      cstate = 1;
      first_entry = true;
    end
  end
  
  %   images
  if ( cstate == 1 )
    if ( first_entry )
      state_timer = tic();
      img_times(TN, 1) = toc( main_time );
      frame = 0;
      logged_time = false;
      first_entry = false;
    end
    
    draw_circles( W, n_stim, rect_size, win_size, [0, 255, 0] );
    ts = Screen( 'Flip', W );
    
    frame = frame + 1;
    
    if ( toc(state_timer) >= (state_dur/1e3) )
      img_times(TN, 2) = toc( main_time );
      img_frames(TN) = frame;
      cstate = 0;
      first_entry = true;
    end
  end
  
  [key_pressed, ~, key_code] = KbCheck();

  if ( key_pressed )
    if ( key_code(stop_key) ) , break; end
  end  
end

Screen( 'Close', W );

outs.image_state_times = img_times(1:TN-1, :);
outs.fixation_state_times = fix_times(1:TN-1, :);
outs.image_onsets = time_stamps(1:TN-1, :);
outs.image_frames = img_frames(1:TN-1);
outs.fixation_frames = fix_frames(1:TN-1);
outs.trial_number = TN-1;

end

function draw_circles(W, N, rect_size, win_size, colors)
rects = get_rects( N, rect_size, win_size );

Screen('FillOval', W, colors, rects );
end

function colors = get_colors(N)

colors = zeros( 3, N );

for i = 1:N
  r = randi( 255 );
  g = randi( 255 );
  b = randi( 255 );
  colors(:, i) = [r, g, b];
end

end

function z = get_rects(N, sz, win_size)

z = zeros( 4, N );

for i = 1:N
  x = rand() * win_size;
  y = rand() * win_size;
  
  r1 = x - (sz/2);
  r2 = x + (sz/2);
  r3 = y - (sz/2);
  r4 = y + (sz/2);
  
  z(:, i) = [ r1, r3, r2, r4 ];
end

end