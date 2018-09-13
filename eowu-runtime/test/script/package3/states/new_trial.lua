local state = {}

state.First = true
state.Name = 'new_trial'
state.Duration = 0

state.Variables = {
  trial = 0
}

function state.Entry()
  local script = eowu_script()
  local state = script:State('new_trial')

  local trial = script:Variable('trial')
  local tn = trial:Get()

  print('Trial: ', tn)

  trial:Uncommit()
  trial:Set(tn + 1)
  trial:Commit()

  script:Variable('frames'):Uncommit()

  if tn < 1000 then state:Next('fixation') end
end

return state