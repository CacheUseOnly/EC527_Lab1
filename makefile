test_combined2d:
	gcc -O1 test_combine2d.c -lrt -o test_combine2d
test_transpose:
	gcc -O1 test_transpose.c -lrt -o test_transpose
test_mmm_block:
	gcc -O1 test_mmm_block.c -lrt -lm -o test_mmm_block
test_mmm_inter:
	gcc -O1 test_mmm_inter.c -lrt -o test_mmm_inter
