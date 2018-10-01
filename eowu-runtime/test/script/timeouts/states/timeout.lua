local timeout = {}

timeout.Name = 'timeout'
timeout.Duration = -1
timeout.First = true

function timeout.Entry()
  print('entered timeout!')

  local script = eowu.script()
  local state = script:State('timeout')
  local set = script:MakeTargetSet('1', {'t1'})
  local start_time = script:Ellapsed()

  script:Render('timeout')
  state:Next('timeout')

  set:Duration(1000)

  set:Entry(function(name) script:Render('entered') end)
  set:Exit(function(name) script:Render('timeout') end)

  set:Select(function(name)
    script:Render('selected')

    local to = script:MakeTimeout('1', 1000, function()
      print((script:Ellapsed() - start_time) * 1000 .. ' ms')
      state:Exit()
    end)
  end)

  set:Begin()
end

local function render_entered()
  local script = eowu.script()
  local stim = script:Stimulus('s1')
  local t1 = script:Target('t1')

  stim:Color({0.8, 0, 0})
  stim:Draw()

  t1:Draw()
end

local function render_selected()
  local script = eowu.script()
  local stim = script:Stimulus('s1')
  local t1 = script:Target('t1')
  
  stim:Color({1, 1, 0})
  stim:Move({0.005})
  stim:ZRotate(0.05)
  stim:Draw()

  t1:Draw()
end

local function default_render()
  local script = eowu.script()
  local stim = script:Stimulus('s1')
  local t1 = script:Target('t1')

  stim:Color({1, 0, 0})
  stim:Units('normalized')
  stim:Position({0.5, 0.5})
  stim:Size({0.25, 0.25})
  stim:ZRotation(0)
  stim:Draw()

  t1:Draw()
end

timeout.Render = {
  timeout = default_render,
  selected = render_selected,
  entered = render_entered
}

return timeout