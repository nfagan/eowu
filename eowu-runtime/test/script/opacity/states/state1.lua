local state1 = {}
state1.Name = 'state1'
state1.Duration = -1
state1.First = true

function state1.Entry()
  local script = eowu.script()
  script:Render('state1')
end

local function position_stimulus(kb, stim, mv_amt)
  if kb:Down('left') then stim:Move({-mv_amt, 0}) end
  if kb:Down('right') then stim:Move({mv_amt, 0}) end
  if kb:Down('up') then stim:Move({0, mv_amt}) end
  if kb:Down('down') then stim:Move({0, -mv_amt}) end
end

local function default_render()
  local script = eowu.script()
  local kb = script:Keyboard()
  local s1 = script:Stimulus('s1')
  local s2 = script:Stimulus('s2')
  local s3 = script:Stimulus('s3')

  s1:Units('normalized')
  s1:Size({0.5, 0.5})
  s1:Position({0.5, 0.5})
  s1:Texture('t1')

  s2:Like(s1)

  s3:Size({1, 1})
  s3:Position({0.5, 0.5})
  s3:Units('normalized')
  s3:Texture('t1')

  if kb:Down('space') then
    s1:Opacity(0.8)
    s2:Opacity(0.5)
  else
    s1:Opacity(1)
    s2:Opacity(1)
  end

  position_stimulus(kb, s1, 0.01)

  s3:Draw()
  s2:Draw()
  -- s1:Draw()
end

state1.Render = {
  state1 = default_render
}

return state1