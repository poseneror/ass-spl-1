#ifndef FILES_H_
#define FILES_H_

#include <string>
#include <vector>

using namespace std;

class BaseFile {
private:
	string name;
	
public:
	BaseFile(string name);
	string getName() const;
	void setName(string newName);
	virtual int getSize() = 0;
	virtual bool isFile() = 0;

    // destructor
    virtual ~BaseFile();
};

class File : public BaseFile {
private:
	int size;

public:
	File(string name, int size); // Constructor
	int getSize(); // Return the size of the file
	bool isFile();

    ~File();
};

class Directory : public BaseFile {
private:
	vector<BaseFile*> children;
	Directory *parent;

    //rule of five:
    void clear();
    void copy(const Directory &other);

public:
	Directory(string name, Directory *parent); // Constructor
	Directory *getParent() const; // Return a pointer to the parent of this directory
	void setParent(Directory *newParent); // Change the parent of this directory
	void addFile(BaseFile* file); // Add the file to children
	void removeFile(string name); // Remove the file with the specified name from children
	void removeFile(BaseFile* file); // Remove the file from children
	void sortByName(); // Sort children by name alphabetically (not recursively)
	void sortBySize(); // Sort children by size (not recursively)
	void sortBySizeName(); // Sort children by size and than name
	vector<BaseFile*> getChildren(); // Return children
	int getSize(); // Return the size of the directory (recursively)
	string getAbsolutePath();  //Return the path from the root to this


    //rule of five:
    // destructor
    ~Directory();
    // copy
    Directory(const Directory &other);
    // move
    Directory(Directory &&other);
    // copy assignment
    Directory &operator= (const Directory &other);
    // move assignment
    Directory &operator= (Directory &&other);


	bool isFile();
	BaseFile *findChild(string childName);
};

#endif