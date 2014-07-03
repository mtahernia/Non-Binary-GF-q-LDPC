***********************************************************************************
*
* C++ source code for running Coset GF(q) LDPC simulations 
*
* Version 1.0, May 1, 2006
* by Amir Bennatan, David Burshtein
* Department of Electrical Engineering-Systems, Tel Aviv University
*
***********************************************************************************

---------
Overview
---------

The attached source code implements coset GF(q) LDPC simulations on the AWGN channels.  
The implementation is based on the paper:

A. Bennatan and D. Burshtein, "Design and analysis of nonbinary LDPC codes for arbitrary discrete-memoryless channels", IEEE Trans. Inf. Theory, vol. 52, pp. 549-583, Feb. 2006.

Also provided is source-code for a coset GF(q) LDPC encoder.  This source-code is not used in the simulations (it is "dead code" in the sense that it is not run from anywhere).  It is provided for the benefit of users who wish to modify the attached code. 

Although we have gone to great lengths to make the program as user-friendly as possible, regretfully, we are unable to provide any support for the attached code.  Use it at your own risk.  See "conditions of use" in the attached license file BSD.txt.

---------
Remarks
---------

1. The source code is suitable for both Linux and Visual-C++.  A Visual-C++ project file and a Linux makefile are included.  The produced executable is called "q_LDPC", and takes command-line arguments (run the executable to obtain a description of the arguments).  

2. One of the arguments is an input file.  This file should contains the definition of the code to be run.  The excutable produces a random code, with each simulation, according to the edge distributions provided in this file.  See Work.opt for an example of this file.

3. The maximum size of each decoder message is hard-set to 64.  When the actual code uses a smaller alphabet, it is advised to change this to improve memory efficiency.  This is done by modifying the constant MAX_Q in Utils_1.h and recompiling the file.

4. One of the command-line arguments is channel.  The user has the choice of an AWGN and a BSC channel.  It does not make much sense to use a nonbinary code on a BSC channel.  This option is provided as an example for the benefit of users who wish to adapt the program to additional channels.