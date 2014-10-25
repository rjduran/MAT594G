RJ Duran
MAT594G HW2
11/6/12

Platform: MACOSX 10.6.8

Julia:
Keyboard commands
  - Left arrow: increase cImag
  - Right arrow: decrease cImag
  - Up arrow: increase cReal
  - Down arrow: decrease cReal        

defferedViewer:
To smooth out the artifacts I averaged the normal values for 4 surrounding fragments to the current fragment in secondPass.frag. I then used this average normal in the calculations following. I think there are more elegant methods to explore here but this was the most direct approach I could think of at the time.

Build instructions:

cd to julia
run make file
./julia

cd to deferredViewer
run make file
./deferredViewer

