#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "Files.h"


class FileSystem {
private:
	Directory* rootDirectory;
	Directory* workingDirectory;

	void copy(const FileSystem &other);
	void clear();
public:
	FileSystem();
	Directory& getRootDirectory() const; // Return reference to the root directory
	Directory& getWorkingDirectory() const; // Return reference to the working directory
	void setWorkingDirectory(Directory *newWorkingDirectory); // Change the working directory of the file system

	//rule of five:
	// destructor
	~FileSystem();
	// copy
	FileSystem(const FileSystem &other);
	// move
	FileSystem(FileSystem &&other);
	// copy assignment
	FileSystem &operator= (const FileSystem &other);
	// move assignment
	FileSystem &operator= (FileSystem &&other);
};


#endif
