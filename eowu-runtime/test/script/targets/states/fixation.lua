local fixation = {}

local first_entry = true
local last_time = nil

local function entry()
  local script = eowu.script()
  local state = script:State('fixation')

  local targ = script:Target('t1')
  local targ2 = script:Target('t2')

  local set1 = script:MakeTargetSet('id1', {'t1', 't2'})
  set1:Duration(100)

  set1:Entry(function(name)
    print('Entered: ' .. name)
    last_time = state:Ellapsed()
  end)

  set1:Select(function(name)
    print('Selected!' .. name)

    current_time = state:Ellapsed()

    if last_time ~= nil then
      print((current_time - last_time)*1000 .. ' (ms)')
    end

    if name == 't1' then
      script:Render('in_bounds')
    elseif name == 't2' then
      script:Render('in_bounds2')
    end
  end)

  set1:Exit(function(name)
    print('Exited!' .. name)
    script:Render('out_of_bounds')
  end)

  set1:Begin()
  -- set1:Reset()

  if first_entry then 
    script:Render('out_of_bounds') 
    first_entry = false
  end

  -- targ:Entry(function()
  --   local script = eowu.script()
  --   script:Render('in_bounds')
  -- end)

  -- targ:Exit(function()
  --   local script = eowu.script()
  --   script:Render('out_of_bounds')
  -- end)

  -- targ2:Entry(function()
  --   eowu.script():Render('in_bounds2')
  -- end)

  -- targ2:Exit(function()
  --   eowu.script():Render('out_of_bounds')
  -- end)

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

local function move(kb, stim)
  local pos = stim.position
  local amt = 0.005

  if kb:Down('left') then pos.x = pos.x - amt end
  if kb:Down('right') then pos.x = pos.x + amt end
  if kb:Down('up') then pos.y = pos.y + amt end
  if kb:Down('down') then pos.y = pos.y - amt end

  stim.position = pos

  if kb:Down('c') then stim:Position({1/3, 0.5}) end
end

local function space_color(kb, stim)
  if kb:Down('space') then
    stim:Color({1, 1, 1})
    return true
  else
    return false
  end
end

local function render_oob()
  local script = eowu.script()
  local stim = script:Stimulus('stim')
  local stim2 = script:Stimulus('stim2')
  local targ = script:Target('t1')
  local targ2 = script:Target('t2')
  local kb = script:Keyboard()

  if not space_color(kb, stim) then
    stim:Color({0.5, 0.5, 1})
  end

  move(kb, stim)

  local rot = stim.rotation
  rot.z = rot.z - 0.01
  stim.rotation = rot

  stim2:Position({2/3, 0.5})
  stim2:Color({1, 0, 0})
  
  stim:Draw()
  stim2:Draw()
  targ:Draw()
  targ2:Draw()
end

local function render_ib()
  local script = eowu.script()
  local stim = script:Stimulus('stim')
  local stim2 = script:Stimulus('stim2')
  local kb = script:Keyboard()

  local rot = stim.rotation
  local pos = stim.position
  rot.z = rot.z + 0.1
  stim.rotation = rot

  move(kb, stim)

  if not space_color(kb, stim) then
    stim:Color({0.5, 0.5, 1})
  end

  stim:Draw()

  stim2:Color({1, 1, 0.5})
  stim2:Draw()

  local pos = stim2.position
  pos.x = pos.x + 0.001
  pos.y = pos.y - 0.001

  stim2.position = pos
end

local function render_ib2()
  local script = eowu.script()
  local stim2 = script:Stimulus('stim2')
  local t2 = script:Target('t2')

  stim2:Color({1, 0, 1})
  stim2:Draw()
  t2:Draw()
end

fixation.First = true
fixation.Name = 'fixation'
fixation.Duration = -1
fixation.Entry = entry
fixation.Exit = exit
fixation.Render = {
  in_bounds = render_ib,
  in_bounds2 = render_ib2,
  out_of_bounds = render_oob,
  fixation = render_default
}

return fixation