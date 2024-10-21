set confirm off
set pagination off
target extended-remote localhost:3333
load
break main
continue
tui layout src
