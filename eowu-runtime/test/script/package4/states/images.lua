local state = {}

state.Name = 'images'
state.Duration = 2000
state.Variables = {}

local printed_time = false
local last_t = nil

function state.Entry()
  local script = eowu.script()
  local state = script:State('images')

  state:Next('fixation')
  script:Render('image')
end

--  render functions

local function render()
  local script = eowu.script()
  local state = script:State('images')
  local stim = script:Stimulus('sq')

  for i = 1, 100 do
    local s = script:Stimulus('sq' .. i)
    s:Units('normalized')
    s:Position({math.random(), math.random()})
    s:Size({0.01, 0.01})
    s:Color({1, 0, 0})
    s:Draw()
  end

  local pos = stim.position
  pos.x = pos.x + 0.001

  stim:Color({0, 0, 1})

  stim.position = pos
  stim:Draw()
end

state.Render = {
  image = render
}

return state