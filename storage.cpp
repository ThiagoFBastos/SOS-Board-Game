#include "storage.hpp"
#include <cstring>
#include <cerrno>
#include <iostream>
#include <cassert>
#include <cstdlib>

Storage :: Storage(const char* filename) {
	checkStorage(filename);
	fs.open(filename, std :: ios_base :: binary | std :: ios_base :: in | std :: ios_base :: out);
	assert(fs.is_open());
}

Storage :: ~Storage() {
	close();
}

void Storage :: close() {
	if(fs.is_open()) fs.close();
}

void Storage :: checkStorage(const char* filename) {
	std :: ifstream ifs(filename, std :: ios_base :: binary);

	if(ifs.is_open()) {
		ifs.close();
		return;
	}

	std :: ofstream ofs(filename, std :: ios_base :: binary);

	bool player {};
	int table[8][8], p1 {}, p2{}, first {};

	memset(table, -1, sizeof table);

	ofs.write((const char*)&player, 1);
	ofs.write((const char*)&table[0][0], 4 * 64);
	ofs.write((const char*)&p1, 4);
	ofs.write((const char*)&p2, 4);
	ofs.write((const char*)&first, 4);

	ofs.close();
}

bool Storage :: getEnemy() {
	bool enemy;
	fs.seekg(0, std :: ios_base :: beg);
	fs.read((char*)&enemy, 1);
	return enemy;
}

void Storage :: saveEnemy(bool enemy) {
	fs.seekg(0, std :: ios_base :: beg);
	fs.write((const char*)&enemy, 1);
}

void Storage :: getTable(int table[8][8]) {
	fs.seekg(1, std :: ios_base :: beg);
	fs.read((char*)&table[0][0], 4 * 64);
}

void Storage :: saveTable(int table[8][8]) {
	fs.seekg(1, std :: ios_base :: beg);
	fs.write((const char*)&table[0][0], 4 * 64);
}

void Storage :: getScore(int& p1, int& p2) {
	unsigned bytes = 1 + 4 * 64;
	fs.seekg(bytes, std :: ios_base :: beg);
	fs.read((char*)&p1, 4);
	fs.read((char*)&p2, 4);
}

void Storage :: saveScore(int p1, int p2) {
	unsigned bytes = 1 + 4 * 64;
	fs.seekg(bytes, std :: ios_base :: beg);
	fs.write((const char*)&p1, 4);
	fs.write((const char*)&p2, 4);
}

int Storage :: getFirst() {
	unsigned bytes = 1 + 4 * 64 + 2 * 4;
	int first;
	fs.seekg(bytes, std :: ios_base :: beg);
	fs.read((char*)&first, 4);
	return first;
}

void Storage :: saveFirst(int first) {
	unsigned bytes = 1 + 4 * 64 + 2 * 4;
	fs.seekg(bytes, std :: ios_base :: beg);
	fs.write((const char*)&first, 4);
}
