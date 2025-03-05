#!/bin/bash
echo "Running test: dirwalk should list all files"
./build/debug/dirwalk . -f > test/output.txt
grep "dirwalk.c" test/output.txt && echo "Test passed!" || echo "Test failed!"
