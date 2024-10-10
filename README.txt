In this experiment we had a small problem with one of our computers not producing results that made sense (specified below), but using a different computer gave us results that made sense, and this was the data we wrote the report on.

Makefile
    This file has two commands, one of which runs as described in the project description. Simply run "make main" then "./main N T (-nolock)" where N is the n-value you want the program to compute to, T is the number of threads to use, and the optional "-nolock" is an optional modifier to run without the locks.
    The other command is done by running "make experiment" and then "./experiment". This runs 10 times for each thread count 1-8, with a hardcoded value of 6678923.

Report Materials.xlxs
    Only the first two tabs on the excel spreadsheet are used for the experiment. 
    Those were done on the computer system specified in the report
    The extra data was done on a laptop with 16 GB RAM, AMD Ryzen 5 5500U with 6 cores and 12 threads. This data didn't make any sense, and we were unable to find out why. It is likely something to do with the computer itself, but we do not know
