#! /bin/bash

k=$1
w=$k*1000000
for (( c=0; c<$w; c++ ))
do
   echo "test$c"
done
