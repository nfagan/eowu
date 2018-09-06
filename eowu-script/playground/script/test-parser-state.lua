State1 = {}

local start_time = os.time()

local function render()
  -- eowu.test(State1.Render)
  -- print('sup')
  local y = 10 + 10
end

function State1.Entry()
  if os.time() - start_time > 1 then
    eowu.test(function()
      print('hello')
    end)
  end
end

function State1.Exit()
  print('yo')
end

function State1.Loop()
  local x = 1 + 1
end

local function eleven()
  local y = 1 + 1
end

function State1.Render()
end