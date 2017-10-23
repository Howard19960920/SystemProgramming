# Multi-thread-PI
calculate PI with Monte Carlo method using multi-thread
Check the reference1.png in the current directory you'll find that function rand() is not thread-safe
To alternate this problem, there are some function guarantee the thread-safe access to rand() 
That is, rand48_r, srand48_r, etc. To see more details about this check the url as follow: https://goo.gl/37cD3D

This is one of the assignments in System Software Programming, tho it ain't hard at all, it requires a lot of 
concepts and background knowledges of Unix like system.

# How to run it
  a.  execute the makefile by simply type make in your terminal in the same directory.(remember the -pthread switch)
  b.  This will produce an execution file named 'pi'
  c.  execute 'pi' with ./pi. Note that 'pi' required two arguments from argv
        (1) How many random (x, y) you want to produce
        (2) How many threads you want this program to run
  d.  The result will show on stdout(basically, your monitor)

# Question 
  ./pi 1000000 {1,2,3,4,5,6,7,8,9,10}
  the executing time is not monotonly decreasing, why is that ?
# end
