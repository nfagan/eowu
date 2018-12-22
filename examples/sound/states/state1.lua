local state = {}

state.Name = 'state1'
state.First = true
state.Duration = -1

function state.Entry()
  eowu.Render('default')

  print('Press + hold space / enter to play sound; press escape to exit.')
end

local sound_handles = {}

local function default_render()
  local kb = eowu.Keyboard()
  local s1 = eowu.Stimulus('s1')

  s1:Geometry('circ')
  s1:Units('mixed')
  s1:Size({200, 200})
  s1:Position({0.5, 0.5})
  s1:Opacity(1.0)

  if kb:Pressed('space') then
    sound_handles[1] = eowu.Sound('piano'):Play()
  end

  if kb:Pressed('enter') then
    sound_handles[2] = eowu.Sound('guitar'):Play()
  end

  if kb:Down('space') then
    s1:ZRotate(0.03)
    s1:Texture('t1')
  else
    if sound_handles[1] then sound_handles[1]:Stop() end
    s1:Color({1, 1, 0})
  end

  if kb:Down('enter') then
    s1:Opacity(0.5)
  else
    if sound_handles[2] then sound_handles[2]:Stop() end
  end

  s1:Draw()
end

state.Render = {
  default = default_render
}

return state