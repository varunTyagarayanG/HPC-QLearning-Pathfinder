# Q-Learning
Q-Learning is a model-free reinforcement learning algorithm which learns the value of an action in a particular state. It can be used to teach a program to learn based on letting explore and finding the best actions to perform based on the state it is at. In this example of Q-Learning, I am using Q-Learning to teach a program to find the shortest path from point A to point B within a graph of nodes:

![graph](https://github.com/sababot/qlearning/blob/master/q-learning-graph.jpeg?raw=true)

Reward Matrix:
```
        0    1    2    3    4    5    6    7
        ------------------------------------
 
0  |   -1    0   -1   -1   -1   -1   -1   -1
   |
1  |    0   -1    0   -1    0    0   -1   -1
   |
2  |   -1    0   -1    0    0   -1   -1   100
   |
3  |   -1   -1    0   -1   -1   -1   -1   -1
   |
4  |   -1    0    0   -1   -1    0   -1   100
   |
5  |   -1    0   -1   -1    0   -1    0   -1
   |
6  |   -1   -1   -1   -1   -1    0   -1   -1
   |
7  |   -1   -1    0   -1    0   -1   -1   100
```

# Setup
Clone this repository and in order to compile the program just run:

```g++ main.cpp utils.cpp```

# Future Work
In the near future, I will use this algorithm (or an algorithm based on this algorithm) to train the computer to drive cars on a narrow road.



                      +---------------------------+
                      |        Start              |
                      +---------------------------+
                                 |
                                 V
                      +---------------------------+
                      | Input initial state       |
                      +---------------------------+
                                 |
                                 V
                      +---------------------------+
                      | Populate ran[] array      |  
                      | (rand() % 8 in loop)      |
                      +---------------------------+
                                 |
                                 V
                      +---------------------------+
                      | Initialize Q-Matrix       |
                      | & R-Matrix                |
                      | (Hardcoded rewards:       |
                      |  - Default: -1, Allowed: 0|
                      |  - Goal transitions: 100) |
                      +---------------------------+
                                 |
                                 V
                      +---------------------------+
                      | Print R-Matrix            |
                      +---------------------------+
                                 |
                                 V
                      +---------------------------+
                      | Create LOGS directory     |
                      | (_mkdir("LOGS"))          |
                      +---------------------------+
                                 |
                                 V
                      +--------------------------------------+
                      | Open log files for:                  |
                      | - Scores  (log_seq.csv)              |
                      | - Time    (log_time.csv)             |
                      | - Memory  (log_mem.csv)              |
                      +--------------------------------------+
                                 |
                                 V
                      +--------------------------------------+
                      | Start overall training timer         |
                      +--------------------------------------+
                                 |
                                 V
                      +--------------------------------------+
                      | Training Loop (500 iterations)       |
                      |                                      |
                      |   For each iteration:                |
                      |   +-------------------------------+  |
                      |   | Start iteration timer         |  |
                      |   +-------------------------------+  |
                      |           |                          |
                      |           V                          |
                      |   +-------------------------------+  |
                      |   | current_state =                |  |
                      |   |    returnRandom(ran)           |  |
                      |   +-------------------------------+  |
                      |           |                          |
                      |           V                          |
                      |   +-------------------------------+  |
                      |   | available_acts =               |  |
                      |   |    available_actions(          |  |
                      |   |       current_state,           |  |
                      |   |       available_acts, rMatrix)  |  |
                      |   +-------------------------------+  |
                      |           |                          |
                      |           V                          |
                      |   +-------------------------------+  |
                      |   | action = sample_next_action(   |  |
                      |   |       size_av_actions,         |  |
                      |   |       available_acts, ran)     |  |
                      |   +-------------------------------+  |
                      |           |                          |
                      |           V                          |
                      |   +-------------------------------+  |
                      |   | score = update(                |  |
                      |   |        current_state, action,  |  |
                      |   |        rMatrix, qMatrix,        |  |
                      |   |        max_index, ran, gammaLR) |  |
                      |   +-------------------------------+  |
                      |           |                          |
                      |           V                          |
                      |   +-------------------------------+  |
                      |   | Log: score, iteration time,    |  |
                      |   | memory usage to respective     |  |
                      |   | log files                      |  |
                      |   +-------------------------------+  |
                      |           |                          |
                      |           V                          |
                      |   +-------------------------------+  |
                      |   | End iteration (clear screen)   |  |
                      |   +-------------------------------+  |
                      +--------------------------------------+
                                 |
                                 V
                      +---------------------------+
                      | End overall training timer|
                      | Print total training time |
                      +---------------------------+
                                 |
                                 V
                      +---------------------------+
                      | Normalization:            |
                      | - Find maximum Q-value    |
                      |   (final_max from qMatrix)|
                      +---------------------------+
                                 |
                                 V
                      +---------------------------+
                      | Print normalized Q-Matrix |
                      +---------------------------+
                                 |
                                 V
                      +--------------------------------------+
                      | Find Optimal Path:                   |
                      | - Set curr_state = initial_state     |
                      | - While goal not reached:            |
                      |     -> For each state not visited,   |
                      |        find max Q value using update() |
                      |        (tie-breaking using returnRandom)|
                      |     -> Mark state as visited          |
                      +--------------------------------------+
                                 |
                                 V
                      +---------------------------+
                      | Print Optimal Path        |
                      +---------------------------+
                                 |
                                 V
                      +---------------------------+
                      |           End             |
                      +---------------------------+

