# UTS

## By: David Michael

Stands for University of Toledo Shell -- a project for my Advanced Systems Programming (EECS 47--) course.

## Running

Link and Compile the program with `g++ main.cpp lib.h -o {myName}.exe` to produce an executable file name a.exe that can be then run with `{myName}.exe`.

## Structure

Two main files for this program lib.h which contains information special to each OS system that this program supports (currently Windows and Linux ONLY). And then main.cpp which gathers the information and writes it out to a resource file located at `.../Desktop/.ourshrc`.

### lib.h

- Seperating slashes preferences for each os
- number of environment variables (easy copying of env)
- method to return current executable path of program on startup

### main.cpp

Runs as follows

1. Copys all current env variables into a vector
2. Gets password from current user
3. Creates the resource file and writes the following (in order) on new lines

    - Executable Path
    - User Password
    - Environment Variables

4. Closes file writing stream and exits.
