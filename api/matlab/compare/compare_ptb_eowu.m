addpath( genpath('~/repositories/cpp/eowu/api/matlab/') );

eowu.util.try_add_ptoolbox();

%%  -- ptb: run

n_trials = 1e3;
state_dur = 200;
do_save = false;

outs = compare_image_fix( n_trials, state_dur );

fname = datestr( now, 'mmddyy__MM__SS' );

if ( do_save )
  save( fullfile(eowu.apidir(), 'compare', 'data', fname), 'outs' );
end

%%  -- eowu: read

% e_datedir = '09-16-2018 13-45-10';
% e_datedir = '09-17-2018 11-04-47';
% e_datedir = '09-17-2018 21-03-25';
e_datedir = '09-17-2018 23-47-53';
% e_datedir = '09-19-2018 11-52-31';
e_datedir = '09-19-2018 18-19-49';
p_fname = '091618__25__42.mat';

ptb_fname = fullfile( eowu.apidir(), 'compare', 'data', p_fname );

file = fullfile( eowu.rootdir(), 'data', e_datedir, 'task.dat' );

s = eowu.read( file );
load( ptb_fname );

%%

res = struct();

%%  -- eowu

frames = cellfun( @(x) x.Data.Variables.frames, s );
onsets = cellfun( @(x) x.Data.States.images.Variables.image_onset.time, s );
exits = cellfun( @(x) x.Data.States.images.Variables.state_time.exit, s );
timeouts = cellfun( @(x) x.Data.States.images.Variables.render_time, s );
state_entry = cellfun( @(x) x.Data.States.images.Timing.Entry, s );

res.eowu.fix = arrayfun( @(x) x.fixation, frames );
res.eowu.img = arrayfun( @(x) x.images, frames );

res.eowu.diffed_img_onsets = diff( onsets ) * 1e3;
res.eowu.diffed_state_entries = diff( state_entry ) * 1e3;

res.eowu.n_trials = numel( frames );

%%
res.ptb.fix = outs.fixation_frames;
res.ptb.img = outs.image_frames;

res.ptb.diffed_img_onsets = diff( outs.image_onsets ) * 1e3;
res.ptb.diffed_state_entries = diff( outs.image_state_times(:, 1) * 1e3 );
res.ptb.n_trials = outs.trial_number;

%%

fs = fieldnames( res );

axs = cell( 1, 4 );

for i = 1:4
  axs{i} = gobjects( size(fs) );
end

for i = 1:numel(fs)
field = fs{i};
  
f = figure(i);
clf( f );

shp = [ 4, 1 ];

ax1 = subplot( shp(1), shp(2), 1 );
hist( ax1, res.(field).diffed_img_onsets, res.(field).n_trials );
med = median( res.(field).diffed_img_onsets );
dev = std( res.(field).diffed_img_onsets );

stat_str = sprintf( 'M=%0.2f ms, D=%0.2f', med, dev );

title( sprintf('%s: inter-image-onset interval (%s)', field, stat_str) );

ax2 = subplot( shp(1), shp(2), 2 );
hist( ax2, res.(field).fix, res.(field).n_trials );
med = median( res.(field).fix );
dev = std( res.(field).fix );

stat_str = sprintf( 'M=%0.2f frames, D=%0.2f', med, dev );
title( sprintf('%s: number of frames drawing fixation (%s)', field, stat_str) );

ax3 = subplot( shp(1), shp(2), 3 );
hist( ax3, res.(field).img, res.(field).n_trials );
med = median( res.(field).img );
dev = std( res.(field).img );
stat_str = sprintf( 'M=%0.2f frames, D=%0.2f', med, dev );
title( sprintf('%s: number of frames drawing images (%s)', field, stat_str) );

ax4 = subplot( shp(1), shp(2), 4 );
hist( ax4, res.(field).diffed_state_entries, res.(field).n_trials );
med = median( res.(field).diffed_state_entries );
dev = std( res.(field).diffed_state_entries );
stat_str = sprintf( 'M=%0.2f ms, D=%0.2f', med, dev );
title( sprintf('%s: inter-state-entry interval (%s)', field, stat_str) );

axs{1}(i) = ax1;
axs{2}(i) = ax2;
axs{3}(i) = ax3;
axs{4}(i) = ax4;

end

for i = 1:numel(axs)
  shared_utils.plot.match_xlims( axs{i} );
  shared_utils.plot.match_ylims( axs{i} );
end
