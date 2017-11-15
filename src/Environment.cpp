#include "Environment.h"

Environment::Environment(): commandsHistory(), fs() {

    // root files
    Directory *d1 = new Directory("dir1", &fs.getRootDirectory());
    Directory *d2 = new Directory("die2", &fs.getRootDirectory());
    Directory *d3 = new Directory("die3", &fs.getRootDirectory());
    Directory *d4 = new Directory("dir4", &fs.getRootDirectory());
    Directory *d5 = new Directory("die5", &fs.getRootDirectory());
    Directory *d6 = new Directory("die6", &fs.getRootDirectory());

    fs.getRootDirectory().addFile(d1);
    fs.getRootDirectory().addFile(d2);
    fs.getRootDirectory().addFile(d3);
    fs.getRootDirectory().addFile(d1);
    d1->addFile(new File("file3", 12));
    d2->addFile(new File("file4", 5));


    fs.getRootDirectory().findChild("dir1")->addFile(new File("file3", 12));



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
            }
        }
        cout << fs.getWorkingDirectory().getAbsolutePath() << ">";
        //TODO: CHANGE TO THIS! for multi-word input
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