-- Copyright (C) 2015 Tomoyuki Fujimori <moyu@dromozoa.com>
--
-- This file is part of dromozoa-prl.
--
-- dromozoa-prl is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- dromozoa-prl is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with dromozoa-prl.  If not, see <http://www.gnu.org/licenses/>.

local image = require "dromozoa.image"

local filename, min_x, max_x, min_y, max_y = ...
min_x = tonumber(min_x)
max_x = tonumber(max_x)
min_y = tonumber(min_y)
max_y = tonumber(max_y)

local img = image.read_file(filename)

local result_min_x
local result_max_x
local result_min_y
local result_max_y

local selection = {}
for p in img:each(min_x, max_x, min_y, max_y) do
  if p.Y > 0 then
    local x = p.x
    local y = p.y
    if result_min_x == nil or result_min_x > x then
      result_min_x = x
    end
    if result_max_x == nil or result_max_x < x then
      result_max_x = x
    end
    if result_min_y == nil or result_min_y > y then
      result_min_y = y
    end
    if result_max_y == nil or result_max_y < y then
      result_max_y = y
    end
  end
end

io.write(("%d %d %d %d\n"):format(result_min_x, result_max_x, result_min_y, result_max_y))

