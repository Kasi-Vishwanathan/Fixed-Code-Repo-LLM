// b2.cpp
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <stdexcept>

using namespace std;

void processFile(istream& input) {
    vector<unsigned char> buffer(1024);
    size_t total = 0;
    
    while (true) {
        input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        size_t bytesRead = input.gcount();
        
        if (input.bad()) {
            throw runtime_error("Error reading file");
        }

        for (size_t i = 0; i < bytesRead; ++i) {
            total += buffer[i];
        }

        if (bytesRead < buffer.size()) {
            if (input.eof()) {
                break;
            }
        }
    }

    cout << "Total: " << total << endl;
}

int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            processFile(cin);
        } else {
            for (int i = 1; i < argc; ++i) {
                ifstream file(argv[i], ios::binary);
                if (!file.is_open()) {
                    cerr << "Could not open file: " << argv[i] << endl;
                    return 1;
                }
                processFile(file);
            }
        }
        return 0;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
}
// File: b2.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>

using namespace std;

void processFile(istream& input) {
    vector<unsigned char> buffer(1024);
    size_t total = 0;
    
    while (true) {
        input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        size_t bytesRead = input.gcount();
        
        if (input.bad()) {
            throw runtime_error("Error reading file");
        }
        
        if (input.fail() && !input.eof()) {
            throw runtime_error("Error reading file");
        }

        for (size_t i = 0; i < bytesRead; ++i) {
            total += buffer[i];
        }

        if (bytesRead < buffer.size()) {
            break;
        }
    }

    cout << "Total: " << total << endl;
}

int main(int argc, char** argv) {
    int exit_code = 0;
    try {
        if (argc < 2) {
            processFile(cin);
        } else {
            for (int i = 1; i < argc; ++i) {
                ifstream file(argv[i], ios::binary);
                if (!file) {
                    cerr << "Could not open file: " << argv[i] << endl;
                    exit_code = 1;
                    continue;
                }
                try {
                    processFile(file);
                } catch (const exception& e) {
                    cerr << "Error processing file " << argv[i] << ": " << e.what() << endl;
                    exit_code = 1;
                }
            }
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        exit_code = 1;
    }
    return exit_code;
}
