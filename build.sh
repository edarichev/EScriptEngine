#!/bin/sh

cd EScript
if [ ! -d lib ]
then
	mkdir lib
fi

echo "OBJS = \\" > obj.d
ls -1v src/*.cpp | sed -r 's/.cpp/.o \\/g' >> obj.d

g++ -I./ -MM src/*.cpp > dep.d

make all
rm obj.d
rm dep.d
make clean

cd ..



