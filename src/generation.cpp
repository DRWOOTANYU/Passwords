#include "../include/generation.hpp"


//Passwords
Passwords::Passwords(std::string* words, std::string* target, const std::vector<std::string>& years) 
        : words_(words), target_(target), years_(years) {}

const std::unordered_set<std::string> Passwords::getPasswords() const {
    return passwords_;
}

Passwords::~Passwords(){}


//PassGenerator
PassGenerator::PassGenerator(std::string* words, std::string* target, const std::vector<std::string>& years,
                  int passLimit, int passLenght, bool symbolsON, bool yearsON, size_t wordsSize, size_t targetSize)
        : Passwords(words, target, years), 
          passLimit_(passLimit), passLenght_(passLenght), symbolsON_(symbolsON), 
          yearsON_(yearsON), wordsSize_(wordsSize), targetSize_(targetSize) {}

void PassGenerator::passwords(int options) {
    std::mt19937_64 gen(std::random_device{}());
    passwords_.clear();

    while (passwords_.size() < passLimit_) {
        std::string pass;
        switch (options) {
            case 1 :
                passFromRange(pass, gen);
                break;
            case 2 :
                targetPassGen(pass, gen);
                break;
            case 3 :
                seedPhraseGen(pass, gen);
                break;
            default:
                throw std::invalid_argument("Invalid password generation option");
        }

        passwords_.insert(pass);
    }
}

void PassGenerator::passFromRange(std::string& pass, std::mt19937_64& gen) {
    if(true == symbolsON_){
        std::uniform_int_distribution<size_t> dist(0, mixedRange_.size() - 1);
        pass.clear();
        for (int i = 0; i < passLenght_; ++i) {
            pass += mixedRange_[dist(gen)];
        }
    }
    else{
        std::uniform_int_distribution<size_t> dist(0, charRange_.size() - 1);
        pass.clear();
        for (int i = 0; i < passLenght_; ++i) {
            pass += charRange_[dist(gen)];
    }
    }
}

void PassGenerator::arrayCopy() {
    if (newTargetArray) {
        delete[] newTargetArray;
        newTargetArray = nullptr;
    }
    
    newTargetArray = new std::string[targetSize_ + years_.size()];
    std::copy(target_, target_ + targetSize_, newTargetArray);
    std::copy(years_.begin(), years_.end(), newTargetArray + targetSize_);
}

void PassGenerator::targetPassGen(std::string& pass, std::mt19937_64& gen) {
    pass.clear();
    std::uniform_int_distribution<size_t> distChar(0, charRange_.size() - 1);
    std::uniform_int_distribution<size_t> distTarget(0, targetSize_ - 1);
    std::uniform_int_distribution<size_t> distYear(0, years_.size() - 1);

    std::string target1 = target_[distTarget(gen)];
    std::string target2 = target_[distTarget(gen)];
    std::string yearPart = yearsON_ && !years_.empty() ? years_[distYear(gen)] : "";

    if (gen() % 2 == 0) {
        pass += target1;
        if (gen() % 2 == 0 && !yearPart.empty()) {
            pass += yearPart;
        } else {
            pass += target2;
        }
    } else {
        if (!yearPart.empty()) {
            pass += yearPart;
        }
        pass += target1;
    }

    while (pass.length() < passLenght_) {
        pass += charRange_[distChar(gen)];
    }

    if (pass.length() > passLenght_) {
        pass = pass.substr(0, passLenght_);
    }
}


void PassGenerator::seedPhraseGen(std::string& pass, std::mt19937_64& gen) {
    size_t wordCount = 0;
    pass.clear();

    while (wordCount < 12) { 
        std::string word = words_[gen() % wordsSize_];
        pass += word;

        ++wordCount;
        if (wordCount < 12) {
            pass += " ";
        }
    }

    while (pass.length() < passLenght_) {
        std::string word = words_[gen() % wordsSize_];
        pass += " " + word;
        ++wordCount;
    }
}

PassGenerator::~PassGenerator(){
    delete[] newTargetArray;
    newTargetArray = nullptr;
}


//MixedPasswords
MixedPasswords::MixedPasswords(int passLimit, int passLenght, bool symbolsON, bool yearsON, size_t wordsSize, size_t targetSize,
                   const std::vector<std::string>& years)
        : PassGenerator(new std::string[wordsSize], new std::string[targetSize], years , passLimit, passLenght, symbolsON, yearsON, wordsSize, targetSize){
        mixedArray_ = new std::string[wordsSize + targetSize + years_.size()];
    }

void MixedPasswords::arrayCopy(){
    if(yearsON_ == true){
        std::copy(target_, target_ + targetSize_, mixedArray_);
        std::copy(words_, words_ + wordsSize_, mixedArray_ + targetSize_);
        std::copy(years_.begin(), years_.end(), mixedArray_ + targetSize_ + wordsSize_);
    } else {
        std::copy(target_, target_ + targetSize_, mixedArray_);
        std::copy(words_, words_ + wordsSize_, mixedArray_ + targetSize_);
    }
}

void MixedPasswords::mixpasswords() {
        
    arrayCopy();

    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<> dis(0, passLenght_ - 1);
    std::string pass;

    for (int iter{}; iter < passLimit_; ++iter) {
        while (pass.size() < passLenght_) {
            pass += mixedArray_[gen() % (wordsSize_ + targetSize_ + years_.size())];
        }
        passwords_.insert(pass);
    }
}


MixedPasswords::~MixedPasswords() {
    delete[] mixedArray_;
    mixedArray_ = nullptr;
}


//YearGen
YearGen::YearGen(const std::vector<std::string>& years, int passLimit, int passLenght, bool symbolsON, bool yearsON, size_t wordsSize, size_t targetSize)
    : MixedPasswords(passLimit, passLenght, symbolsON, yearsON, wordsSize, targetSize, years) {}

const std::vector<std::string> YearGen::getDates() const{
    return dates_;
} 

bool YearGen::isLeapYear(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

std::map<int, int> YearGen::getDaysInMonth(int year) {
    std::map<int, int> daysInMonth = {
        {1, 31}, {2, isLeapYear(year) ? 29 : 28}, {3, 31}, {4, 30},
        {5, 31}, {6, 30}, {7, 31}, {8, 31}, {9, 30}, {10, 31}, {11, 30}, {12, 31}
    };
    return daysInMonth;
}

std::vector<std::string> YearGen::dateGen(int startYear, int endYear) {
    std::string date;

    for (int year{startYear}; year <= endYear; ++year) {
        std::map<int, int> daysInMonth = getDaysInMonth(year);

        for (int month{1}; month <= 12; ++month) {
            int dayTo = daysInMonth[month];

            for (int day{1}; day <= dayTo; ++day) {
                date = (day < 10 ? "0" + std::to_string(day) : std::to_string(day)) +
                       (month < 10 ? "0" + std::to_string(month) : std::to_string(month)) +
                       std::to_string(year);
                
                dates_.push_back(date);
            }
        }
    }

    return dates_;
}

YearGen::~YearGen(){}