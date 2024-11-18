#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf(2, "Usage: sleep <number of ticks>\n");
    exit();
  }

  int ticks = atoi(argv[1]);
  sleep(ticks);
  exit();
}