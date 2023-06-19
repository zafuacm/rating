generate:
  g++ src/rating.cpp -o src/rating.out -std=c++20 -O3
  python3 src/format.py
  cp rating/*.json web/public/

web:
  cd web && pnpm install && pnpm build

dev:
  cd web && pnpm install && pnpm dev
