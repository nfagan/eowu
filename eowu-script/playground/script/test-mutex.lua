State = {}

function render()
  print('yo')
end

local start_time = os.time()

function State.Entry()
  if os.time() - start_time > 1 then
    print('setting from entry')
    eowu.test(State.Render)
  end
end

function State.Exit()
end

function State.Loop()
end

function State.Render()
  print('setting from render')
  -- eowu.test(State.Render())

  eowu.test(State.Entry)

  eowu.test(function()
    print('sup')
  end)

  eowu.test(function() 
    -- print('hi')
  end)
end