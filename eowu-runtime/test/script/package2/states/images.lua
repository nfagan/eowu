local state = {}

state.First = true
state.Name = 'images'
state.Duration = 1000

state.Variables = {
  shown_image = false
}

function state.Entry()
  print('Entered images!')
  local script = eowu_script()

  script:State('images'):Next('fixation')
  script:Render('images')
end

function state.Exit()
end

function state.Loop()
end

local function render_default()
  local script = eowu_script()
  local stim = script:Stimulus('sq')
  stim:Units('normalized')
  stim:Size({0.5, 0.5})
  stim:Color({1, 1, 1})
  stim:Position({0.5, 0.5})
  stim:Draw()
end

state.Render = {
  images = render_default
}

return state