# System Programming Lab 11 Multiprocessing

* **Name:** Lincoln Jiang
* **Repo:** https://github.com/MSOE-CPE2600/multiprocessing-turney-LincolnBWJ.git
* **Branch:** labWeek11dev



### Part 1: Implementation Overview

To make the movie, I wrote a new program called `mandelmovie.c`.

* This program uses `getopt` to read the `-n` number from the command line.
* It uses a `while` loop to run `fork()` `TOTAL_FRAMES` (50) times.
* The child process uses `execv` to run the `./mandel` program with the correct zoom settings for each frame.
* The parent process uses `wait()` to make sure that no more than `-n` children are running at the same time.
* I also updated the `Makefile` to build `mandelmovie` along with the original `mandel` program.


### Part 2: Runtime Graph

This graph shows the time it took to make 50 frames with different `-n` values.

![Mandelmovie Runtime Graph](graph.png)

*(Data collected using the `time` command for 1, 2, 5, 10, and 20 processes)*


### Part 3: Results Discussion

The results from the graph are clear.

* Using more processes makes the program much faster. Going from 1 process (106.88s) to 10 processes (17.78s) saved a lot of time.
* But, the speed increase stopped after 10 processes. The time for 10 processes (17.78s) is almost the same as 20 processes (15.77s).
* **Conclusion**: Using more processes helps, but only up to a point. After about 10 processes, it doesn't get much faster. This is probably because my computer ran out of CPU cores to use.



# Lab 12 - Multithreading

### Part 4: Implementation Overview

I modified the original `mandel.c` program to support multithreading.
* I added a new `-t <threads>` command line option to `mandel.c` using `getopt` and added validation for 1-20 threads.
* I created a `thread_args_t` struct to pass all necessary data (like `xmin`, `xmax`, and the image pointer) to each thread.
* I created a new worker function `compute_image_thread` that contains the old `for` loops. It calculates the `j_start` and `j_end` rows that each specific thread is responsible for.
* The original `compute_image` function was changed to be a "thread manager." It creates an array of `pthread_t` and `thread_args_t`, launches all the threads with `pthread_create`, and then waits for them all to finish with `pthread_join`.
* Finally, I modified `mandelmovie.c` to also accept a `-t <threads>` argument, which it then passes down to every child `execv` call.

### Part 5: Runtime Table

Here is the runtime data for generating 50 frames with different combinations of processes (`-n`) and threads (`-t`).

| | `-t 1` (1 Thread) | `-t 2` (2 Threads) | `-t 5` (5 Threads) | `-t 10` (10 Threads) | `-t 20` (20 Threads) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **`-n 1` (1 Process)** | `1m46.119s` | `1m02.870s` | `0m35.380s` | `0m22.785s` | `0m19.215s` |
| **`-n 2` (2 Processes)** | `0m56.509s` | `0m39.623s` | `0m27.997s` | `0m22.079s` | `0m19.554s` |
| **`-n 5` (5 Processes)** | `0m33.149s` | `0m22.818s` | `0m20.178s` | `0m18.927s` | `0m17.898s` |
| **`-n 10` (10 Processes)**| `0m20.848s` | `0m20.333s` | `0m20.081s` | `0m18.376s` | `0m18.103s` |
| **`-n 20` (20 Processes)**| `0m19.847s` | `0m18.995s` | `0m18.655s` | `0m18.572s` | `0m17.592s` |


### Part 6: Results Discussion

*(Answers to PDF questions)*

* **i. Which technique impacted runtime more - multithreading or multiprocessing?**
    Looking at the table, **multithreading** (going from `-t 1` to `-t 10`) had a bigger impact than **multiprocessing** (going from `-n 1` to `-n 10`). For example, `-n 1 -t 10` (`0m22s`) is much faster than `-n 10 -t 1` (`0m20s`) compared to the baseline `1m46s`.
    * **Why?** I think this is because creating threads is "cheaper" and faster for the computer than creating brand new processes. Processes need their own memory, while threads can share memory, so the overhead is lower.

* **ii. Was there a "sweet spot" for optimal runtime?**
    Yes. The fastest time in the entire table was **`0m17.592s`** at **`-n 20, -t 20`**.
    * However, the speed increase almost stopped long before that. The "sweet spot" (best value for the cost) seems to be around **`-n 5, -t 20`** (`0m17.898s`) or **`-n 10, -t 10`** (`0m18.376s`).
    * Once you get past 10 processes and 10 threads, the speed gain is tiny (less than 1-2 seconds), showing that my computer was at its limit.