all:
	g++ -std=c++11 main.cpp -o main
test: all
	echo '> 1 0 0 0 2 6, exp: S5' > test.out
	echo '1 0 0 0 2 6' | ./main >> test.out
	echo '' >> test.out
	echo '> 1 0 0 0 0 -2, exp: GA(1,5)' >> test.out
	echo '1 0 0 0 0 2' | ./main >> test.out
	echo '' >> test.out
	echo '> 1 0 0 0 -2 0, exp: D4' >> test.out
	echo '1 0 0 0 -2 0' | ./main >> test.out
	echo '' >> test.out
	echo '>1 0 0 -2 0 0, exp: S3' >> test.out
	echo '1 0 0 -2 0 0' | ./main >> test.out
	echo '' >> test.out
	echo '> 1 0 2 0 2 0, exp: D4' >> test.out
	echo '1 0 2 0 2 0' | ./main >> test.out
clean:
	rm -f main
