#ifndef INIT_HPP
#define INIT_HPP

#include <iostream>
#include <string>
#include <fstream>


class Init {
public:
    size_t defineMemory(const std::string& filename);
    std::string* initData(const std::string& filename, size_t size);
    std::string* initWordsData(const std::string& filename);
    std::string* initTargetData(const std::string& filename);
    
    ~Init() = default;

private:
    size_t wordsSize_{};
    size_t targetSize_{};

};

#endif