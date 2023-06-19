build:
	g++ src/rating.cpp -o src/rating.out -std=c++20 -O3

run:
   python3 src/format.py
   cp rating/*.json web/public/
