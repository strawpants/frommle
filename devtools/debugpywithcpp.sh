#!/bin/bash

set -m
#debug poth python and the attached c++ library

python3 -m pdb $1 &

pidnr=$!

echo $pidnr

konsole --new-tab -e gdb --pid=$pidnr & 


fg %python3

