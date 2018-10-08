local choice = {}

local function get_script_state()
  local script = eowu.script()
  return script, script:State('choice')
end

local function make_target_set(script, state)
  local tset = script:MakeTargetSet('tset1', {'t1', 't2'})
  local selected = state:Variable('selected')
  local entered = state:Variable('entered')

  tset:Duration(500)

  tset:Entry(function(name)
    print('Entered: ' .. name)

    entered:Set(true)
  end)

  tset:Exit(function(name)
    print('Exited: ' .. name)

    selected:Reset()
    entered:Set(false)

    state:Exit('choice_error')
  end)

  tset:Select(function(name)
    print('Selected: ' .. name)

    local tset1 = script:TargetSet('tset1')
    tset1:Reset()

    selected:Set(name)
    state:Exit('choice_success')
  end)

  tset:Begin()
end

local function make_timeout(script, state, duration)
  local to1 = script:MakeTimeout('to1', duration, function()
    local tset1 = script:TargetSet('tset1')
    local selected = state:Variable('selected')
    local entered = state:Variable('entered')
    
    if not entered:Get() then
      tset1:Reset()
      state:Exit('choice_error')
    end
  end)
end

local function entry()
  local script, state = get_script_state()
  local entered = state:Variable('entered')
  local selected = state:Variable('selected')

  entered:Reset()
  selected:Reset()

  make_timeout(script, state, 2000)
  make_target_set(script, state)

  script:Render('default')

  print('Entered: ' .. state.name)
end

local function exit()
  local script = eowu.script()
  script:TargetSet('tset1'):Reset()
end

local function render_default()
  local script = eowu.script()
  local state = script:State('choice')
  local s1 = script:Stimulus('s1')
  local s2 = script:Stimulus('s2')
  local t1 = script:Target('t1')
  local t2 = script:Target('t2')

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