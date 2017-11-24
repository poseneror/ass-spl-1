//#include "../include/Environment.h"
//
//// ... You may not change this file
//
//unsigned int verbose = 0;
//
//int main(int , char **) {
////    FileSystem *fs = new FileSystem();
////    fs -> getWorkingDirectory().addFile(new Directory("dir1", &fs -> getRootDirectory()));
////    ((Directory *) fs -> getWorkingDirectory().findChild("dir1"))->addFile(new Directory("dir2", (Directory *) fs -> getWorkingDirectory().findChild("dir1")));
////    fs -> setWorkingDirectory((Directory*)((Directory *)fs -> getWorkingDirectory().findChild("dir1"))->findChild("dir2"));
////    FileSystem *fs1 = new FileSystem(*fs);
//	Environment env;
////    Environment env1 = *new Environment(env);
//	env.start();
//	return 0;
//}
#include "../include/Environment.h"
#include <iostream>
using namespace std;

unsigned int verbose = 0;

int main() {
	cout << "Please note that when one test fails, all the "
			"rest \nwill fail so if you get an error, start with the first number that appears." << endl;
	cout << "I recommend creating a makefile for this test\n"
			"and using it in valgrind to make there there are no memory leaks." << endl;
	cout << "If you're getting anything but exit code 0 in the end, it means that there's a segmentation fault." << endl;
	cout << "Crucial assumption here is that addfile updates the parent of the added file if it's a directory." << endl;
	//Creating framework for the tests.
	Directory *dir1 = new Directory("dir1", nullptr);
	Directory *dir2 = new Directory("dir2", nullptr);
	Directory *dir3 = new Directory("dir3", nullptr);
	Directory *dir4 = new Directory("dir4", nullptr);
	File *file1 = new File("file1", 10);
	File *file2 = new File("file2", 12);
	File *file3 = new File("file3", 13);
	File *file4 = new File("file4", 14);
	File *file5 = new File("file5", 15);
	File *file6 = new File("file6", 16);
	File *file7 = new File("file7", 17);
	File *file8 = new File("file8", 18);
	File *file9 = new File("file9", 19);
	//TODO Add a test for copy constructor
	//Randomly distributing the files among the files.
	dir1->addFile(dir2);
	dir1->addFile(dir3);
	dir4->addFile(file4);
	dir4->addFile(file5);
	dir4->addFile(file6);
	dir3->addFile(file7);
	dir3->addFile(file8);
	dir1->addFile(file1);
	dir1->addFile(file2);
	dir1->addFile(file3);
	dir2->addFile(dir4);
	dir2->addFile(file9);
	Directory* dir5 = new Directory(*dir1);
	delete dir1;
	//Checks if the deletion of dir1 affected dir5, which was copied from it.
	if (dir5->getName().compare("dir1") != 0){
		cout << "(1) Directory was not copied correctly using the copy constructor" << endl;
	}
	bool flag = false;
	//The following test makes sure that the parents were updated properly when copying.
	for (BaseFile* child_of_dir5: dir5->getChildren()){
		if (dynamic_cast<Directory*> (child_of_dir5)){
			if (child_of_dir5->getName().compare("dir2") || child_of_dir5->getName().compare("dir3")){
				flag = true;
			}
			if (((Directory*)child_of_dir5)->getParent() != dir5){
				cout << "(2) Parent one level down not updated properly when using copy constructor" << endl;
				break;
			}
			if (child_of_dir5->getName().compare("dir2")){
				for (BaseFile* child_of_dir2: ((Directory*)child_of_dir5)->getChildren()){
					if (dynamic_cast<Directory*> (child_of_dir2)){
						if (((Directory*)child_of_dir2)->getParent() != child_of_dir2){
							cout << "(3) Parent two levels down not updated properly when using copy constructor" << endl;
							break;
						}

					}
				}
			}
		}
	}
	if (!flag){
		//If it couldn't find both folders, it means the copying function of the copy constructor is malfunctioning.
		cout << "(4) Children not copied properly from parent" << endl;
	}
	//Copy assignment operator, using the dir5 from last part.
	//Creating a small batch of folders
	Directory* dir6 = new Directory("dir6", nullptr);
	File* file10 = new File("bobby", 15);
	dir6->addFile(file10);
	dir6->addFile(dir5);
	Directory* dir7 = new Directory("dir7", nullptr);
	Directory* dir8 = new Directory("dir8", nullptr);
	dir7->addFile(dir8);
	*dir5 = *dir7; //Folders 5 and 7 should be identical and the previous content of dir7 should have been deleted.
	for (BaseFile* child_of_dir6: dir6->getChildren()){
		if (child_of_dir6->getName().compare("dir1") == 0){
			cout << "(5) Child not deleted from previous parent when using copy assignment operator." << endl;
			break;
		}
	}
	for (BaseFile* child_of_new_dir5: dir5->getChildren()){
		if (child_of_new_dir5->getName().compare("dir8") != 0){
			cout << "(6) Error: dir5 should have a single child" << endl;
		}
	}
	delete dir5;
	delete dir6;
	delete dir7;
	cout << "\nNow for the move operators and assignment operators for directory. "
			"\nAll assets have been cleared." << endl;
	Directory* dir10 = new Directory("dir10", nullptr);
	Directory* dir11 = new Directory("dir11", nullptr);
	Directory* dir12 = new Directory("dir12", nullptr);
	Directory* dir15 = new Directory("dir15", nullptr);
	File* file11 = new File("file11", 20);
	File* file12 = new File("file12", 21);
	File* file13 = new File("file13", 23);
	File* file16 = new File("file16", 26);
	File* file17 = new File("file17", 27);
	dir10->addFile(dir11);
	dir11->addFile(file11);
	dir11->addFile(dir12);
	dir11->addFile(file16);
	dir11->addFile(file17);
	dir12->addFile(file12);
	dir12->addFile(file13);
	dir12->addFile(dir15);
	Directory* dir13 = new Directory("dir13", nullptr);
	Directory* dir14 = new Directory("dir14", nullptr);
	Directory* dir16 = new Directory("dir16", nullptr);
	Directory* dir17 = new Directory("dir17", nullptr);
	File* file14 = new File("file14", 24);
	File* file15 = new File("file15", 25);
	dir13->addFile(file14);
	dir13->addFile(dir16);
	dir16->addFile(dir17);
	dir17->addFile(file15);
	dir14->addFile(dir13);
	*dir13 = move(*dir11);
	if (dir14->getChildren().size() != 0){
		cout << "(7) Object moved into was not removed from previous parent" << endl;
	}
	bool flag1 = false;
	for (BaseFile* child_of_dir10: dir10->getChildren()){
		if (child_of_dir10 == dir11){
			cout << "(8) Moved object was not removed from parent" << endl;
		}
		if (child_of_dir10 == dir13){
			flag1 = true;
			if (((Directory*)dir13)->getParent() != dir10){
				cout << "(9) Parent not updated properly on the moved object" << endl;
			}
			for (BaseFile* child_of_dir13: ((Directory*)child_of_dir10)->getChildren()){
				if (dynamic_cast<Directory*> (child_of_dir13)){
					if (((Directory*)child_of_dir13)->getParent() != child_of_dir10){
						cout << "(10) Faulty update of parent 2 levels down" << endl;
					}
				}
			}
		}
	}
	if (!flag1){
		cout << "(11) Moved object was not properly updated in parent." << endl;
	}
	if (dir13->getChildren().size() != 4){
		cout << "(12) Children were not moved properly" << endl;
	}
	delete dir14;
	delete dir11;
	delete dir10;
	cout << "\nI'm not sure but the following move constructor part, take the results with a grain of salt." << endl;
	Directory* dir20 = new Directory("dir20", nullptr);
	Directory* dir21 = new Directory("dir21", nullptr);
	Directory* dir22 = new Directory("dir22", nullptr);
	Directory* dir23 = new Directory("dir23", nullptr);
	Directory* dir24 = new Directory("dir24", nullptr);
	dir20->addFile(dir21);
	dir21->addFile(dir22);
	dir21->addFile(dir23);
	dir23->addFile(dir24);
	Directory* dir25 = new Directory(move(*dir21));
	if (dir20->getChildren().size() != 1){
		cout << "(13) Incorrect number of children in new parent. Check if it was added twice." << endl;
	}
	if (dir25->getParent() != dir20){
		cout << "(14) Parent was not updated for the moved object, move constructor" << endl;
	}
	if (dir20->getChildren()[0] != dir25){
		cout << "(15) Moved object was not inserted to the parent, move constructor" << endl;
	}
	BaseFile* temp1 = dir25->getChildren()[0];
	BaseFile* temp2 = dir25->getChildren()[1];
	if (temp1 != dir22 || temp2 != dir23){
		cout << "(16) Children were not updated properly when using move constructor" << endl;
	}
	if (((Directory*)temp1)->getParent() != dir25){
		cout << "(17) Parent of first level children were not updated when using move constructor" << endl;
	}
	delete dir20;
	delete dir21;

	cout << "\nCleared assets of the Directory test. Commencing check on filesystem." << endl;
	FileSystem* fs1 = new FileSystem();
	Directory* first_level = new Directory("first_level", nullptr);
	Directory* second_level = new Directory("second_level", nullptr);
	first_level->addFile(second_level);
	fs1->getRootDirectory().addFile(first_level);
	fs1->setWorkingDirectory(second_level);
	FileSystem* fs2 = new FileSystem(*fs1);
	if (&fs1->getWorkingDirectory() == &fs2->getWorkingDirectory()){
		cout << "(18) Working directory is still the old one copy constructor" << endl;
	}
	if (&fs1->getWorkingDirectory() == &fs2->getRootDirectory()){
		cout << "(19) Root directroy was not updated properly copy constructor" << endl;
	}
	if (fs2->getWorkingDirectory().getName().compare("second_level") != 0){
		cout << "(20) Working directroy was not updated to the correct one copy constructor" << endl;
	}
	if (fs2->getWorkingDirectory().getParent() == first_level){
		cout << "(21) Parents of the directories 2 levels down were not updated properly copy constructor" << endl;
	}
	if (fs2->getWorkingDirectory().getParent()->getParent() != &fs2->getRootDirectory()){
		cout << "(22) Parents of the directories 1 level 1 were not updated properly. copy constructor" << endl;
	}
	delete fs1;
	fs2->getRootDirectory(); //Checking that the deletion of the first file system did not affect the first.
	fs2->getWorkingDirectory();
	FileSystem* fs3 = new FileSystem; //Empty constructor.
	*fs2 = *fs3;
	fs2->getWorkingDirectory();
	if (&fs2->getWorkingDirectory() != &fs2->getRootDirectory()){
		cout << "(23) Working directory was not updated properly after using copy assignment operator" << endl;
	}
	cout << "\nThe move operator for fs is trivial, "
			"just moving pointers and/or wiping the previous roots. \nNot checking it" << endl;
	cout << "\nFinished testing fs, clearing assets and checking environment." <<endl;
	cout << "\nPlease note that for this section, I assume that the arguments are already parsed" << endl;
	delete fs2;
	delete fs3;
	Environment* env1 = new Environment;
	BaseCommand* command1 = new MkdirCommand(" dir1");
	BaseCommand* command2 = new CdCommand(" /dir1");
	BaseCommand* command3 = new MkdirCommand(" dir2");
	BaseCommand* command4 = new CdCommand(" dir2");
	BaseCommand* command5 = new ExecCommand(" 1", env1->getHistory());
	BaseCommand* command6 = new HistoryCommand(" dummy",env1->getHistory());
	command1->execute(env1->getFileSystem());
	env1->addToHistory(command1);
	command2->execute(env1->getFileSystem());
	env1->addToHistory(command2);
	command3->execute(env1->getFileSystem());
	env1->addToHistory(command3);
	command4->execute(env1->getFileSystem());
	env1->addToHistory(command4);
	command5->execute(env1->getFileSystem());
	env1->addToHistory(command5);
	command6->execute(env1->getFileSystem());
	env1->addToHistory(command6);
	Environment* env2 = new Environment(*env1);
	const vector<BaseCommand*> env1_history = env1->getHistory();
	for (BaseCommand* env1_command: env1_history){
		for (BaseCommand* env2_command: env2->getHistory()){
			if (env1_command == env2_command){
				cout << "(24) The commands were copied by pointer value and not by inner value, copy constructor" << endl;
			}
		}
	}
	if (&env2->getFileSystem() == &env1->getFileSystem()){
		cout << "(25) Environment:: File system was not properly copied when using copy constructor" << endl;
	}

	delete env1;
	cout << "\nMight crash here. \nIt makes sure that there are no dependencies between copied environments, "
			"\nespecially histories" << endl;
	for (BaseCommand* command_of_env2: env2->getHistory()){
		command_of_env2->execute(env2->getFileSystem());
	}
	Environment* env3 = new Environment;
	*env2 = *env3;
	if (env2->getHistory().size() != 0){
		cout << "(26) History was no properly deleted when using copy assignment operator" << endl;
	}
	if (&env2->getFileSystem() == &env3->getFileSystem()){
		cout << "(27) Environment:: File system was not properly copied when using copy assignment operator." << endl;
	}
	delete env2;
	delete env3;
	cout << "If there were no warning up to this points, it means that your RO5 classes are working fine."
			"\nAt least according to my tests. Best of luck!" << endl;

	return 0;
}
//
//
//
