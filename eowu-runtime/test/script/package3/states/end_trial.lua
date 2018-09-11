local state = {}

state.Name = '1'
state.Duration = 0

function state.Entry()
  local script = eowu_script()
  local state = script:State('1')

  state:Next('new_trial')
  script:Commit()
end

return state