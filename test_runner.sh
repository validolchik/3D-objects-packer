#!/bin/bash

set -e

g++ random_placement.cpp -o random

n=10
allPrecisions=("0.05" "0.03" "0.02" "0.01" "0.005")

for precision in "${allPrecisions[@]}"; do
  rm -f defaults
  {
    echo "$precision"
    echo "0.8"
    echo "10"
    echo "20"
  } >> defaults
  for (( i = 0; i < n; i++ )); do
      echo "$(date) precision: $precision, run: $i"
      mkdir -p test_results/"$precision"
      ./random models_to_read.txt > test_results/"$precision"/"$i"
      cp plate_matrices/best test_results/"$precision"/"$i"_best
  done
done


