ePuck_RL_Agents
===============
The Reinforcement Learning Agents in this package are of two different types.
One being the continuous action algorithms and the others handling discrete actions.
Each pair of header and source code files have a certain terminology.

  XAgent_l <-- the ending "_l" indicates the use of eligibility traces (l for lambda)
  |
  +---- the first letter can be c(ontinuous) or Q/S for Qlearning or SARSA
  
More complicated classes of agents inherit simpler baseclasses, e.g. QAgent_l inherits Agent_l inherits Agent.
Therefore all baseclasses must also be present if you plan on including some higherlevel classes in your code.
