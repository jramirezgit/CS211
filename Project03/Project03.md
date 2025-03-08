# Project 3 - Win 80% of Votes but Lose Presidency??? #
## Popular Vote Minimizer ##
### (Re)Cursing the Electoral College System for US Presidential Elections ###

The president of the United States is not elected by a popular vote, but by a majority vote in the Electoral College.
Each of the 50 states, plus DC, gets some number of electors in the Electoral College, and whoever they vote in becomes 
the next President. There are four occurrences where the winner of the Electoral College and eventual President Elect 
did not win the popular vote (plus the 1824 election where the House of Representatives decided the winner). For the 
purposes of this project, we're going to make some simplifying assumptions:
- You need to win a majority of the votes in a state to earn its electors, and you get all the state's electors if you 
win the majority of the votes. For example, in a small state with 999,999 people, you'd need 500,000 votes to win all 
its electors. Similarly, in a state with 1,000,000 people, you'd need 500,001 votes to win all its electors. These 
assumptions aren’t entirely accurate, both because in most states a plurality suffices and some states split their 
electoral votes in other ways.
- You need to win a majority of the electoral votes to become president. In the 2008 election, you'd need 270 votes 
because there were 538 electors. In the 1804 election, you'd need 89 votes because there were only 176 electors. (You 
can technically win the presidency without winning the Electoral College; we'll ignore this for simplicity.)
- Electors never defect. The electors in the Electoral College are free to vote for whomever they please, but the 
expectation is that they'll vote for the candidate that won their home state. As a simplifying assumption, we'll just 
pretend electors always vote with the majority of their state.

This project explores the following central question: under the assumptions laid out above, for past elections ...

### What was the fewest number of popular votes you could get and still be elected President of the United States of America? ###

In order to answer this central question, the programming tasks involve command-line arguments, writing a useful 
makefile, file input, writing a recursive function, developing your own test cases, optimizing recursion using 
memoization, and file output. 