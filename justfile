build:
	g++ src/rating.cpp -o src/rating.out -std=c++20 -O3

run: 
   ./src/rating.out rating/0000-00-00.txt contest/2022-12-26.txt rating/2022-12-26.txt 
   ./src/rating.out rating/2022-12-26.txt contest/2022-12-30.txt rating/2022-12-30.txt 
   ./src/rating.out rating/2022-12-30.txt contest/2023-01-04.txt rating/2023-01-04.txt 
   ./src/rating.out rating/2023-01-04.txt contest/2023-01-11.txt rating/2023-01-11.txt 
   ./src/rating.out rating/2023-01-11.txt contest/2023-02-26.txt rating/2023-02-26.txt 
   ./src/rating.out rating/2023-02-26.txt contest/2023-05-07.txt rating/2023-05-07.txt 
   ./src/rating.out rating/2023-05-07.txt contest/2023-05-13.txt rating/2023-05-13.txt 
   ./src/rating.out rating/2023-05-13.txt contest/2023-05-21.txt rating/2023-05-21.txt 
   ./src/rating.out rating/2023-05-21.txt contest/2023-05-27.txt rating/2023-05-27.txt 
