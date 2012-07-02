// Conways game of life using bits.

#include <cstdlib>
#include <time.h>
#include <stdio.h>

typedef unsigned long long ull;
enum { BITS = 64 };

ull grid[2][BITS];
unsigned s = 0;
unsigned pop3 [8] = {0,1,1,2,1,2,2,3};
unsigned sum = 0;
ull mask = 0x00;
ull bit[BITS];


inline unsigned long long calcNextGen(unsigned sum, unsigned on)
{
  return static_cast<unsigned long long>((sum == 3) || (sum == 4 && on));
}

void initialize()
{
  srand( time(NULL) );

  for(unsigned i = 0; i < BITS; ++i)
  {
    grid[s][i] = rand();
    grid[s][i] <<= 32;
    grid[s][i] += rand();
    bit[i] = static_cast<unsigned long long>(0x01ULL << i);
  }
}

void displayAsBinary(ull g[])
{
  printf("\e[H");
  for(unsigned i = 0; i < BITS; ++i)
  {
    for(int j = BITS; j >= 0; --j)
      printf("%c", ((g[i] & bit[j] ? 'X' : '.')));
    printf("\n");
  }
}

void loop()
{
  mask = 0x01ull;
  unsigned i;

  sum = pop3[ grid[s][0] & 3] + pop3[ grid[s][1] & 3];
  grid[!s][0] = calcNextGen(sum, grid[s][0] & 1);

  sum += pop3[ grid[s][2] & 3];
  grid[!s][1] = calcNextGen(sum, grid[s][1] & 1);

  for(i = 2; i < 63; ++i)
  {
    sum += pop3[ grid[s][i + 1] & 3] - pop3[ grid[s][i - 2] & 3];
    grid[!s][i] = calcNextGen(sum, grid[s][i] & 1);
  }

  sum -= pop3[ grid[s][61] & 3];
  grid[!s][63] = calcNextGen(sum, grid[s][63] & 1);

  while(mask <<= 1)
  {
    sum = 0;
    sum = pop3[ grid[s][0] & 7] + pop3[ grid[s][1] & 7];
    grid[!s][0] |= calcNextGen(sum, grid[s][0] & 2) ? mask : 0ull;

    sum += pop3[ grid[s][2] & 7];
    grid[!s][1] |= calcNextGen(sum, grid[s][1] & 2) ? mask : 0ull;

    for(i = 2; i < 63; ++i)
    {
      sum += pop3[ grid[s][i + 1] & 7] - pop3[ grid[s][i - 2] & 7];
      grid[!s][i] |= calcNextGen(sum, grid[s][i] & 2) ? mask : 0ull;
      grid[s][i - 2] >>= 1;
    }

    sum -= pop3[ grid[s][61] & 7];
    grid[!s][63] |= calcNextGen(sum, grid[s][63] & 2) ? mask : 0ull;
    grid[s][61] >>= 1;
    grid[s][62] >>= 1;
    grid[s][63] >>= 1;
  }

  s = !s;
  displayAsBinary(grid[s]);
}

void run()
{
  printf("\e[2J");

  for(unsigned i = 0; i < 50000; ++i)
  {
    loop();
//    usleep(700000);
    printf("Generation: %d\n", i);
  }
}

int main()
{
  initialize();
  run();
  return 0;
}


