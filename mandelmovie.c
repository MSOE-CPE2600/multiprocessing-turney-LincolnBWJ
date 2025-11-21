/**
 * File: mandelmovie.c
 * Modified by: Lincoln Jiang
 * 
 * Brief summary of program:
 * uses -n child processes to generate 50 mandelbrot frames.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define TOTAL_FRAMES 50

// this is the spot we'll "fly" into
#define X_CENTER "0.286932"
#define Y_CENTER "0.014287"

int main(int argc, char* argv[]) {

    int num_processes = 1; // default
    int c;

    // parse -n arg
    while ((c = getopt(argc, argv, "n:")) != -1) {
        switch (c) {
            case 'n':
                num_processes = atoi(optarg);
                break;
            case '?': 
                fprintf(stderr, "Usage: %s -n <num_processes>\n", argv[0]);
                return 1;
        }
    }

    if (num_processes < 1 || num_processes > TOTAL_FRAMES) {
        fprintf(stderr, "Error: Number of processes must be between 1 and %d\n", TOTAL_FRAMES);
        return 1;
    }

    printf("Generating %d frames using %d concurrent processes...\n", TOTAL_FRAMES, num_processes);

    double start_scale = 1.0;
    double end_scale = 0.0005;
    double scale_step = (start_scale - end_scale) / (TOTAL_FRAMES - 1);

    int running_jobs = 0;
    int frame_index = 0; // next frame to start
    int frames_done = 0; // frames completed

    // loop til all frames are done
    while (frames_done < TOTAL_FRAMES) {

        while (running_jobs < num_processes && frame_index < TOTAL_FRAMES) {
            
            double current_scale = start_scale - (scale_step * frame_index);
            char scale_str[50];
            char file_str[50];

            sprintf(scale_str, "%f", current_scale);
            sprintf(file_str, "mandel%d.jpg", frame_index);

            pid_t pid = fork();

            if (pid == 0) { 
                
                // args for ./mandel
                char* args[] = {
                    "./mandel",
                    "-x", X_CENTER,
                    "-y", Y_CENTER,
                    "-s", scale_str,
                    "-o", file_str,
                    "-W", "800",   
                    "-H", "800",
                    NULL
                };

                execv("./mandel", args);

                perror("execv failed");
                exit(1);
            
            } else if (pid > 0) {
                running_jobs++;
                frame_index++;
            
            } else { 
                perror("fork failed");
                break; 
            }
        }

        pid_t finished_pid = wait(NULL); 
        
        if (finished_pid > 0) {
            running_jobs--;
            frames_done++;
            printf("Finished frame. Total done: %d/%d\n", frames_done, TOTAL_FRAMES);
        }
    }

    printf("\n--- All %d frames generated. ---\n", TOTAL_FRAMES);
    printf("To create movie, run:\n");
    printf("ffmpeg -r 10 -i mandel%%d.jpg mandel.mpg\n");
    
    return 0;
}