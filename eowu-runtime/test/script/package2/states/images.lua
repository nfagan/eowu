local state = {}

state.ID = 'images'
state.Duration = 1000

function state.Entry()
  print('Entered images!')
  eowu_script():State('images'):Next('fixation')
  eowu_script():Render('images')
end

function state.Exit()
end

function state.Loop()
end

local function render_default()
end

state.Render = {
  images = render_default
}

return state