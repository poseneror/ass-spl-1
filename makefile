# All Targets
all: Main

# Tool invocations
# Executable "Main" depends on the files Main.o and ...
Main: bin/Main.o bin/Files.o bin/FileSystem.o bin/Commands.o bin/Environment.o
	@echo 'Building target: Main'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/Main bin/Main.o bin/Files.o bin/FileSystem.o bin/Commands.o bin/Environment.o
	@echo 'Finished building target: Main'
	@echo ' '

# Depends on the source and header files
bin/Main.o: src/Main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp

# Depends on the source and header files
bin/Files.o: src/Files.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Files.o src/Files.cpp

# Depends on the source and header files
bin/FileSystem.o: src/FileSystem.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/FileSystem.o src/FileSystem.cpp

# Depends on the source and header files
bin/Commands.o: src/Commands.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Commands.o src/Commands.cpp

# Depends on the source and header files
bin/Environment.o: src/Environment.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Environment.o src/Environment.cpp

#Clean the build directory
clean:
	rm -f bin/*

CC = g++
CFLAGS = -g -Wall -Weffc++ -std=c++11
LFLAGS = -L/usr/lib

all: Main

Main:  bin/Main.o bin/Files.o bin/FileSystem.o bin/Commands.o bin/Environment.o
      	@echo 'Building target: Main'
      	@echo 'Invoking: C++ Linker'
      	$(CC) -o bin/Main bin/Main.o bin/Files.o bin/FileSystem.o bin/Commands.o bin/Environment.o $(LFLAGS)
      	@echo 'Finished building target: Main'
      	@echo ' '


bin/Main.o: src/Main.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Main.o src/Main.cpp

# Depends on the source and header files
bin/Files.o: src/Files.cpp
$(CC) $(CFLAGS) -c -Iinclude -o bin/Files.o src/Files.cpp

# Depends on the source and header files
bin/FileSystem.o: src/FileSystem.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/FileSystem.o src/FileSystem.cpp

# Depends on the source and header files
bin/Commands.o: src/Commands.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Commands.o src/Commands.cpp

# Depends on the source and header files
bin/Environment.o: src/Environment.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Environment.o src/Environment.cpp

#Clean the build directory
clean:
	rm -f bin/*