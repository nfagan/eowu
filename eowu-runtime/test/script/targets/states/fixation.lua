local fixation = {}

local first_entry = true

local function entry()
  local script = eowu.script()
  local state = script:State('fixation')

  local targ = script:Target('t1')
  local targ2 = script:Target('t2')

  if first_entry then 
    script:Render('out_of_bounds') 
    first_entry = false
  end

  targ:Entry(function()
    local script = eowu.script()
    script:Render('in_bounds')
  end)

  targ:Exit(function()
    local script = eowu.script()
    script:Render('out_of_bounds')
  end)

  targ2:Entry(function()
    eowu.script():Render('in_bounds2')
  end)

  targ2:Exit(function()
    eowu.script():Render('out_of_bounds')
  end)

  state:Next('fixation')
end

local function render_default()
  local script = eowu.script()
  local stim = script:Stimulus('stim')
  local stim2 = script:Stimulus('stim2')

  stim:Color({0, 0, 1})
  stim:Rotation({0, 0, 0})

  stim:Draw()
  stim2:Draw()
end

local function render_oob()
  local script = eowu.script()
  local stim = script:Stimulus('stim')
  local stim2 = script:Stimulus('stim2')
  local targ = script:Target('t1')
  local targ2 = script:Target('t2')

  local rot = stim.rotation
  rot.z = rot.z - 0.01
  stim.rotation = rot

  stim2:Position({2/3, 0.5})
  stim2:Color({1, 0, 0})

  stim:Color({1, 1, 1})
  stim:Draw()
  stim2:Draw()
  targ:Draw()
  targ2:Draw()
end

local function render_ib()
  local script = eowu.script()
  local stim = script:Stimulus('stim')
  local stim2 = script:Stimulus('stim2')

  local rot = stim.rotation
  rot.z = rot.z + 0.01
  stim.rotation = rot

  stim:Color({0, 0, 1})
  stim:Draw()

  stim2:Color({1, 1, 0})
  stim2:Draw()

  local pos = stim2.position
  pos.x = pos.x + 0.001
  pos.y = pos.y - 0.001

  stim2.position = pos
end

local function render_ib2()
  local script = eowu.script()
  local stim2 = script:Stimulus('stim2')

  stim2:Color({1, 0, 1})
  stim2:Draw()
end

fixation.First = true
fixation.Name = 'fixation'
fixation.Duration = 2000
fixation.Entry = entry
fixation.Exit = exit
fixation.Render = {
  in_bounds = render_ib,
  in_bounds2 = render_ib2,
  out_of_bounds = render_oob,
  fixation = render_default
}

return fixation