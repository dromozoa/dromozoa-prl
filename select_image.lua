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

local base64 = require "dromozoa.commons.base64"
local sequence_writer = require "dromozoa.commons.sequence_writer"
local image = require "dromozoa.image"

local filename, x, y, w, h = assert(...)

local img1 = image.read_file(filename)
local min_x = assert(tonumber(x))
local min_y = assert(tonumber(y))
local max_x = min_x + assert(tonumber(w)) - 1
local max_y = min_y + assert(tonumber(h)) - 1

local result_min_x
local result_max_x
local result_min_y
local result_max_y

for p in img1:each(min_x, max_x, min_y, max_y) do
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

local x = assert(result_min_x)
local y = assert(result_min_y)
local w = assert(result_max_x) - x + 1
local h = assert(result_max_y) - y + 1

local img2 = image({
  width = w;
  height = h;
  channels = 1;
  min = 0;
  max = 1;
})
local q = img2:pixel()

for p in img1:each(result_min_x, result_max_x, result_min_y, result_max_y) do
  if p.Y > 0 then
    q:gray(1)
  else
    q:gray(0)
  end
  q:next()
end

io.write(("%d %d\n"):format(result_min_x, result_min_y))

img2:write_file("test.png")
local data = base64.encode(img2:write_png(sequence_writer()):concat())
print(data)
local img3 = image.read(base64.decode(data))
img3:write_file("test.tga")
