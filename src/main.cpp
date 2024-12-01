#include "../include/init.hpp"
#include "../include/generation.hpp"

#define PASSFROMRANGE 1
#define TARGETPASSGEN 2
#define SEEDPHRASEGEN 3
#define MIXPASSWORDS 4
#define GENERATEDATES 5
#define EXIT_PROGRAM 0


void displayLogo();
void displayMenu();
void runGenerator(YearGen& ygen, PassGenerator& passgen, MixedPasswords& mixpass, std::string outputFile, int selectMethod, int startYear, int endYear);
void passFromRange(YearGen& ygen, PassGenerator& passgen, const std::string& filename);
void targetPassGen(YearGen& ygen, PassGenerator& passgen, const std::string& filename);
void seedPhraseGen(YearGen& ygen, PassGenerator& passgen, const std::string& filename);
void mixpasswords(YearGen& ygen, PassGenerator& passgen, MixedPasswords& mixpass, const std::string& filename);
void generateDates(YearGen& ygen, PassGenerator& passgen, const std::string& filename, int startYear, int endYear);
void writePassToFile(YearGen& ygen, PassGenerator& passgen, const std::string& filename, int options);

int main() {
    std::vector<std::string> years;

    int selectMethod{}, passLimit{}, passLength{}, startYear{}, endYear{};
    bool yearsON{}, symbolsON{};
    size_t wordsSize{}, targetSize{};

    // Change paths to your input/output files
    std::string wordsFilename = "/home/user/etc/projects/Passwords/dict/words.txt";
    std::string targetFilename = "/home/user/etc/projects/Passwords/dict/target.txt";
    std::string outputFile = "/home/user/etc/projects/Passwords/dict/passwords.txt";

    Init init;
    std::string* wordsPointer = init.initWordsData(wordsFilename);
    std::string* targetPointer = init.initTargetData(targetFilename);
    wordsSize = init.defineMemory(wordsFilename);
    targetSize = init.defineMemory(targetFilename);

    if (wordsSize == 0 || targetSize == 0) {
        std::cerr << "Error reading input files. Exiting..." << std::endl;
        return -1;
    }

    while (true) {
        displayLogo();
        displayMenu();

        std::cout << "Select an option: ";
        std::cin >> selectMethod;

        if (selectMethod == EXIT_PROGRAM) {
            std::cout << "Exiting... Goodbye!" << std::endl;
            break;
        }

        if (selectMethod == PASSFROMRANGE || selectMethod == TARGETPASSGEN || selectMethod == SEEDPHRASEGEN || selectMethod == MIXPASSWORDS) {
            std::cout << "[INFO] Enter pass limit and pass length (e.g., 100 8): ";
            std::cin >> passLimit >> passLength;
            if (selectMethod == PASSFROMRANGE) {
                std::cout << "[INFO] Include symbols? (1/0): ";
                std::cin >> symbolsON;
            } else if (selectMethod == TARGETPASSGEN || selectMethod == MIXPASSWORDS) {
                std::cout << "[INFO] Include years in passwords? (1/0): ";
                std::cin >> yearsON;
                std::cout << "[INFO] Enter start year and end year (e.g., 2000 2024): ";
                std::cin >> startYear >> endYear;
                years.clear();
                for (int year = startYear; year <= endYear; ++year) {
                    years.emplace_back(std::to_string(year));
                }
            }
        } else if (selectMethod == GENERATEDATES) {
            std::cout << "[INFO] Enter start year and end year (e.g., 2000 2024): ";
            std::cin >> startYear >> endYear;
            years.clear();
            for (int year = startYear; year <= endYear; ++year) {
                years.emplace_back(std::to_string(year));
            }
        }

        YearGen ygen(years, passLimit, passLength, symbolsON, yearsON, wordsSize, targetSize);
        PassGenerator passgen(wordsPointer, targetPointer, years, passLimit, passLength, symbolsON, yearsON, wordsSize, targetSize);
        MixedPasswords mixpass(passLimit, passLength, symbolsON, yearsON, wordsSize, targetSize, years);

        runGenerator(ygen, passgen, mixpass, outputFile, selectMethod, startYear, endYear);

        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
        system("clear"); // Use "cls" on Windows
    }

    delete[] wordsPointer;
    delete[] targetPointer;
    wordsPointer = nullptr;
    targetPointer = nullptr;

    return 0;
}

void displayLogo() {
    std::cout << "============================\n";
    std::cout << "       PASSWORD TOOL        \n";
    std::cout << "============================\n";
}

void displayMenu() {
    std::cout << "                             \n";
    std::cout << "[1] Pass from range generator\n";
    std::cout << "[2] Target pass generator\n";
    std::cout << "[3] Seed phrase generator\n";
    std::cout << "[4] Mixed pass generator\n";
    std::cout << "[5] Dates generator\n";
    std::cout << "[0] Exit\n";
    std::cout << "                             \n";
}

void runGenerator(YearGen& ygen, PassGenerator& passgen, MixedPasswords& mixpass, std::string outputFile, int selectMethod, int startYear, int endYear){
    if(1 == selectMethod){
        passFromRange(ygen, passgen, outputFile);
    }
    else if(2 == selectMethod){
        targetPassGen(ygen, passgen, outputFile);
    }
    else if(3 == selectMethod){
        seedPhraseGen(ygen, passgen, outputFile);
    }
    else if(4 == selectMethod){
        mixpasswords(ygen, passgen, mixpass, outputFile);
    }
    else if(5 == selectMethod){
        generateDates(ygen, passgen, outputFile, startYear, endYear);
    }
}

void passFromRange(YearGen& ygen, PassGenerator& passgen, const std::string& filename){
    passgen.passwords(PASSFROMRANGE);
    writePassToFile(ygen, passgen, filename, 1);
}

void targetPassGen(YearGen& ygen, PassGenerator& passgen, const std::string& filename){
    passgen.passwords(TARGETPASSGEN);
    writePassToFile(ygen, passgen, filename, 1);
}

void seedPhraseGen(YearGen& ygen, PassGenerator& passgen, const std::string& filename){
    passgen.passwords(SEEDPHRASEGEN);
    writePassToFile(ygen, passgen, filename, 1);
}

void mixpasswords(YearGen& ygen, PassGenerator& passgen, MixedPasswords& mixpass, const std::string& filename){
    mixpass.mixpasswords();
    writePassToFile(ygen, passgen, filename, 1);
}

void generateDates(YearGen& ygen, PassGenerator& passgen, const std::string& filename, int startYear, int endYear){
    ygen.dateGen(startYear, endYear);
    writePassToFile(ygen, passgen, filename, 2);
}

void writePassToFile(YearGen& ygen, PassGenerator& passgen, const std::string& filename, int options) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << '\n';
        return;
    }

    if (1 == options) {
        const auto& passwords = passgen.getPasswords();
        for (const auto& password : passwords) {
            file << password << "\n";
        }
    } else if (2 == options) {
        const auto& dates = ygen.getDates();
        for (const auto& date : dates) {
            file << date << '\n';
        }
    }

    std::cout << "Passwords written to file successfully: " << filename << '\n';
}