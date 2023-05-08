# pipex - a 42 project

## Description
This project is about multithreading. There is a table of n philosophers (1 philospher = 1 thread) that share a common routine (eating, sleeping, thinking). 
Each of them has one fork, but in order to eat they need to hold on to two forks. Communication between philosophers is not allowed.
The main challenges are handling shared resources among the threads and avoiding data races. 

Read [philosphers.pdf](https://github.com/leonyannick/philosophers/blob/main/philosophers.pdf) for the detailed project instruction. </br>

## Installation
Compile executable with executing `make`.

## Usage
`./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> <[number_of_times_each_philosopher_must_eat]>`
