#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int left_pipe[2]) __attribute__((noreturn));

void primes(int left_pipe[2]) {
  int prime;

  if (read(left_pipe[0], &prime, sizeof(prime)) == 0) {
    close(left_pipe[0]);
    exit(0);
  }

  printf("prime %d\n", prime);

  int num;
  int right_pipe[2];

  if (pipe(right_pipe) < 0) {
    fprintf(2, "pipe error\n");
    exit(1);
  }

  if (fork() == 0) {
    close(left_pipe[0]);
    close(right_pipe[1]);
    primes(right_pipe);
  } else {
    close(right_pipe[0]);
    while (read(left_pipe[0], &num, sizeof(num)) > 0) {
      if (num % prime != 0) {
        write(right_pipe[1], &num, sizeof(num));
      }
    }

    close(left_pipe[0]);
    close(right_pipe[1]);
    wait(0);
    exit(0);
  }
}

int main(int argc, char *argv[]) {
  int p[2];

  if (pipe(p) < 0) {
    fprintf(2, "pipe error\n");
    exit(1);
  }

  if (fork() == 0) {
    close(p[1]);
    primes(p);
  } else {
    close(p[0]);
    for (int i = 2; i <= 280; i++) {
      write(p[1], &i, sizeof(i));
    }
    close(p[1]);
    wait(0);
    exit(0);
  }
}