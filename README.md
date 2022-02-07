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

>**TODO**: how many tasks can you run at the same time on this machine, and
get close to the same bandwidth per task as what you get with just one task?

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
### 3a. ijk
- 2 plateaus
- a 
