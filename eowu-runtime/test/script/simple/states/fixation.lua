local fixation = {}

local function entry()
  local script = eowu.script()
  local trial = script:Variable('trial')
  local state = script:State(fixation.Name)

  trial:Set(trial:Get() + 1)

  script:Render('fixation')

  -- state:Next('fixation')

  print('Entered state: ' .. state.name)
end

local function exit()
  local script = eowu.script()
  local state = script:State(fixation.Name)
  local trial = script:Variable('trial')
  local n = trial:Get()

  print('Finished trial: ' .. n)
  print('Ran for: ' .. state:Ellapsed() * 1000 .. ' ms')
end

local function render()
  local script = eowu.script()
  local stim = script:Stimulus('fix')

  stim:Draw()
end

fixation.Name = 'fixation'
fixation.Duration = 1000
fixation.Entry = entry
fixation.Exit = exit
fixation.Render = {
  fixation = render
}

return fixation