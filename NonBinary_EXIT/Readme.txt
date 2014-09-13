***********************************************************************************
*
* Matlab source code for using EXIT charts to design nonbinary LDPC codes.
*
* Version 1.1, August 2005
* by Amir Bennatan, David Burshtein
* Department of Electrical Engineering-Systems, Tel Aviv University
* Change log:
*	30 August 2005,		Initial version
*	13 November 2005,		Minor bug correction
*
***********************************************************************************

---------
Overview
---------

This file applies the EXIT chart design method presented at our ISIT'2005 paper,
which is called EXIT Method no. 2 in our paper "Design and Analysis of Nonbinary 
LDPC Codes for Arbitrary Discrete-Memoryless Channels".  

In particular,

- Computation of the approximation of functions J, J_R and of the EXIT curves
- Design of codes for given a mapping and SNR.

The main file, and the only file that should concern you as a user, is "main.m".  
The file contains comments that explain how to modify it to design a code.  The 
only sections that need modification are the sections titled "Initial constants" 
and "Design" (optional).

---------
Remarks
---------

1. The program uses an improvised database - the files "JPols.mat" and 
"CND_and_JR_Pols.mat".  These files contain the computation of the J, J_R and
I_CND curves.  The computation of the curves is performed only once, and 
stored in the database for further use.   

2. Once a curve is computed, it stays in the file forever.  Thus, a file
may occasionally grow too large.  This hasn't happened to us.  However, if it
happens, delete the file (or move it to another directory).  The next time the 
program runs, it will create a new file. Note that it will need to re-compute the 
relevant curves.

3. The current files "JPols.mat" and "CND_and_JR_Pols.mat" contain data for
several curves that we used in our work.  The file "main.m" contains the design
process that we used to create the code provided in the ISIT paper (which is also 
the first code presented in the journal paper).

4. Throughout your use of the functions, you will notice occasional Matlab
warning.  We could not find a way to silence these warnings, and you may ignore 
them.

