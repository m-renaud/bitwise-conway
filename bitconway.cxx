// Conways game of life using bits instead of

#include <array>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <random>
#include <functional>

#define BINARY_OUTPUT


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace mrr {

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class conway
{
  static const std::size_t BITS = 64;
  static const std::size_t OFFSET = 64;

  using ull = unsigned long long;
  using grid_type = std::array<ull, BITS>;
  using full_grid_type = std::array<grid_type, 2>;
  

  unsigned s;

  full_grid_type grid;
  std::array<unsigned,8> pop3;

  unsigned sum;
  ull mask;
  ull bit[BITS];
  std::mt19937_64 engine;
  std::uniform_int_distribution<ull> dist;
  std::function<ull()> generator;

  
public:
  conway();

  int run();
  void display_as_binary(grid_type const& g);
  void display_as_decimal(grid_type const& g);

private:
  void loop();

  ull calc_next_gen(unsigned sum, unsigned on)
  {
    return (sum == 3) || (sum == 4 && on);
  }

}; // class conway



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
conway::conway()
  : s(0), pop3{{0,1,1,2,1,2,2,3}}, sum(0), mask(0x00),
      engine(std::time(0)), generator(std::bind(dist, engine))
{
  for(std::size_t i = 0; i < BITS; ++i)
  {
    grid[s][i] = generator();
    bit[i] = static_cast<ull>(0x01ULL << i);
  }
}


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int conway::run()
{
#ifdef BINARY_OUTPUT // If displaying the grid
  system("clear");
  std::printf("\e[2J");
#endif

  for(std::size_t i = 0; i < 50000; ++i)
  {
    loop();

#ifdef BINARY_OUTPUT
    std::printf("Generation: %ld\n", i);
#endif
  }

  return 0;

} // int conway::run()



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void conway::loop()
{
  mask = 0x01ull;
  std::size_t i;

  sum = pop3[ grid[s][0] & 3] + pop3[ grid[s][1] & 3];
  grid[!s][0] = calc_next_gen(sum, grid[s][0] & 1);

  sum += pop3[ grid[s][2] & 3];
  grid[!s][1] = calc_next_gen(sum, grid[s][1] & 1);

  for(i = 2; i < 63; ++i)
  {
    sum += pop3[ grid[s][i + 1] & 3] - pop3[ grid[s][i - 2] & 3];
    grid[!s][i] = calc_next_gen(sum, grid[s][i] & 1);
  }

  sum -= pop3[ grid[s][61] & 3];
  grid[!s][63] = calc_next_gen(sum, grid[s][63] & 1);

  while(mask <<= 1)
  {
    sum = pop3[ grid[s][0] & 7] + pop3[ grid[s][1] & 7];
    grid[!s][0] |= calc_next_gen(sum, grid[s][0] & 2) ? mask : 0;

    sum += pop3[ grid[s][2] & 7];
    grid[!s][1] |= calc_next_gen(sum, grid[s][1] & 2) ? mask : 0;

    for(i = 2; i < 63; ++i)
    {
      sum += pop3[ grid[s][i + 1] & 7] - pop3[ grid[s][i - 2] & 7];
      grid[!s][i] |= calc_next_gen(sum, grid[s][i] & 2) ? mask : 0;
      grid[s][i - 2] >>= 1;
    }

    sum -= pop3[ grid[s][61] & 7];
    grid[!s][63] |= calc_next_gen(sum, grid[s][63] & 2) ? mask : 0;
    grid[s][61] >>= 1;
    grid[s][62] >>= 1;
    grid[s][63] >>= 1;
  }

  s = !s;

#ifdef BINARY_OUTPUT
  display_as_binary(grid[s]);
#endif

#ifdef DECIMAL_OUTPUT
  displayAsDecimal(grid[s]);
#endif

} // void conway::loop()



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void conway::display_as_binary(grid_type const& g)
{
  std::printf("\e[H");
  for(std::size_t i = 0; i < BITS; ++i)
  {
    for(int j = static_cast<int>(BITS); j >= 0; --j)
      std::printf("%c", ((g[i] & bit[j] ? 'X' : '.')));
    std::printf("\n");
  }

} // void conway::display_as_binary()



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void conway::display_as_decimal(grid_type const& g)
{
  for(std::size_t i = 0; i < BITS; ++i)
    printf("%Lu", g[i]);

}  // void conway::display_as_decimal()



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


int main()
{
  mrr::conway simulation;

  simulation.run();

  return 0;
}
