States = {}

function RenderFunction()
  -- print('hello from 2')
end

function AssignFunction(source)
  print('source is:', source)
  local s = load(source)

  assert(s ~= nil)

  return s
end

local t = os.time()
local updated = false

function States.Loop()
  local dur = 1

  if os.time() - t > dur and not updated then
    local function v() print('hello from another') end
    -- eowu.Render(v)
    eowu.RenderRaw(string.dump(v))
    updated = true
    print('V is:', string.dump(v))
  end
end

-- load(string.dump(function() print('hi') end))