#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <fstream>

class Storage {
	std :: fstream fs;

	void checkStorage(const char*);

	public:
	
	Storage(const char*);
	~Storage();
	
	void close();
	
	bool getEnemy();
	void getTable(int[8][8]);
	void getScore(int&, int&);
	int getFirst();

	void saveEnemy(bool);
	void saveTable(int[8][8]);
	void saveScore(int, int);
	void saveFirst(int);
};

#endif
