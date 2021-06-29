# COMP20007-Assignment-2

Design of Algorithms Assignment on __Dynamic Programming__ and __Graph Algorithms__ in C Language.

This assignment tackles the __*Critical Node Detection Problem*__ in networks.

Some Algorithms Used:
  * Depth-First Search
  * Floyd's Algorithm

Marks: 19/20

(Coding Portion: 8/8)

[Specification](/assignment2.pdf) 

[Report](/Report.pdf)

[Code](/a2-skeleton)

___

To run the programs (command line) you must first compile with ```make``` followed by one of task2, task3, task4 or task7. 
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

There are 4 example networks, named ```network-(1/2/3/4).txt```

There are 4 example outages, named ```outage-(1/2/3/4).txt```

Example outputs are in ```t(2/3/4/7)-out-(1/2/3/4).txt```

all available [here](/a2-skeleton/tests). 


Visual representation of the network configurations are in the appendix of [the specification](/assignment2.pdf) (p.g. 11).

