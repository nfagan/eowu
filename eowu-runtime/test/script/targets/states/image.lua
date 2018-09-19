local image = {}

function image.Entry()
  local script = eowu.script()
  local state = script:State('image')
  local targ = script:Target('t1')
  local made_choice = script:Variable('made_choice')

  made_choice:Reset()

  local choice1 = script:MakeChoice('c1', {'t1', 't2'})

  choice1:Entry(function(id)
    print('entered: ' .. id)
  end)

  choice1:Exit(function(id)
    print('exited: ' .. id)
  end)

  choice1:Choose(function(id)
    print('chose: ' .. id)
  end)

  state:Next('fixation')
  script:Render('image')
  targ:Reset()

  print('Entered ' .. state.name)
end

function image.Loop()
  local script = eowu.script()
  local state = script:State('image')

  if state:Ellapsed() > 2 then
    local made_choice = state:Variable('made_choice')

    if ~made_choice:Get() then
      state:Next()
    end
  end
end

local function render()
  local script = eowu.script()
  local stim = script:Stimulus('stim')
  local stim2 = script:Stimulus('stim2')
  local t1 = script:Target('t1')
  local t2 = script:Target('t2')

  stim:Color({1, 1, 0})
  stim:Draw()   
  stim2:Draw()
  t1:Draw()
  t2:Draw()
end

image.First = false
image.Name = 'image'
image.Duration = 2000
image.Variables = {
  made_choice = false,
}
image.Render = {
  image = render
}

return image