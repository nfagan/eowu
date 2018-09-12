local state = {}

state.Name = 'end_trial'
state.Duration = 0

function state.Entry()
  local script = eowu_script()
  local state = script:State('end_trial')

  state:Next('new_trial')
  script:Commit()
end

return state