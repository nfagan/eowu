local state = {}

state.Name = 'end_trial'
state.Duration = 0

function state.Entry()
  local script = eowu.script()
  local state = script:State('end_trial')

  script:Variable('frames'):Commit()

  state:Next('new_trial')
  script:Commit()
end

return state