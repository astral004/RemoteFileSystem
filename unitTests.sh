#!/bin/sh

echo "./fget INFO test1.txt"
./fget INFO test1.txt

echo "\nNotice, testedGet.txt does not yet exist."
ls -al
printf "\n./fget GET test1.txt testedGet.txt\n"
./fget GET test1.txt testedGet.txt
ls -al

echo "\n./fget PUT testedGet.txt testedPut.txt"
./fget PUT testedGet.txt testedPut.txt
ls -al /Volumes/WG_STUDENT

echo "\n./fget MD testedMD"
./fget MD testedMD
ls -al /Volumes/WG_STUDENT

echo "\n./fget RM testedPut.txt"
./fget RM testedPut.txt
ls -al /Volumes/WG_STUDENT

echo "\nTesting single location GET and PUT"

echo "\nTesting begins now"
echo "\n./fget GET tmpFile.txt"
./fget GET tmpFile.txt


echo "\n./fget PUT singlePut.txt"
./fget PUT singlePut.txt
ls -al /Volumes/WG_STUDENT

./fget EXIT