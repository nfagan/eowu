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
  state:Next('fixation')

  local trial = state:Variable('trial')
  trial:Uncommit()
  trial:Set(trial:Get() + 1)
  trial:Commit()
end

return state