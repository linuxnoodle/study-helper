#include <cards.hpp>

void parseCards(std::string filepath){
    std::ifstream file(filepath);
    if (!file){
        std::cerr << "Error: Study file provided not found.\n";
        std::exit(1);
    }

    std::string currentLine;
    unsigned int lineNumber = 1;
    while (std::getline(file, currentLine)){
        if (currentLine.rfind("//", 0) == 0 || currentLine == ""){
            // Ignore comments and empty lines
            continue;
        }

        size_t position = currentLine.find(" : ");
        if (position == std::string::npos){
            std::cerr << "Parsing error: line " << lineNumber << " lacks a ` : ` delimiter. Please fix this, and rerun the program.\n";
            std::exit(1);
        } else {
            std::string question, answer, userInput;

            question = currentLine.substr(0, position);
            std::cout << question << "\n";

            answer = currentLine.substr(position + 3, currentLine.length() - 2);
            std::getline(std::cin, userInput);

            if (answer == userInput){
                std::cout << "Congrats! You got it right.\n";
            } else {
                std::cout << "Aww... The correct answer was " << answer << ".\n";
            }
        }

        ++lineNumber;
    }
}

void startStudy(std::string filename){
    std::cout << "Answer the questions, and you will recieve feedback.\n";
    parseCards("cards/" + filename);
}
