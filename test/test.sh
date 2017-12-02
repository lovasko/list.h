#!/bin/bash

function run_test {
  printf "Running test '${1}' ... "
  RESULT=`./${2}`
  if [[ $? = "0" ]]
  then
    printf "\e[32mOK\e[0m\n"
  else
    printf "\e[31mFAIL\e[0m\nFull output of test '${1}':\n${RESULT}\n"
  fi
}

# Compile the test programs
cc -Wall -Wextra -std=c99 -O3 sort.c -o test_sort

# Run the test programs
run_test "sort" test_sort
