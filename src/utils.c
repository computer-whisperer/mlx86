#include <time.h>
#include "utils.h"
#include "math.h"

double getUnixTime(void)
{
    struct timespec tv;

    if(clock_gettime(CLOCK_REALTIME, &tv) != 0) return 0;

    return (tv.tv_sec + (tv.tv_nsec / 1000000000.0));
}

// fast random number generated yanked from the internet

unsigned int g_seed = 4819;

// Used to seed the generator.           
void seed_fast_rand(int seed) {
    g_seed = seed;
}

// Compute a pseudorandom integer.
// Output value in range [0, 32767]
int fast_rand(void) {
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}

// Compute a pseudorandom integer.
// Output value in range [0, 32767]
int fast_rand_seeded(int m_seed) {
    m_seed = (214013*m_seed+2531011);
    return (m_seed>>16)&0x7FFF;
}

//=========================================================================
//= Multiplicative LCG for generating uniform(0.0, 1.0) random numbers    =
//=   - x_n = 7^5*x_(n-1)mod(2^31 - 1)                                    =
//=   - With x seeded to 1 the 10000th x value should be 1043618065       =
//=   - From R. Jain, "The Art of Computer Systems Performance Analysis," =
//=     John Wiley & Sons, 1991. (Page 443, Figure 26.2)                  =
//=========================================================================
double double_uniform_rand(int seed)
{
  const long  a =      16807;  // Multiplier
  const long  m = 2147483647;  // Modulus
  const long  q =     127773;  // m div a
  const long  r =       2836;  // m mod a
  static long x = 29845;               // Random int value
  long        x_div_q;         // x divided by q
  long        x_mod_q;         // x modulo q
  long        x_new;           // New x value

  // Set the seed if argument is non-zero and then return zero
  if (seed > 0)
  {
    x = seed;
    return(0.0);
  }

  // RNG using integer arithmetic
  x_div_q = x / q;
  x_mod_q = x % q;
  x_new = (a * x_mod_q) - (r * x_div_q);
  if (x_new > 0)
    x = x_new;
  else
    x = x_new + m;

  // Return a random value between 0.0 and 1.0
  return((double) x / m);
}

//===========================================================================
//=  Function to generate normally distributed random variable using the    =
//=  Box-Muller method                                                      =
//=    - Input: mean and standard deviation                                 =
//=    - Output: Returns with normally distributed random variable          =
//===========================================================================
double double_norm_rand(double mean, double std_dev)
{
  double   u, r, theta;           // Variables for Box-Muller method
  double   x;                     // Normal(0, 1) rv
  double   norm_rv;               // The adjusted normal rv

  // Generate u
  u = 0.0;
  while (u == 0.0)
    u = double_uniform_rand(0);

  // Compute r
  r = sqrt(-2.0 * log(u));

  // Generate theta
  theta = 0.0;
  while (theta == 0.0)
    theta = 2.0 * 3.1415926535 * double_uniform_rand(0);

  // Generate x value
  x = r * cos(theta);

  // Adjust x value for specified mean and variance
  norm_rv = (x * std_dev) + mean;

  // Return the normally distributed RV value
  return(norm_rv);
}

// http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

unsigned long hash_with_len(unsigned char *str, size_t len)
{
    unsigned long hash = 5381;
    int c;
    int i;

    for (i = 0; i < len; i++)
    {
    	c = *str++;
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}


// https://martin.ankerl.com/2012/01/25/optimized-approximative-pow-in-c-and-cpp/
double fastPow(double a, double b)
{
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;
  return u.d;
}

// https://stackoverflow.com/questions/3272424/compute-fast-log-base-2-ceiling
int ceil_log2(unsigned long long x)
{
  static const unsigned long long t[6] = {
    0xFFFFFFFF00000000ull,
    0x00000000FFFF0000ull,
    0x000000000000FF00ull,
    0x00000000000000F0ull,
    0x000000000000000Cull,
    0x0000000000000002ull
  };

  int y = (((x & (x - 1)) == 0) ? 0 : 1);
  int j = 32;
  int i;

  for (i = 0; i < 6; i++) {
    int k = (((x & t[i]) == 0) ? 0 : j);
    y += k;
    x >>= k;
    j >>= 1;
  }

  return y;
}

U8 hex_lookup[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
void print_data_as_hex(U8 * data, U32 data_len)
{
	U32 i = 0;
	while (i < data_len)
	{
		putchar(hex_lookup[(data[i]&0xF0)>>4]);
		putchar(hex_lookup[(data[i]&0xF)]);
		i++;
		if (i%0x20 == 0)
		{
			putchar('\n');
		}
	}
	putchar('\n');
}

void print_data_as_hex_10wrap(U8 * data, U32 data_len)
{
	U32 i = 0;
	while (i < data_len)
	{
		putchar(hex_lookup[(data[i]&0xF0)>>4]);
		putchar(hex_lookup[(data[i]&0xF)]);
		i++;
		if (i%10 == 0)
		{
			putchar('\n');
		}
	}
	putchar('\n');
}


static U8 process_offset = 0;
void set_process_offset(U8 offset)
{
	process_offset = offset;
}
U8 get_process_offset()
{
	return process_offset;
}

