#include "fatsim.h"
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

namespace {
struct Timer {
  // return elapsed time (in seconds) since last reset/or construction
  // reset_p = true will reset the time
  double elapsed(bool reset_p = false);
  // reset the time to 0
  void reset();
  Timer();
  ~Timer();

  private:
  struct Pimpl;
  Pimpl * pimpl_;
};

struct Timer::Pimpl {
  std::chrono::time_point<std::chrono::steady_clock> start;
};
Timer::Timer()
{
  pimpl_ = new Pimpl;
  reset();
}
Timer::~Timer() { delete pimpl_; }
double Timer::elapsed(bool reset_p)
{
  double result = 1e-6
      * std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now() - pimpl_->start)
            .count();
  if (reset_p) reset();
  return result;
}
void Timer::reset() { pimpl_->start = std::chrono::steady_clock::now(); }
} // anon namespace

int main(int argc, char **)
{
  if (argc != 1) {
    printf("Thanks for all the command line arguments.\n"
           "But I don't like any of them... Bye.\n");
    exit(-1);
  }

  std::vector<long> fat;
  long maxnum = 0;
  while (1) {
    long num;
    if (1 != scanf("%ld", &num)) break;
    if (num < -1) {
      printf("I don't like your FAT, it's too negative.\n");
      exit(-1);
    }
    maxnum = std::max(maxnum, num);
    fat.push_back(num);
  }

  if (long(fat.size()) <= maxnum) {
    printf("Some of your FAT entries are too big.\n");
    exit(-1);
  }

  Timer t;
  std::vector<long> result = fat_check(fat);
  auto elapsed = t.elapsed();

  std::string line = "chain sizes:";
  for( auto n : result) {
    line += " " + std::to_string(n);
    if( line.size() > 80) {
      printf("%s\n", line.c_str());
      line = " ";
    }
  }
  if( line.size())
    printf("%s\n", line.c_str());
  printf("elapsed time: %.3lfs\n", elapsed);

  return 0;
}
