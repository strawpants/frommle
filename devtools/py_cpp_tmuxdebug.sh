#!/bin/bash
#starts a tmux session with a vim editor using both pdb and gdb to debug python with c++ code
#usage
#py_cpp_tmuxdebug.sh path/tomainfile.py [extra arguments]

vimcomm="vim +'packadd termdebug'  +'let g:termdebug_wide=163'  +'e  $1' +'Termdebug python3' +'Run -m pdb $*'"
tmux new-session  "$vimcomm"
