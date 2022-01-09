#!/bin/bash

# usage: ./parallel.sh [curl flags] [number of parallel connections]
# example: time ./parallel.sh -vs 100

for ((i = 1; i <= $2; i++))
do
  curl $1 localhost:8080 &
done
wait