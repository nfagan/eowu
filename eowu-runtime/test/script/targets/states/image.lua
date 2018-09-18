local image = {}

function image.Entry()
  local script = eowu.script()
  local state = script:State('image')
  local targ = script:Target('t1')

  state:Next('fixation')
  script:Render('image')
  targ:Reset()

  print('Entered ' .. state.name)
end

local function render()
  local stim = eowu.script():Stimulus('stim')
  local stim2 = eowu.script():Stimulus('stim2')

  stim:Color({1, 0, 0})
  stim:Draw()   
  stim2:Draw()
end

image.Name = 'image'
image.Duration = 1000
image.Render = {
  image = render
}

return image