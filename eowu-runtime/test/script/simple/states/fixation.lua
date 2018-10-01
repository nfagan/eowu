local fixation = {}

--  entry function: Called once when first entering the state.

local function entry()
  local script = eowu.script()
  local trial = script:Variable('trial')
  local frames = script:Variable('frames')
  local state = script:State('fixation')
  local tn = trial:Get()

  trial:Set(tn + 1)
  frames:Reset()
  script:Render('fixation')
  state:Next('fixation')

  print('Entered state: ' .. state.name)
end

--  exit function: Called once when exiting the state.

local function exit()
  local script = eowu.script()
  local state = script:State('fixation')
  local trial = script:Variable('trial')
  local frames = script:Variable('frames')
  local tn = trial:Get()
  local f = frames:Get()
  local t = state:Ellapsed()

  local trial_info = 'Completed trial: ' .. tn
  local time_info = '; time: ' .. t .. ' (s)'
  local frame_info = '; frames: ' .. f

  print(trial_info .. time_info .. frame_info)
end

--  render function: Called each render-frame.

local function render()
  local script = eowu.script()
  local stim = script:Stimulus('example')
  local tn = script:Variable('trial'):Get()
  local frames = script:Variable('frames')
  local kb = script:Keyboard()

  if kb:Down('space') then
    stim:ZRotate(-0.04)
  else
    stim:ZRotate(0.01)
  end

  if tn % 2 == 0 then 
    --  even number trial
    stim:Color({1, 0, 0})
  else
    stim:Color({0, 1, 0})
  end

  stim:Units('normalized')
  stim:Position({0.5, 0.5})
  stim:Size({0.5, 0.5})
  stim:Draw()

  frames:Set(frames:Get() + 1)
end

fixation.First = true       -- begin in this state
fixation.Name = 'fixation'  -- name of the state
fixation.Duration = 2000    -- how long the state will last (ms)
fixation.Entry = entry      -- entry function
fixation.Exit = exit        -- exit function
fixation.Render = {         -- render functions; maps local function 'render' to the string key 'fixation'
  fixation = render         
}

return fixation