#!/bin/bash

for ((i = 2; i <= 12; i++))
do
    mv ./samples/big_samples/$i.samp ./tests/end_to_end_tests/my_input_dat/$((i + 3)).dat
done
