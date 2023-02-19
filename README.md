# System-Monitoring-Tool (B09 Assignment)
- C program that reports different metrics of the utilization of a given system.
- Runs on Linux Operating Systems

How I solved the problem:
- I imported multiple libraries to make this assignment work:
   - #include<stdio.h>
     * Standard input/output library to be able to use 'printf' for example.
   - #include<stdlib.h>
     * Standard library that allows me to include functions with memory allocation, conversions, system commands, and more.
   - #include<string.h>
     * String library that allows string handling and contains string manipulation functions. For example, I used 'strcmp', which checks if two strings are equal to each other.
   - #include <ctype.h>
     * Header file that declares functions to transform individual characters. For example, I used 'isdigit()' to get if command arguments are integers rather than strings or chars.
   - #include <sys/resource.h>
     * Provides definitions for XSI resource operations. It allows me to get the memory usage using the struct 'getrusage'.
   - #include <sys/utsname.h>
     * Allows me to obtain system information using the struct 'utsname'
   - #include <sys/sysinfo.h>
     * Allows me to obtain the memory information (Phys.Used/Tot -- Virtual Used/Tot) using the struct 'sysinfo'.
   - #include <utmp.h>
     * Keeps track of all logins and logouts to the system. This allows me to retrieve sessions/user information using the struct 'utmp'
   - #include <unistd.h>
     * The unix standard header. It allows me to get the number of cores using 'sysconf(_SC_NPROCESSORS_ONLN);'. It also lets me use the sleep() function for 't_delay'.
- To calculate the physical memory, I did total ram minus free ram, then converted the bytes to gigabytes.
- To calculate the virtual memory, I did (total ram minus free ram) plus (total swap minus free swap). Then, I converted it from bytes to gigabytes. 
- (Note: Linux allocates a lot of free ram to the buffer and cache so that they aren't idling. This means that it is possible to calculate ram another way as well - mentioned on Piazza using cache and buffers, since Linux also has a lot of 'used' ram).
- To get the cpu usages for the "total cpu use" section:
    - I read the first line of the file "/proc/stat".
    - I skipped the first word of that line because it is always the cpu name.
    - I summed up all of the times (numbers) in that first line of the file to get the total time (sum)
    - I divided the 'idle' (4th column or 3rd index), by the sum (total time) to get the cpu usage.
- To test to see if my values for cpu and ram work is by using the rand() function that randomizes values.

Overview of the functions:
- display_memory():
    - Displays the "Memory" section. If there is the flag '--graphics' or '-g', it generates a graphical representation showing the memory used.
    - Takes in 6 parameters:
        - *phys_mem: Pointer of type float that accesses the physical memory value.
        - *virtual_mem: Pointer of type float to the virtual memory value.
        - index: type int that takes the index when iterating through the number of 'samples'.
        - arr_size: type int that takes in the number of 'samples'.
        - is_graphics: type int that returns 1 if the command arguments has the flag '--graphics' or '-g', and returns 0 if it does not.
        - is_sequential: type int that returns 1 if the command arguments has the flag '--sequential', and returns 0 if it does not.
    - Does not return anything
- set_memory():
    - Sets the physical and virtual memory values to be used in display_memory()
    - Takes in 2 parameters:
        - *phys_mem: Pointer of type float that accesses the physical memory value.
        - *virtual_mem: Pointer of type float to the virtual memory value.
    - Does not return anything
- memory_used():
    - Displays the "Memory Usage" section
    - Does not return anything or take any parameters/arguments
- display_session():
    - Displays the "sessions/users" section from reading a file
    - Does not return anything or take any parameters/arguments
- display_cpu():
    - Displays "total cpu use" section. If there is the flag '--graphics' or '-g', it generates a graphical representation showing the memory used.
    - Takes in 5 parameters/arguments:
        - cpu_used: array of type float that takes cpu usage values
        - index: type int that takes the index when iterating through the number of 'samples'.
        - arr_size: type int that takes in the number of 'samples'.
        - is_graphics: type int that returns 1 if the command arguments has the flag '--graphics' or '-g', and returns 0 if it does not.
        - is_sequential: type int that returns 1 if the command arguments has the flag '--sequential', and returns 0 if it does not.
    - Does not return anything
- set_cpu():
    - Sets the cpu values in the "total cpu use" section by reading from a file
    - Takes in 1 parameter/argument:
        - *cpu_used: pointer of type float that points to the memory location of where the value of the cpu usage is stored
    - Does not return anything
- display_core():
    - Displays the "Number of cores" section
    - Takes in 5 parameters/arguments:
        - cpu_used: array of type float that takes cpu usage values
        - index: type int that takes the index when iterating through the number of 'samples'.
        - arr_size: type int that takes in the number of 'samples'.
        - is_graphics: type int that returns 1 if the command arguments has the flag '--graphics' or '-g', and returns 0 if it does not.
        - is_sequential: type int that returns 1 if the command arguments has the flag '--sequential', and returns 0 if it does not.
    - Does not return anything
- display_sys_info():
    - Displays the "System Information" section
    - Does not return anything or take any parameters/arguments
- main():
    - Deals with the different command line arguments, calls all the functions, and displays everything to an output (Ex: terminal)
    - Takes in 2 parameters/arguments:
        - argc: type int that holds the number of command line arguments
        - **argv: pointer to another pointer of type char that stores an array of strings from the command line arguments
    - Returns 0 if there is no error. Any nonzero value indicates some type of failure in the program.

How to run(use) my program:
- To test and run the program, I connected to the lab servers (UTSC lab machines).
- The program accepts multiple command line arguments:
    - '--system': only the system usage will be shown
    - '--user': only the users usage will be shown
    - '--graphics' or '-g': generates a graphical output/representation showing the variation of used memory
    - '--sequential': information is outputted sequentially without "refreshing" the screen
    - '--samples=N': Outputs 'N' number of statistics. 'N' is defaulted to 10 samples.
    - '--tdelay=T': Outputs the statistics every 'T' seconds. 'T' is defaulted to 1 second.

- The program can also accept 2 integer values as the first two command line arguments (after the file name)
    - The first integer value is the number of samples.
    - The second integer value is the number of tdelays.
    - If any of the inputted values are negative numbers, the program will set 'samples' and 'tdelay' to their defaulted values.

- If '--user' and '--system' are both inputted in the command line arguments, the program displays everything
- If incorrect command line arguments are present, the program will perform the same as if you were to simply run the program. This means that everything is set as their default values and everything is printed.
