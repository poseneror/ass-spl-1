#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <string>
#include <vector>
#include <iostream>

#include "Files.h"
#include "FileSystem.h"
#include "Commands.h"


using namespace std;

class Environment {
private:
	vector<BaseCommand*> commandsHistory;
	FileSystem fs;

	void copy(const Environment &other);
	void clear();
public:
	Environment();
	void start();
	FileSystem& getFileSystem(); // Get a reference to the file system
	void addToHistory(BaseCommand *command); // Add a new command to the history
	const vector<BaseCommand*>& getHistory() const; // Return a reference to the history of commands

	//rule of five:
	// destructor
	~Environment();
	// copy
	Environment(const Environment &other);
	// move
	Environment(Environment &&other);
	// copy assignment
	Environment &operator= (const Environment &other);
	// move assignment
	Environment &operator= (Environment &&other);

};

#endif