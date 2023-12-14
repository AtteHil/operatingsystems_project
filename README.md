Operating Systems and System Programming Project Repository
============================================================
This repository contains the projects for the course Operating Systems and System Programming.
Projects are written in C and are ment to be run on Linux environment.

Project 1 - Reverse:
--------------------
The program reads a text file and prints it's content in reverse order. The program takes two files as an argument. The first file is the input file and the second file is the output file, where the reversed text will be written. If the second file is not provided, the program will print the reversed text to standard output stream.

Project 2 - Unix Utilities:
---------------------------
The program implements a few simple Unix utilities. The program takes a file as an argument and a command. The command can be one of the following:
* `./my-zip`: Zips the content of a file to an output file.
* `./my-unzip`: Unzips the content of a file to standard output stream.
* `./my-cat`: Prints the content of a file to standard output stream.
* `./my-grep`: Prints the lines of multiple files that contain a given string to standard output stream. If no files are provided, the program reads from standard input stream.

Project 3 - Unix Shell:
-----------------------
The program implements a simple Unix shell, which can take simple commands and execute them. The program can take a file as an argument, or command line arguments. If a file is provided, the program will read the commands from the file and execute them.
Shell can be executed with the following command: 
* `./wish`


Authors:
--------
These projects were written and developed as a pair work by:
* **Aleksi Haapalainen**
* **Atte Hiltunen**