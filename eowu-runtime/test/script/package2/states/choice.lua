state.Name = 'choice'
state.Duration = 1000
state.Variables = {
  errors = {
    broke_fixation = false,
  },
  events = {
    
  }
  outcome = ''
}

function state.Entry()
  local script = eowu.script()
  local state = script:State('choice')
  local targ1 = script:Target('1')
  local targ2 = script:Target('2')

  local set = script:TargetSet({targ1, targ2})

  set:Entry(function(targ, time)
    
  end)

  set:Exit(function(targ, time)
  end)

  set:Reentry(function(targ, time)
  end)

  set:Ellapsed(function(targ, time)
  end)
end

function state.Loop()
end

function state.Exit()
end

local function default_render()
  --
end

function state.Render = {
  default = default_render
}