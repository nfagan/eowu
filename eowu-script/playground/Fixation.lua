local Fixation = {}

Fixation.Duration = 10

local eleven = 12

function Fixation.Entry()

  print(eleven)
  eleven = 13

end

function Fixation.Exit()
  print("After setting: ", eleven)
end

function Fixation.Loop()

end

return Fixation