MapReduce is a programming model and an associated implementation for processing and generating large data sets with a parallel, distributed algorithm on a cluster. A MapReduce program is composed of a Map() procedure and a Reduce() procedure.

In this project, you are supposed to briefly introduce the framework of MapReduce (How does it work?), and implement a MapReduce program to count the number of occurrences of each word in a set of documents.  Your task includes the following steps:

Setup MapReduce libraries. A popular open-source implementation is Apache Hadoop.
Write a parallel MapReduce program and a serial program to solve this problem. You are supposed to print your results in non-increasing order of the number of occurrences of words. If two or more words have the same number of occurrences, they must be printed in lexicographical order. Make sure that each line contains one word, followed by its number of occurrences, separated by a space, and there must be no extra space at the end of each line.
Prepare appropriate test data. A set of documents (files) which contains a minimum of 100,000 words must be used for testing.
Test your programs; make sure that the results are accurate. Then compare and analyze the performances between parallel and serial algorithms.
Note that the hardware, the scale of your set of documents, and setting of Hadoop (the number of Map Task and Reduce Task) may all have impact on the performance.  You must give specific analysis on each factor.

Grading Policy:
Programming: Implement the parallel and the serial algorithms (7 pts.).  Write a test of performance program (1 pts.).  All the codes must be sufficiently commented.

Testing: Prepare the test cases, give the run time table, and show the plots (3 pts.).  Write analysis and comments (5 pts.).  The analysis must be based on your testing results.

Documentation: Chapter 1 (1 pt.), Chapter 2 (2 pts.), and finally a complete report (1 point for overall style of documentation).