CXX=g++
PREFIX=..

all:
	$(CXX) -O2 --std=c++11 -o query_db query_db.cpp -lsqlite3 

install: all
	@if [ ! -d $(PREFIX)/bin ]; then mkdir -p $(PREFIX)/bin; fi
	@if [ ! -d $(PREFIX)/script ]; then mkdir -p $(PREFIX)/script; fi
	cp query_db $(PREFIX)/bin
	cp script/create_db.py $(PREFIX)/script
	chmod +x $(PREFIX)/script/create_db.py