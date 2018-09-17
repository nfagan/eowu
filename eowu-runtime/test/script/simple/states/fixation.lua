local fixation = {}

--  entry function: Called once when first entering the state.

local function entry()
  local script = eowu.script()
  local trial = script:Variable('trial')
  local state = script:State('fixation')
  local tn = trial:Get()

  trial:Set(tn + 1)
  script:Render('fixation')
  state:Next('fixation')

  print('Entered state: ' .. state.name)
end

--  exit function: Called once when exiting the state.

local function exit()
  local script = eowu.script()
  local state = script:State('fixation')
  local trial = script:Variable('trial')
  local tn = trial:Get()
  local t = state:Ellapsed()

  print('Finished trial: ' .. tn .. '; lasted: ' .. t .. ' (s)')
end

--  render function: Called each render-frame.

local function render()
  local script = eowu.script()
  local stim = script:Stimulus('example')
  local tn = script:Variable('trial'):Get()

  local rot = stim.rotation
  rot.z = rot.z + 0.01
  stim.rotation = rot

  if tn % 2 == 0 then 
    --  even number trial
    stim:Color({0.5, 1, 0.75})
  else
    stim:Color({1, 0.5, 0.75})
  end

  stim:Units('normalized')
  stim:Position({0.5, 0.5})
  stim:Size({0.5, 0.5})
  stim:Draw()
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