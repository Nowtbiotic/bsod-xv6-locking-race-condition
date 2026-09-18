#include "types.h"
#include "stat.h"
#include "user.h"

#define NUM_PROCESSES 4
#define INCREMENTS 5000

int
main(void)
{
  int i;
  int pid;
  int expected;
  int actual;

  counter_reset();

  printf(1, "Starting race condition test...\n");
  printf(1, "Processes: %d\n", NUM_PROCESSES);
  printf(1, "Increments per process: %d\n", INCREMENTS);

  for(i = 0; i < NUM_PROCESSES; i++){
    pid = fork();

    if(pid < 0){
      printf(1, "fork failed\n");
      exit();
    }

    if(pid == 0){
      int j;

      for(j = 0; j < INCREMENTS; j++)
        counter_increment();

      exit();
    }
  }

  for(i = 0; i < NUM_PROCESSES; i++)
    wait();

  expected = NUM_PROCESSES * INCREMENTS;
  actual = counter_get();

  printf(1, "\nExpected counter: %d\n", expected);
  printf(1, "Actual counter:   %d\n", actual);

  if(actual == expected)
    printf(1, "No lost increments observed.\n");
  else
    printf(1, "RACE CONDITION DETECTED! Lost increments: %d\n",
           expected - actual);

  exit();
}
