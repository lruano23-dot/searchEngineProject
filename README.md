A project made for my CS 251 class.
This project is a search engine and it works by reading in a text file that contains
a web address and the line below the address is the words from that specific webpage. The program
then creates an inverted index where each word of text is associated with the web address. That way 
when a user searches a word the web address or any other web address that also contains that word will pop up.

There are 4 files that make up this project:

  textFiles Folder: This folder contains 5 text files that each hold websites that can be used within the project.

  main.cpp: This file is the cpp code that first asks the user to input what text file they want the program to read
  and then runs the searchEngine() function.

  makefile: Contains 3 configurations to compile, run, and clean the program.

  search.h: Contains 12 different functions including the searchEngine function that runs in main.
  There are functions for formatting and building the inverse index and there are also functions for making a query.
  Each function has more in depth explanation and comments in the file. 
