function TestCase()

  local model = eowu.CreateModel('id')
  local y = eowu.Model('id')

  model:Color(0.5, 0.5, 1.0)

  local pos = model.position

  assert(pos.x == 0 and pos.y == 0 and pos.z == 0, 'Failed to initialize position')

  pos.x = 10
  model.position = pos

  assert(model.position.x == 0, 'Non-committed position should remain 0.')
  print('OK:', 'Non-committed position remained 0.')

  eowu.Execute()

  assert(model.position.x == 10, 'Committed position should update.')
  print('OK:', 'Commited position updated')

end