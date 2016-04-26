#! /bin/sh -e

# Copyright (C) 2016 Tomoyuki Fujimori <moyu@dromozoa.com>
#
# This file is part of dromozoa-prl.
#
# dromozoa-prl is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# dromozoa-prl is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with dromozoa-prl.  If not, see <http://www.gnu.org/licenses/>.

name=$1
case x$name in
  x) exit 1;;
esac
shift

prlctl start "$name"

i=0
while test $i -lt 10
do
  i=`expr $i + 1`
  now=`date '+%Y-%m-%d-%H-%M-%S'`
  prlctl capture "$name" --file "$name-$now.png"
  sleep 1
done

prlctl stop "$name" --kill
