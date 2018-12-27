local choice = {}

local function make_target_set(state)
  local tset = eowu.MakeTargetSet('tset1', {'t1', 't2'})
  local selected = state:Variable('selected')
  local entered = state:Variable('entered')

  local t0 = eowu.Elapsed()

  tset:Duration(500)

  tset:Entry(function(name)
    print('Entered: ' .. name)

    entered:Set(true)

    t0 = eowu.Elapsed()
  end)

  tset:Exit(function(name)
    print('Exited: ' .. name)

    selected:Reset()
    entered:Set(false)

    state:Exit('choice_error')
  end)

  tset:Select(function(name)
    print('Selected: ' .. name)

    local tset1 = eowu.TargetSet('tset1')
    tset1:Reset()

    print('Took: ' .. (eowu.Elapsed() - t0) * 1e3 .. ' ms.')

    selected:Set(name)
    state:Exit('choice_success')
  end)

  tset:Begin()
end

local function make_timeout(state, duration)
  local to1 = eowu.MakeTimeout('to1', duration, function()
    local tset1 = eowu.TargetSet('tset1')
    local selected = state:Variable('selected')
    local entered = state:Variable('entered')
    
    if not entered:Get() then
      tset1:Reset()
      state:Exit('choice_error')
    end
  end)
end

local function entry()
  local state = eowu.CurrentState()
  local entered = state:Variable('entered')
  local selected = state:Variable('selected')

  entered:Reset()
  selected:Reset()

  make_timeout(state, 2000)
  make_target_set(state)

  eowu.Render('default')

  print('Entered: ' .. state.name)
end

local function exit()
  eowu.TargetSet('tset1'):Reset()
end

local function render_default()
  local state = eowu.State('choice')
  local s1 = eowu.Stimulus('s1')
  local s2 = eowu.Stimulus('s2')
  local t1 = eowu.Target('t1')
  local t2 = eowu.Target('t2')

  s1:Draw()
  s2:Draw()

  t1:Draw()
  t2:Draw()
end

choice.Name = 'choice'
choice.Duration = -1
choice.First = true
choice.Entry = entry
choice.Exit = exit

choice.Render = {
  default = render_default
}

choice.Variables = {
  selected = '',
  entered = false
}

return choice