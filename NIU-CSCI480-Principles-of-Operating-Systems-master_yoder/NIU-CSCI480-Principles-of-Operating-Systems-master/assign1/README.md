

CSCI 480                                                 Autumn, 2018

Assignment 1 -- System Functions

**(100 points)**

This assignment involves using LINUX system functions such as fork(), getpid(), getppid(), wait() and system().

---------------------------------------------------------------------

Write a program on the turing system using C or C++.  Your executable file should be named Assign1.  It should do the following:

1. Print a message identifying the process as the original process

   and providing its PID and its parent&#39;s PID.

2. Call fork() after printing a message saying you are about to do

   so.

3. If fork() fails, print an error message (&quot;The first fork failed.&quot;)

   and exit with a status of -1.

4. If fork() succeeds, we now have two processes:  parent and child.

   In the child process:

   (a) Print a message identifying it as the child and providing its

       PID and its parent&#39;s PID.

   (b) Call fork() a second time after printing a message saying you

       are about to do so.

   (c) If fork() fails, print an error message (&quot;The second fork

       failed.&quot;) and exit as above.

   (d) If fork() succeeds, we now have three processes:  one parent,

       one child and one grandchild.

       In the grandchild process:

       (i) Print a message identifying it as the grandchild and

           providing its PID and its parent&#39;s PID.

       (ii) Print a message saying it is about to exit.

       (iii) Exit with a status of 0.

       In the child process (after the second fork):

       (i) Print a message identifying it as the child and

           providing its PID and its parent&#39;s PID.

       (ii) Use wait(0) to wait for the grandchild to terminate.

       (iii) Print a message saying it is about to exit.

       (iv) Exit with a status of 0.

   In the parent process:

    (a) Print a message identifying it as the parent and providing

        its PID and its parent&#39;s PID.

    (b) Call the sleep() function to sleep for 2 seconds.

    (c) Print a message saying it is about to call ps.

    (d) Use system() to execute the &quot;ps&quot; command.

    (e) Use wait(0) to wait for the child to terminate.

    (f) Print a message saying it is about to terminate.

    (g) Exit with a status of 0.

5.  The overall program should return a value of 0.

--------------------------------------------------------------------

Notes:

The messages printed by different processes may appear in various orders depending on timing and the order does not have to match the sample output.

Print your output without buffering.  The simplest way to do this is to use stderr instead of stdout.  (In C++, these are cerr and cout, respectively.)  If you are using C, you may be able to use the line &quot;setbuf(stdout, NULL);&quot; at the beginning of the program to get unbuffered output with stdout.  (There is probably some similar way

to do the same with C++.)

You may want to read about any or all of these functions:  getpid(), getppid(), wait(),system(), setbuf(), exit(), etc.  You may want to read about the LINUX command ps.

You may want to look at the fork() example on the course web site.

Your program should use reasonable variable names and should be appropriately indented and well documented.  You can find style guidelines on the web sites of the CSCI 240 and 241 courses.

You should have a makefile.  The name of the executable file should be &quot;Assign1&quot;.

When you are done, you need to submit your work on Blackboard.  You should create a tar file containing the two files involved:  the program file and the makefile.  To do this, you need the &quot;tar&quot; utility.

Do the following (replacing &quot;Znumber&quot; with your own Z-ID):

  (a) Create a subdirectory named Znumber\_A1\_dir.

  (b) Copy the two files into it.

  (c) In the parent directory of Znumber\_A1\_dir, use this command:

      tar -cvf Znumber\_A1.tar Znumber\_A1\_dir

Use an FTP progam to retrieve the tar file and then submit it on Blackboard.  The TA will move it to turing, extract the files and run your makefile, as in:

    tar -xvf Znumber\_A1.tar

    cd Znumber\_A1\_dir

    make

    Assign1

If your makefile does not run or your program does not compile and run, you will receive no credit.

--------------------------------------------------------------------

**Sample Output:**

(The precise order of lines and the id numbers may vary.)

I am the original process.  My PID is  11308  and my parent&#39;s PID is 10456

Now we have the first fork.

I am the parent.  My PID is  11308  and my parent&#39;s PID is  10456

I am the parent, about to call ps.

I am the child.  My PID is  11309  and my parent&#39;s PID is  11308

Now we have the second fork.

I am still the child.  My PID is  11309  and my parent&#39;s PID is  11308

I am the grandchild.  My PID is  11311 and my parent&#39;s PID is  11309

I am the grandchild, about to exit.

I am the child, about to exit.

  PID TTY          TIME CMD

10456 pts/4    00:00:00 bash

11308 pts/4    00:00:00 Assign2

11309 pts/4    00:00:00 Assign2 \&lt;defunct\&gt;

11310 pts/4    00:00:00 sh

11312 pts/4    00:00:00 ps

I am the parent, about to exit.

