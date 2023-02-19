#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
// #include<time.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <utmp.h>
#include <unistd.h>

void display_memory(float *phys_mem, float *virtual_mem, int index, int arr_size, int is_graphics, int is_sequential) {
    // This function prints the "Memory" section
    printf("%s", "---------------------------------------\n");
    printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");
    // Obtain memory information using the sysinfo struct
    struct sysinfo memory;
    sysinfo(&memory);
    for(int i = 0; i < arr_size; i++) {
        // Obtain the required information using the variable 'memory' of type sysinfo
        //  --> "Phys.Used/Tot -- Virtual Used/Tot"
        // Convert Bytes to Gigabytes (G-->GB) : (1000.0 * 1024 * 1024)
        if(i <= index && (is_sequential ? index == i : 1)) {
            printf("%.02f GB / %.02f GB -- %.02f GB / %.02f GB",
            phys_mem[i],
            memory.totalram / (1000.0 * 1024 * 1024) * memory.mem_unit,
            virtual_mem[i],
            (memory.totalswap + memory.totalram) / (1000.0 * 1024 * 1024) * memory.mem_unit);
            
            // Check if flag is '--graphics' or '-g'
            if(is_graphics == 1) {
                printf(" | ");
                if(i > 0) {
                    float difference = virtual_mem[i] - virtual_mem[i-1];
                    int bracket = 1+(int)(abs((int)(difference*100)) + 0.5); // Round up
                    while(--bracket) {
                        // If positive value, add '#', if negative value, add ':'
                        printf((difference >=0) ? "#":":");
                        if(bracket == 1) printf((difference >=0) ? "*":"@");
                    }
                    // Print the difference (virtual memory minus previous virtual memory)
                    printf(" %.02f", (int)(difference*100)/100.0);
                }
            }
        }
        printf("\n");
    }
}

void set_memory(float *phys_mem, float *virtual_mem) {
    // This function sets the physical and virtual memory to use it later in the program
    struct sysinfo memory;
    sysinfo(&memory);
    // Calculate the physical and virtual memory. Then convert it from Bytes to Gigabytes
    *phys_mem = (memory.totalram - memory.freeram) / (1000.0 * 1024 * 1024) * memory.mem_unit;
    *virtual_mem = ((memory.totalram - memory.freeram) + (memory.totalswap - memory.freeswap)) / (1000.0 * 1024 * 1024) * memory.mem_unit;
    // *virtual_mem += (float)(rand() % 10)/100;
}

void memory_used() {
    // This function displays the 'Memory Usage' section
    struct rusage mem_used;
    getrusage(RUSAGE_SELF,&mem_used);
    printf(" Memory usage: %ld kilobytes\n", mem_used.ru_maxrss);
}

void display_session() {
    // This function prints the sessions/users section
    printf("%s", "---------------------------------------\n");
    printf("### Sessions/users ###\n");
    struct utmp session;
    // Read the file "_PATH_UTMP"
    FILE *file_path = fopen(_PATH_UTMP, "r");
    // Read each line of the file and get buffer
    int success = fread(&session, sizeof(session), 1, file_path);
    while(success) {
        // Get users rather than dead processes
        if(session.ut_type == USER_PROCESS) {
            printf("%s\t%s (%s)\n", session.ut_user, session.ut_line, session.ut_host);
        }
        success = fread(&session, sizeof(session), 1, file_path);
    }
    // %s\t%s (%s)
    // marcelo       pts/0 (138.51.12.217)
} 

void display_cpu(float cpu_used[], int index, int arr_size, int is_graphics, int is_sequential) {
    // This function displays the "total cpu use" section
    printf(" total cpu use = %.02f%%\n", cpu_used[index]);   
    
    // If there is a flag for '--graphics' or '-g', print each iteration of cpu_used
    if(is_graphics == 1) {
        for(int i = 0; i < arr_size; i++) {
            if((is_sequential && i == index) || !is_sequential) {
                int line_count = 3;
                if(i <= index) {
                    printf("\t");
                    for(int j = 0; j < line_count+(int)cpu_used[i]; j++) {
                        printf("|");
                    }
                    printf(" %.02f", cpu_used[i]);
                }
            }
            printf("\n");
        }
    }
}

void set_cpu(float *cpu_used) {
    // This function sets the cpu values in the "total cpu use" section
    // Read from file "/proc/stat"
    FILE *file_path = fopen("/proc/stat", "r");

    char line[512];
    fgets(line, 512, file_path);

    // Access cpu info from file
    char *temp;
    unsigned long sum = 0, idle = 0;
    temp = strtok(line, " ");
    for(int i = 0; i < 10; i++) {
        temp = strtok(NULL, " ");
        sum += strtol(temp, NULL, 10);
        // Access idle memory at column 4 (3rd index) and convert it to a long int
        if(i == 3) {
            idle = strtol(temp, NULL, 10);
        }
    }

    *cpu_used =  (float)idle/sum;
    // *cpu_used += (float)(rand()%10);
    // printf(" total cpu use = %.02f%%\n", (float)idle/sum);
}

