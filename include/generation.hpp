#ifndef GENERATION_HPP
#define GENERATION_HPP

#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <map>


class Passwords {
protected:
    const std::string charRange_ = "0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
    const std::string symbols_ = "!@#$%^&*";
    const std::string mixedRange_ = charRange_ + symbols_;
    std::string* words_;
    std::string* target_;
    std::vector<std::string> years_;
    std::unordered_set<std::string> passwords_;
public:
    Passwords(std::string* words, std::string* target, const std::vector<std::string>& years);

        const std::unordered_set<std::string> getPasswords() const;

    ~Passwords();
};

class PassGenerator : public Passwords {
protected:
    bool symbolsON_, yearsON_;
    int passLimit_, passLenght_;
    size_t wordsSize_, targetSize_;
    std::string* newTargetArray{nullptr};
public:
    PassGenerator(std::string* words, std::string* target, const std::vector<std::string>& years,
                  int passLimit, int passLenght, bool symbolsON, bool yearsON, size_t wordsSize, size_t targetSize);

    void passwords(int options);

    void passFromRange(std::string& pass, std::mt19937_64& gen);
    
    virtual void arrayCopy();

    void targetPassGen(std::string& pass, std::mt19937_64& gen);

    void seedPhraseGen(std::string& pass, std::mt19937_64& gen);

    ~PassGenerator();
};

class MixedPasswords : public PassGenerator {
private:
    std::string* mixedArray_{nullptr};
public:
    MixedPasswords(int passLimit, int passLenght, bool symbolsON, bool yearsON, size_t wordsSize, size_t targetSize,
                   const std::vector<std::string>& years);

    void arrayCopy() override;

    void mixpasswords();

    ~MixedPasswords();
};

class YearGen : public MixedPasswords {
private:
    std::vector<std::string> dates_;
public:
    YearGen(const std::vector<std::string>& years, int passLimit, int passLenght, bool symbolsON, bool yearsON, size_t wordsSize, size_t targetSize);

    const std::vector<std::string> getDates() const;

    bool isLeapYear(int year);
    
    std::map<int, int> getDaysInMonth(int year);
    
    std::vector<std::string> dateGen(int startYear, int endYear);

    ~YearGen();
};

#endif