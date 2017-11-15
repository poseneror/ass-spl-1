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

public:
	Environment();
	void start();
	FileSystem& getFileSystem(); // Get a reference to the file system
	void addToHistory(BaseCommand *command); // Add a new command to the history
	const vector<BaseCommand*>& getHistory() const; // Return a reference to the history of commands
};

#endif