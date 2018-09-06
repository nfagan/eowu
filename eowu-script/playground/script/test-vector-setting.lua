function TestCase()

  local vec = eowu.Vector()

  local zero_msg = 'Value was not initialized to 0'

  assert(vec.x == 0, zero_msg)
  assert(vec.y == 0, zero_msg)
  assert(vec.z == 0, zero_msg)

  print('OK:', 'Initialized vector to 0s')

  vec.x = 100.0

  assert(vec.x == 100, 'Setting vector property did not change property')
  print('OK:', 'Setting vector property changed property.')
end