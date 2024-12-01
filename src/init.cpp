#include "../include/init.hpp"


size_t Init::defineMemory(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << '\n';
        return 0;
    }
    size_t counter{};
    std::string line;
    while (std::getline(file, line)) {
        counter++;
    }
    return counter;
}

std::string* Init::initData(const std::string& filename, size_t size) {
    std::string* list = new std::string[size];
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << '\n';
        delete[] list;
        return nullptr;
    }
    std::string line;
    for (size_t i{}; i < size; ++i) {
        std::getline(file, line);
        list[i] = line;
    }
    return list;
}

std::string* Init::initWordsData(const std::string& filename) {
    wordsSize_ = defineMemory(filename);
    if (wordsSize_ == 0) return nullptr;
    return initData(filename, wordsSize_);
}

std::string* Init::initTargetData(const std::string& filename) {
    targetSize_ = defineMemory(filename);
    if (targetSize_ == 0) return nullptr;
    return initData(filename, targetSize_);
}