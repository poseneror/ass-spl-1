#include "Environment.h"

Environment::Environment(): commandsHistory(), fs() {

    // root files
    Directory *d1 = new Directory("dir1", &fs.getRootDirectory());
    Directory *d2 = new Directory("dir2", &fs.getRootDirectory());
    Directory *d3 = new Directory("dir3", &fs.getRootDirectory());
    Directory *d4 = new Directory("dir4", &fs.getRootDirectory());
    Directory *d5 = new Directory("dir5", d1);
    Directory *d6 = new Directory("dir6", d1);

    fs.getRootDirectory().addFile(d1);
    fs.getRootDirectory().addFile(d2);
    fs.getRootDirectory().addFile(d3);
    fs.getRootDirectory().addFile(d4);
    d1->addFile(d5);
    d1->addFile(d6);
    d1->addFile(new File("file4", 12));
    d1->addFile(new File("file3", 12));


}
void Environment::start() {
    string command = "";
    while(command != "exit"){
        if(command != ""){
            string cType = command.substr(0, command.find(" "));
            if(cType == "pwd") {
                PwdCommand(command).execute(fs);
            } else if(cType == "cd") {
                CdCommand(command).execute(fs);
            } else if(cType == "ls") {
                LsCommand(command).execute(fs);
            }
        }
        cout << fs.getWorkingDirectory().getAbsolutePath() << ">";
        getline(cin, command);
    }
}

FileSystem& Environment::getFileSystem() {
    return fs;
}

void Environment::addToHistory(BaseCommand *command) {
    commandsHistory.push_back(command);
}

const vector<BaseCommand *> & Environment::getHistory() const {
    return commandsHistory;
}