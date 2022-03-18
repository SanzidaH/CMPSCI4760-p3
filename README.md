# CMPSCI-4760 P3

## Project Goal:

* Getting familiar with semaphores in Linux.
* Solving the concurrency issues using the semaphores for synchronization of processes.

## How to run
Commands to run project from hoare server:

```
$ make
$ ./master -t ss n
```
Here, ss = termination time, n = maximum number of processes.

## Git

https://github.com/SanzidaH/CMPSCI4760-p3.git

## Task Summary

> Master will spawn fan of child processes.
> Slave will process these processes and these processes will attempt to write on same shared segment (entering critical section) creating concurrency issue.
> This problem will be solved using semaphores.
