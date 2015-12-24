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

local dumper = require "dromozoa.commons.dumper"
local image = require "dromozoa.image"

local filename, min_x, max_x, min_y, max_y = ...
min_x = tonumber(min_x)
max_x = tonumber(max_x)
min_y = tonumber(min_y)
max_y = tonumber(max_y)

local img1 = image.read_file(filename)
local img2 = image({
  width = max_x - min_x + 1,
  height = max_y - min_y + 1,
  channels = 1,
  min = 0,
  max = 1
})
local q = img2:pixel()

for p in img1:each(min_x, max_x, min_y, max_y) do
  if p.Y > 0 then
    q:gray(1)
  else
    q:gray(0)
  end
  q:next()
end

dumper.write(io.stdout, img2):write("\n")
