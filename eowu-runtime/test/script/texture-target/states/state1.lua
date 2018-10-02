local state1 = {}

state1.Name = 's1'
state1.Duration = -1
state1.First = true
state1.Variables = { ib = false }

function state1.Entry()
  local script = eowu.script()
  local state = script:State('s1')
  local t1 = script:Target('t1')
  local ib = state:Variable('ib')

  script:Render('setup')

  t1:Entry(function() ib:Set(true) end)
  t1:Exit(function() ib:Set(false) end)
end

local function position_stimulus(kb, stim, mv_amt)
  if kb:Down('left') then stim:Move({-mv_amt, 0}) end
  if kb:Down('right') then stim:Move({mv_amt, 0}) end
  if kb:Down('up') then stim:Move({0, mv_amt}) end
  if kb:Down('down') then stim:Move({0, -mv_amt}) end
  if kb:Down('c') then stim:Position({0.5, 0.5}) end
end

local function render_once()
  local script = eowu.script()
  local s1 = script:Stimulus('s1')
  local s3 = script:Stimulus('s3')

  s1:Units('normalized')
  s1:Size({0.5, 0.5})
  s1:Position({0.5, 0.5})

  s3:Size({1, 1})
  s3:Position({0.5, 0.5})
  s3:Units('normalized')

  script:Render('state1')
end

local function default_render()
  local script = eowu.script()
  local state = script:State('s1')
  local ib = state:Variable('ib'):Get()
  local kb = script:Keyboard()
  local s1 = script:Stimulus('s1')
  local s2 = script:Stimulus('s2')
  local s3 = script:Stimulus('s3')

  position_stimulus(kb, s1, 0.01)

  s1:Texture('t1')
  s2:Like(s1)
  s3:Texture('t1')

  if ib or kb:Down('space') then
    s1:Opacity(1)
    s2:Opacity(1)
  else
    s3:Color({0.5, 0.5, 1})
  end

  s3:Draw()
  s2:Draw()
end

state1.Render = {
  setup = render_once,
  state1 = default_render
}

return state1