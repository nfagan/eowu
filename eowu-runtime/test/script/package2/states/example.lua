local state = {}

state.ID = 'fixation'
state.Duration = 1000
state.First = true

state.Variables = {
  start = true,
  events = {
    fixation_offset = 0.0
  }
}

function state.Entry()
  print('Entered fixation!')

  local state = eowu_script():State('fixation')

  state:Next('fixation')
  eowu_script():Render('fixation')

  local variables = state.Variables
  local events = variables:Field('events')

  events:Set({
    first = 10,
    second = 11
  })

  events:Commit()
end

function state.Exit()
  print(eowu_script():State('fixation'):Ellapsed())
end

function state.Loop()
end

local function render_default()
  local vars = eowu_script():State('fixation'):Variables()
end

state.Render = {
  fixation = render_default
}

return state