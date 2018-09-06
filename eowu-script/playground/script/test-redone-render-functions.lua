State = {}

State.ID = '1'

function State.Entry()
end

function State.Exit()
end

function State.Loop()
end

local function default_render()
  print('rendering')
end

State.Render = {
  default = default_render
}