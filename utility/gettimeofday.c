#include <stdio.h>
#include <sys/time.h>

double get_elapsed_time(struct timeval *begin, struct timeval *end) {
  return (end->tv_sec - begin->tv_sec) * 1000
  + (end->tv_usec - begin->tv_usec) / 1000.0;
}

int main(void) {
  struct timeval t1, t2;
  gettimeofday(&t1, NULL);
  計算 (*)
  gettimeofday(&t2, NULL);
  プログラム (*) の部分の実行時間 = get_elapsed_time(&t1, &t2); /* ミリ秒 */
  return 0;
}