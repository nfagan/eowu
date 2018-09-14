local paths = {}

local function get_outer_directory(path)
  local pattern = '^(.-)([/\\]+)([^/\\]+)$'
  i, j, k = string.match(path, pattern)
  return i, j, k
end