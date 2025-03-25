CXX=g++
PREFIX=../bin

all:
	$(CXX) -O2 --std=c++11 -o query_db query_db.cpp -lsqlite3 

install: all
	@if [ ! -d $(PREFIX) ]; then mkdir -p $(PREFIX); fi
	cp query_db $(PREFIX)