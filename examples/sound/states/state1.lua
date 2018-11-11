local state = {}

state.Name = 'state1'
state.First = true
state.Duration = -1

function state.Entry()
  local script = eowu.script()
  script:Render('default')

  print('Press + hold space to play sound!')
end

local sound_handle = nil

local function default_render()
  local script = eowu.script()
  local kb = script:Keyboard()
  local s1 = script:Stimulus('s1')

  s1:Geometry('circ')
  s1:Units('mixed')
  s1:Size({200, 200})
  s1:Position({0.5, 0.5})

  local tex = 't1'

  if kb:Down('space') then
    s1:ZRotate(0.03)

    if kb:Pressed('space') then
      local sound = script:Sound('piano')
      sound_handle = sound:Play()
    end

    s1:Texture(tex)
  else    
    if sound_handle ~= nil then
      sound_handle:Stop()
    end

    s1:Color({1, 0, 0})
  end

  s1:Draw()
end

state.Render = {
  default = default_render
}

return state