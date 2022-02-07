/*****************************************************************************/
// gcc -O1 test_mmm_block.c -lrt -lm -o test_mmm_block

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define CPNS 3.0    /* Cycles per nanosecond -- Adjust to your computer,
                       for example a 3.2 GhZ GPU, this would be 3.2 */

/* We want to test a wide range of work sizes. We will generate these
   using the quadratic formula:  A x^2 + B x + C                     */
#define A   2  /* coefficient of x^2 */
#define B   4  /* coefficient of x */
#define C   8  /* constant term */

#define NUM_TESTS 15 /* Number of different sizes to test */
#define BSIZE_START 3 /* exponential of block size */
#define BSIZE_TESTS 4 /* Number of different block size to test */

#define IDENT 0

typedef double data_t;

/* Create abstract data type for matrix */
typedef struct {
  long int len;
  data_t *data;
} matrix_rec, *matrix_ptr;

/* Prototypes */
int clock_gettime(clockid_t clk_id, struct timespec *tp);
matrix_ptr new_matrix(long int row_len);
int set_matrix_row_length(matrix_ptr m, long int row_len);
long int get_matrix_row_length(matrix_ptr m);
int init_matrix(matrix_ptr m, long int row_len);
int zero_matrix(matrix_ptr m, long int row_len);
void mmm_ijk(matrix_ptr a, matrix_ptr b, matrix_ptr c);
void bijk(matrix_ptr a, matrix_ptr b, matrix_ptr c, int bsize);
double pow(double x, double y);

/* -=-=-=-=- Time measurement by clock_gettime() -=-=-=-=- */
/*
  As described in the clock_gettime manpage (type "man clock_gettime" at the
  shell prompt), a "timespec" is a structure that looks like this:
 
        struct timespec {
          time_t   tv_sec;   // seconds
          long     tv_nsec;  // and nanoseconds
        };
 */

double interval(struct timespec start, struct timespec end)
{
  struct timespec temp;
  temp.tv_sec = end.tv_sec - start.tv_sec;
  temp.tv_nsec = end.tv_nsec - start.tv_nsec;
  if (temp.tv_nsec < 0) {
    temp.tv_sec = temp.tv_sec - 1;
    temp.tv_nsec = temp.tv_nsec + 1000000000;
  }
  return (((double)temp.tv_sec) + ((double)temp.tv_nsec)*1.0e-9);
}
/*
     This method does not require adjusting a #define constant

  How to use this method:

      struct timespec time_start, time_stop;
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_start);
      // DO SOMETHING THAT TAKES TIME
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stop);
      measurement = interval(time_start, time_stop);

 */


/* -=-=-=-=- End of time measurement declarations =-=-=-=- */

/* This routine "wastes" a little time to make sure the machine gets
   out of power-saving mode (800 MHz) and switches to normal speed. */
double wakeup_delay()
{
  double meas = 0; int i, j;
  struct timespec time_start, time_stop;
  double quasi_random = 0;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_start);
  j = 100;
  while (meas < 1.0) {
    for (i=1; i<j; i++) {
      /* This iterative calculation uses a chaotic map function, specifically
         the complex quadratic map (as in Julia and Mandelbrot sets), which is
         unpredictable enough to prevent compiler optimisation. */
      quasi_random = quasi_random*quasi_random - 1.923432;
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stop);
    meas = interval(time_start, time_stop);
    j *= 2; /* Twice as much delay next time, until we've taken 1 second */
  }
  return quasi_random;
}

