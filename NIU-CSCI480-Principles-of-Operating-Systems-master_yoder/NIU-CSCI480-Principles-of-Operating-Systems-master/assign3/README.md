

CSCI 480                                                 Autumn, 2018

**Assignment 3 -- Microshell Simulation**

(100 points)

In this assignment, you will implement a microshell in C/C++.

You will need to use several system calls under Linux including

fork(), pipe() and dup().  This assignment&#39;s logic is more complex

than in the previous assignments.  Please allocate sufficient time for this assignment (suggestion:  multiple days).

The name of the executable file should be &quot;Assign3&quot;.

Your shell does the following:

--- Print a prompt &quot;480shell\&gt;&quot; and wait for input.

--- Read the input typed after the prompt.

--- Execute the command typed in after the prompt and print a new

    prompt.

--- The shell understands the commands &quot;quit&quot; and &quot;q&quot; as the special

    commands to exit.

--- The shell understands a special symbol &quot;||&quot;, by which you can

    pipe the output of one command to next command.  To simplify,

    this assignment only requires one pipe between two commands, such

    as in:

    cat ourfile || sort

    Please note that the standard Unix/Linux pipe is &quot;|&quot;, which is

    different from what our microshell would understand.

---------------------------------------------------------------------

Input File

A text file is needed to produce the sample output.  The file name is

&quot;ourfile&quot;.  Create a file containing 4 lines:

NIU

Illinois

USA

DeKalb

Sample Output

turing%\&gt;480sh

480shell\&gt;cat ourfile ||sort

DeKalb

Illinois

NIU

USA

480shell\&gt;quit

turing%\&gt;mysh

480shell\&gt;more ourfile || grep DeKalb

DeKalb

480shell\&gt;ls

ourfile  480sh

480shell\&gt;q

turing%\&gt;

---------------------------------------------------------------------

Background Knowledge

1. The assignment will need several system calls for process

   management and IPC such as fork(), exec(), pipe(), dup().  You

   need to read the Linux manual page to understand their usage.  You

   will use fork() to create two child processes and use pipe() with

   the help of dup()/dup2() to set up the communication between the

   child processes.  The system call dup() is used to duplicate the

   file descriptors so that you can replace the standard input or

   output of a process by the file descriptors of a pipe.

2. You need to close all the unneeded file descriptors of the pipes.

   You need to close the two pipe file descriptors in the parent

   process, the read end of the pipe for the first child process,

   and the write end of the pipe for the second child process.  After

   the dup() is called in each child process, you can close the

   write end for  the first process and the read end for the second

   process since they are no longer needed.

3. You may need strtok() to parse the command line for you.  Read the

   manual page to understand this function.  You can use other ways

   to parse the line too.

4. When you use execv() or execvp(), you need to build an array of

   pointers to the arguments.  The last element of the array should

   be (char \*) NULL.

5. The parent process needs to call waitpid() to wait for the

   completion of the commands.

Tackle the problem step by step:

--- Make sure that your shell is taking inputs correctly.

--- Next test the execution of commands without any pipe involved.

--- Now you can go ahead and solve the pipe problem.

--------------------------------------------------------------------

Programming Hints

How do I find the library for a system call?

Check the manual page.  For example:  Do &quot;man strtok&quot;.  In the

synopsis, you will see:

     #include \&lt;strings.h\&gt;

If you are interested in the exact contents of the header file, you

can go to /usr/include and do &quot;more strings.h&quot;.

How do I structure the program?

You can start with the following example and add logic to handle pipe(), etc.  And of course, you will need to define several functions due to the not-so-trivial logic.

#include \&lt;sys/types.h\&gt;

#include \&lt;sys/wait.h\&gt;

#include \&lt;stdio.h\&gt;

int main(void)

{

 char buf[1024];

 pid\_t pid;

 int status;

 printf(&quot;%% &quot;);

 while (fgets(buf,1024,stdin) != NULL)

 {

   buf[strlen(buf) -1] = 0;  //remove the last character. Important!

   if ((pid = fork()) \&lt;0)

    printf(&quot;fork error&quot;);

   else if (pid == 0)

    { /\* child \*/

     execlp(buf, buf, (char \*) 0);

     printf(&quot;couldn&#39;t execute: %s&quot;, buf);

     exit(127);

    }

   /\* parent \*/

 if ( (pid = waitpid(pid, &amp;status, 0)) \&lt; 0)

  printf(&quot;waitpid error&quot;);

 printf(&quot;%% &quot;);

 }

 exit(0);

}

How do I parse the commands?

One approach is to use strtok() which can help you parse the

commands you get from input.  There are other ways such as using

c++ string functions.  If you choose to use strtok(), the syntax is

like:

       char \*strtok(char \*s1, const char \*s2);

The following is extracted from the Unix manual page.  See the manual for the full description:

It can be used to break the string pointed to by s1 into a sequence of tokens, each of which is delimited by one or more characters from the string pointed to by s2.

 The first call (with pointer s1 specified) returns a pointer to the first character of the first token, subsequent calls (which must be made with the first argument being a null pointer) will work through the  string s1 immediately following that token.  In this way subsequent calls will work through the string s1 until no tokens remain.  When no token remains in s1, a null pointer is returned.

Example:

       Suppose string is &quot;abcd efgh || ijk lmn&quot;

       number =0;

       command[0] = strtok(string,&quot;||&quot;);

        while((ptr = strtok(NULL, &quot;||&quot;)) != NULL)

           {

         number++;

               command[number] = ptr;

           }

   Then command[0] will contain &quot;abcd efgh&quot; and command[1] will contain &quot;ijk lmn&quot;

How do I do error handling?

You need to check the return value of a system call.  For example,  if the return value of -1 indicates an error, you should have a block that handles it.  The simplest way to handle is to print out some error page and then exit the program.

---------------------------------------------------------------------

Requirements

The program should:

--- work according to the specifications

--- be comprehensible and well documented

--- check the return value of the system calls and have proper error

    handling

---------------------------------------------------------------------

Submission

Submission requirement is the same as in the 1st assignment.  Note that the directory must be called: &quot;z1234567\_project3\_dir&quot;, all in

lowercase.  Important steps are repeated below:

You will submit a compressed file through Blackboard under

&quot;Assignments&quot; as usual.

The compressed file contains your source code and a Makefile. It

should be named as &quot;your-zid\_A3.tar&quot; and must be created

following the procedure described below:

1.        Put all your source code files (NO OBJECT or EXECUTABLE FILES) and your Makefile in a directory called &quot;your-zid\_A3\_dir&quot;.

Example:   z1234567\_A3\_dir.  Note: &#39;z&#39; must be in lower case.

In your Makefile, you need to make sure your compilation produces

the executable file called &quot;your-zid\_A3&quot;.  For a student with

z1234567 as her zid, the executable would be z1234567\_A3.

2.        In  the parent directory of your-zid\_A3\_dir, compress this

whole subdirectory by the following command:

       tar  -cvvf  your-zid\_A3.tar  your-zid\_A3\_dir

Example:

        tar -cvvf   z1234567\_A3.tar z1234567\_A3\_dir

&quot;your-zid\_A3.tar&quot; is now the compressed file containing the

whole subdirectory of your files. You can then transfer (e.g. using a

secure ftp client) the tar file from turing to a computer on which you can open a web browser for your final submission to the

Blackboard system.

