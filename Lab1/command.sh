#!/bin/sh

uname -v > MyKernal             # a 
touch MyFile                    # b
mkdir MyDirectory               # c
cp MyFile MyDirectory           # d
cd MyDirectory                  # e
mv MyFile MyCopy                # e
nvim MyCopy                     # f 
cd ..                           # g
rm -rf MyDirectory              # g
echo Helloredirection! > file   # h