/*****************************************************************************/
int main(int argc, char *argv[])
{
  struct timespec time_start, time_stop;
  double time_stamp[BSIZE_TESTS][NUM_TESTS];
  double wakeup_answer;
  long int x, n, alloc_size;
  int block_size;

  x = NUM_TESTS-1;
  alloc_size = A*x*x + B*x + C;
  /* use this line if you want 2^x */
  // alloc_size = pow(2, x);

  printf("Block MMM tests \n\n");

  wakeup_answer = wakeup_delay();

  printf("for %d different matrix sizes from %d to %d\n",
                                                     NUM_TESTS, C, alloc_size);
  printf("This may take a while!\n\n");

  /* declare and initialize the matrix structure */
  matrix_ptr a0 = new_matrix(alloc_size);
  init_matrix(a0, alloc_size);
  matrix_ptr b0 = new_matrix(alloc_size);
  init_matrix(b0, alloc_size);
  matrix_ptr c0 = new_matrix(alloc_size);
  zero_matrix(c0, alloc_size);

  int index = 0;
  for (block_size = BSIZE_START; block_size < BSIZE_START + BSIZE_TESTS; ++block_size) {
	  int size = (int) pow(2, block_size);
	  for (x=0; x<NUM_TESTS && (n = A*x*x + B*x + C, n<=alloc_size); x++) {
	    printf(" Block MMM, iter %ld, size %ld, block size %d\n", x, n, size);
	    set_matrix_row_length(a0, n);
	    set_matrix_row_length(b0, n);
	    set_matrix_row_length(c0, n);
	    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_start);
	    bijk(a0, b0, c0, size);
	    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_stop);
	    printf("time: %ld\n", (long int) ((double)(CPNS) * 1.0e9 * interval(time_start, time_stop)));
	    time_stamp[index][x] = interval(time_start, time_stop);
	  }
	  printf("\n");
	  ++index;
  }

  printf("Done collecting measurements.\n\n");

  printf("row_len, bijk\n");
  
	int i, j;
	for (i = 0; i < NUM_TESTS; i++) {
		printf("%ld, ", A*i*i + B*i + C);
		// printf("%d, ", (int) pow(2, i));
		for (j = 0; j < BSIZE_TESTS; ++j) {
			if (j != 0) {
			  printf(", ");
			}
			printf("%ld", (long int) ((double)(CPNS) * 1.0e9 * time_stamp[j][i]));
		}
		printf("\n");
	}
  
  printf("\n");

  printf("Wakeup delay computed: %g \n", wakeup_answer);
} /* end main */

/**********************************************/

/* Create matrix of specified length */
matrix_ptr new_matrix(long int row_len)
{
  long int i;
  long int alloc;

  /* Allocate and declare header structure */
  matrix_ptr result = (matrix_ptr) malloc(sizeof(matrix_rec));
  if (!result) return NULL;  /* Couldn't allocate storage */
  result->len = row_len;

  /* Allocate and declare array */
  if (row_len > 0) {
    alloc = row_len * row_len;
    data_t *data = (data_t *) calloc(alloc, sizeof(data_t));
    if (!data) {
	  free((void *) result);
	  printf("\n COULDN'T ALLOCATE %ld BYTES STORAGE \n",
                                                       alloc * sizeof(data_t));
	  return NULL;  /* Couldn't allocate storage */
	}
	result->data = data;
  } else {
    result->data = NULL;
  }

  return result;
}

/* Set length of matrix */
int set_matrix_row_length(matrix_ptr m, long int row_len)
{
  m->len = row_len;
  return 1;
}

/* Return length of matrix */
long int get_matrix_row_length(matrix_ptr m)
{
  return m->len;
}

/* initialize matrix */
int init_matrix(matrix_ptr m, long int row_len)
{
  long int i;

  if (row_len > 0) {
    m->len = row_len;
    for (i = 0; i < row_len*row_len; i++) {
      m->data[i] = (data_t)(i);
    }
    return 1;
  }
  else return 0;
}

/* initialize matrix */
int zero_matrix(matrix_ptr m, long int row_len)
{
  long int i,j;

  if (row_len > 0) {
    m->len = row_len;
    for (i = 0; i < row_len*row_len; i++) {
      m->data[i] = (data_t)(IDENT);
    }
    return 1;
  }
  else return 0;
}

data_t *get_matrix_start(matrix_ptr m)
{
  return m->data;
}

/*************************************************/

/* mmm */
void mmm_ijk(matrix_ptr a, matrix_ptr b, matrix_ptr c)
{
  long int i, j, k;
  long int length = get_matrix_row_length(a);
  data_t *a0 = get_matrix_start(a);
  data_t *b0 = get_matrix_start(b);
  data_t *c0 = get_matrix_start(c);
  data_t sum;

  for (i = 0; i < length; i++) {
    for (j = 0; j < length; j++) {
      sum = IDENT;
      for (k = 0; k < length; k++) {
        sum += a0[i*length+k] * b0[k*length+j];
      }
      c0[i*length+j] += sum;
    }
  }
}

void bijk(matrix_ptr a, matrix_ptr b, matrix_ptr c, int bsize) {
  long int i, j, k, kk, jj;
  long int length = get_matrix_row_length(a);
  data_t *a0 = get_matrix_start(a);
  data_t *b0 = get_matrix_start(b);
  data_t *c0 = get_matrix_start(c);
  data_t sum;
	
	for (kk = 0; kk < length; kk += bsize) {
		for (jj = 0; jj < length; jj += bsize) {
			for (i = 0; i < length; ++i) {
				for (j = jj; j < jj+bsize && j < length; ++j) {
					sum = c0[i*length + j];
					for (k = kk; k < kk+bsize && k < length; ++k) {
						sum += a0[i*length + k] + b0[k*length + j];
					}
					c0[i*length + j] = sum;
				}
			}
		}
	}
}

