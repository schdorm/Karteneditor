#!/bin/bash
echo ---enter src/---
cd src
echo ---qmake---
qmake ke.pro
echo ---make---
make
echo ---leave src/---
cd ..
echo ---run programm---
./bin/KE 3>&1 >&2 2>&3 3>&- |tee log.txt

