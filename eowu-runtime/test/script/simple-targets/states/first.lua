local first = {}

function first.Entry()
  local script = eowu.script()
  local t1 = script:Target('t1')

  script:Render('r1')

  t1:Entry(function()
    script:Render('r2', 'f1')

    --  render after 1 second
    local t1 = script:MakeTimeout('t1', 1000, function()
      script:Render('r3')
    end)
  end)

  t1:Exit(function()
    script:Render('r1')
    script:Timeout('t1'):Cancel()
  end)
end

local function position_stimulus(s1)
  local kb = eowu.script():Keyboard()

  s1:Units('normalized')
  s1:Size({0.5, 0.5})

  local mv_amt = 0.01

  if kb:Down('left') then s1:Move({-mv_amt, 0}) end
  if kb:Down('right') then s1:Move({mv_amt, 0}) end
  if kb:Down('up') then s1:Move({0, mv_amt}) end
  if kb:Down('down') then s1:Move({0, -mv_amt}) end
  --  recenter
  if kb:Down('c') then s1:Position({0.5, 0.5}) end
end

local function in_bounds_render()
  local script = eowu.script()
  local s1 = script:Stimulus('s1')
  local t1 = script:Target('t1')

  position_stimulus(s1)

  -- s1:Color({1, 0, 0})
  s1:Opacity(0.1)
  s1:Draw()

  t1:Draw()
end

local function ellapsed_render()
  local script = eowu.script()
  local s1 = script:Stimulus('s1')
  local t1 = script:Target('t1')

  position_stimulus(s1)

  s1:Color({0, 0.8, 1})
  s1:Opacity(1.0)
  s1:Draw()

  t1:Draw()
end

local function default_render()
  local script = eowu.script()
  local s1 = script:Stimulus('s1')
  local t1 = script:Target('t1')

  position_stimulus(s1)

  s1:Color({0, 1, 1})
  s1:Draw()

  t1:Draw()
end

local function flip(name, t)
  --
end

first.Name = 's1'
first.Duration = -1

first.Render = {
  r1 = default_render,
  r2 = in_bounds_render,
  r3 = ellapsed_render
}

first.Flip = {
  f1 = flip
}

return first