void display_core(float cpu_used[], int index, int arr_size, int is_graphics, int is_sequential) {
    // This function prints the "Number of cores" section
    int num_of_cores = sysconf(_SC_NPROCESSORS_ONLN);
    printf("%s", "---------------------------------------\n");
    printf("Number of cores: %d\n", num_of_cores);
    // Call the function "display_cpu()"
    display_cpu(cpu_used, index, arr_size, is_graphics, is_sequential);
}

void display_sys_info() {
    // This function prints the "System Information" section
    // Use the 'utsname' struct to access the information
    // --> (using the dot operator to access each field of the struct)
    struct utsname sys_info;
    uname(&sys_info);
    printf("%s", "---------------------------------------\n");
    printf("%s\n", "### System Information ###");
    printf("System Name = %s\n", sys_info.sysname);
    printf("Machine Name = %s\n", sys_info.nodename);
    printf("Version = %s\n", sys_info.version);
    printf("Release = %s\n", sys_info.release);
    printf("Architecture = %s\n", sys_info.machine);
    printf("%s", "---------------------------------------\n");
}

int main (int argc, char **argv) {
    // Declare variables
    int samples = 10;
    int t_delay = 1;
    int is_system = 0, is_user = 0, is_graphics = 0, is_sequential = 0;
    int is_digit1 = 1, is_digit2 = 1;
    
    // Check if all digits of an argument are integers
    if(argc > 1) {
        for(int i = 0; i < strlen(argv[1]); i++) {
            if(!isdigit(argv[1][i])) {
                is_digit1 = 0;
                break;
            }
        }
        if(is_digit1) {
            samples = (atoi(argv[1]));
        }
    }
    if(argc > 2) {
        for(int i = 0; i < strlen(argv[2]); i++) {
            if(!isdigit(argv[2][i])) {
                is_digit2 = 0;
                break;
            }
        }
        if(is_digit1 && is_digit2) {
            samples = (atoi(argv[1]));
            t_delay = (atoi(argv[2]));
        }
    }
    // Iterate through the arguments after 0, since argv[0] is the filename
    for(int i = 1; i < argc; i++) {
        // If the argument is any of the following strings, set the respective variable to 1 (TRUE)
        if(strcmp(argv[i], "--system") == 0) is_system = 1;
        else if(strcmp(argv[i], "--user") == 0) is_user = 1;
        else if(strcmp(argv[i], "--graphics") == 0 || strcmp(argv[i], "-g") == 0) is_graphics = 1;
        else if(strcmp(argv[i], "--sequential") == 0) is_sequential = 1;
        else if(strstr(argv[i], "--samples=")) {
            // Get the number of samples
            samples = strtol(&argv[i][strlen("--samples=")], NULL, 10);
            // if 'samples' is a negative number, set 'samples' to its default value
            if(samples < 0 || strcmp(argv[i], "--samples=") == 0) {
                printf("Invalid samples argument. Resetting to default value.\n");
                samples = 10;
            }
        }
        else if(strstr(argv[i], "--tdelay=")) {
            // Get the number of seconds
            t_delay = strtol(&argv[i][strlen("--tdelay=")], NULL, 10);
            // if t_delay is a negative number, set 't_delay' to its default value
            if(t_delay < 0 || strcmp(argv[i], "--tdelay=") == 0) {
                printf("Invalid tdelay argument. Resetting to default value.\n");
                t_delay = 1;
            }
        }
    }

    float phys_used[samples];
    float virtual_used[samples];
    float cpu_used[samples];
    // srand(time(NULL));
    for(int i = 0; i < samples; i++) {
        printf((is_sequential == 0) ? "\033[2J":""); // Clear screen
        // Print iterations if there's a '--sequential' flag
        if(is_sequential == 1) {
            printf(">>> iteration %d\n", i);
        }
        printf("Nbr of samples %d -- every %d secs\n", samples, t_delay);

        memory_used();
        // If the flags '--user' and '--system' are both inputted
        // in command arguments, display everything
        if(is_user == 0 || (is_user == 1 && is_system == 1)) {
            set_memory(&phys_used[i], &virtual_used[i]);
            display_memory(phys_used, virtual_used, i, samples, is_graphics, is_sequential);
        }
        // Display users when there's no '--system' flag
        if(is_system == 0 || (is_user == 1 && is_system == 1)) {
            display_session();
        }
        if(is_user == 0 || (is_user == 1 && is_system == 1)) {
            set_cpu(&cpu_used[i]);
            display_core(cpu_used, i, samples, is_graphics, is_sequential);
        }

        // Display system information when all 'samples' iterations are done
        if(i == samples - 1) {
            display_sys_info();
        }
        // Delay the 'samples' iterations by t_delay seconds
        sleep(t_delay);
    }
    return 0;
}
