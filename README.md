# EC527 Lab1
Chenliang Wang\
edwangd@bu.edu\
Yuxuan Luo\
luoyx@bu.edu

## Part 1

### 1a.

**1048576** bytes of memory give the highest bandwidth. \
Besides, **262144** bytes gives a very similiar bandwidth.

### 1b.

**67108864** gives the lowest.  \
The ratio between the highest and the lowest is 1.19 

### 1c.

16 Tasks with 262144 Bytes give the highest total bandwidth. \
16 Tasks with 1048576 Bytes gives a similliar bandwidth.

### 1d.

The bandwidth achieved when running just one task with largest memory size is 3.069e+10. 
The bandwidth per task achieved closest to this bandwidth when running multiple tasks is **1.81e+10**.

## Part2
### 2a.
- Around the size of 50000 x 50000, the test starts to run incredibly slow. 
- At first it Combine2D() is 7.5% faster than Combine2D_Rev(), after the row length passes 1800, the difference becomes more than 50%.
- Starting from row_len 1164, the ratio drastically increased.

### 2b.
cycle time: 3.33*e-10
|   | CPE for range 100-442 | CPE for range 10000-10342 |
|  ----  | ----  | ----  |
| Combine2D | 5.44e+09 - 5.14e+09 | 5.44e+09 - 5.45e+09  |
| Combine2D_rev  | 5.36e+09 - 5.13e+09 | 9.64e+09 - 8.51e+09  |
> **TODO**: Applying your knowledge of the computer, interpret your observations here.
### 2c.
The U shape starts at the very beginning of two charts. 
Here is the zoom-in from 0 to 2000 row length.
>TODO: Add picture 2c
> Add explaination.
## Part3

### **3a** `ijk`

  - How many plateaus are there?

    2

  - For each plateau, what is the number of cycles per innermost loop iteration (as defined above)?

    First is around $7.6$; the second one is around $12.7$ 

  - Between plateaus are the "transitions". Expressing your answer in terms of the matrix sizes (horizontal axis), where do the transitions occur?

    The matrix size is around $1358^2 = 1844164$ 
    
### **3b** `jki`

  - How many plateaus are there?

    2

  - For each plateau, what is the number of cycles per innermost loop iteration (as defined above)?

    First is around $8.39$; the second one is around $23.26$ 

  - Between plateaus are the "transitions". Expressing your answer in terms of the matrix sizes (horizontal axis), where do the transitions occur?

    The matrix size is around $1806^2$ 
### **3c** `kij`

  - How many plateaus are there?

    2

  - For each plateau, what is the number of cycles per innermost loop iteration (as defined above)?

    First is around $6.0$; the second one is around $7.0$ 

  - Between plateaus are the "transitions". Expressing your answer in terms of the matrix sizes (horizontal axis), where do the transitions occur?

    The matrix size is around $1256^2$ 

## Part 4

### a

**The benefit of blocking as a function of matrix size**

$HitRate(n) = \frac{2n^3}{\text{cache block size}\times \text{block size}}$

### b

**The optimal block size as a function of matrix size.**

According to the graph, it should be `16`

