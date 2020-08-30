# Poker-River-Solver

This project contains an implementation of Counterfactual Regret Minimization using chance sampling to solve Kuhn poker in C++. The Kuhn poker implementation is heavily borrowed from [1]. 

I am currently working on implementing the same algorithm to solve No-limit Texas Hold-em poker on the river given a board and two players' ranges. The solver works for solving small ranges, where bet sizes and ranges are hardcoded. The hand evaluator is borrowed from [2], and code to use the hand evaluator is borrowed from [3].

## Next Steps

The following is a list of next steps I have currently planned for this project:

  1. Implement passing in a range of hands for each player + bet sizes + raise sizes.
  2. Allow for solving at the turn.
  3. Implement a multi-threaded approach to solving two players' ranges.
  4. Allow for solving at the flop.
  5. Implement a simple GUI to make everything easier to use.
  
## References

[1]http://modelai.gettysburg.edu/2013/cfr/cfr.pdf

[2]https://github.com/christophschmalhofer/poker/blob/master/XPokerEval/XPokerEval.TwoPlusTwo/HandRanks.dat

[3]https://www.codingthewheel.com/archives/poker-hand-evaluator-roundup/

## Contact

Feel free to contact me at calebtkim@gmail.com if you have any questions about this project. 
