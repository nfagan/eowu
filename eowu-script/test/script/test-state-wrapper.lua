State1 = {}
State2 = {}

State1.ID = '1'
State2.ID = '2'

State1.Duration = 100
State2.Duration = 100

function State1.Entry()
  print('State1!')
  local s = eowu.Task.State('1')
  s:Next('2')
end

function State1.Exit()
  local s = eowu.Task.State('1')
  print(s:Ellapsed())
end

function State1.Loop()  
end

function State1.Render()
end

function State2.Entry()
  print('State2!')
  local s = eowu.Task.State('2')
  s:Next('1')
end

function State2.Exit()
end

function State2.Loop()
end

function State2.Render()
end

States = {
  State1 = State1,
  State2 = State2
}