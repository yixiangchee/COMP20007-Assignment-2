# COMP20007-Assignment-2

Design of Algorithms Assignemnt on __Dynamic Programming__ and __Graph Algorithms__.

This assignment tackles the _Critical Node Detection Problem_ in networks.

Some Algorithms Used:
  * Depth-First Search
  * Floyd's Algorithm

Marks: 19/20
(Coding Portion: 8/8)

[Specification](/assignment2.pdf)  
[Report](/Report.pdf)
[Code](/a2-code/)

To run the programs you must first compile with make followed by one of task2, task3, task4 or task7. 
For all tasks, you should give the network file as an argument and can send in the outage information via standard input redirection.

For example:
```
make task2
make task3
make task4
make task7
./task2 tests/network-1.txt < tests/outage-1.txt
./task3 tests/network-1.txt < tests/outage-1.txt
./task4 tests/network-1.txt < tests/outage-1.txt
./task7 tests/network-1.txt < tests/outage-1.txt
```
