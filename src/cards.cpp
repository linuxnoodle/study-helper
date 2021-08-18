#include <cards.hpp>

void parseCards(std::string filepath, std::vector<std::vector<std::string>> &questions){
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
            std::string question, answer;

            question = currentLine.substr(0, position);
            answer = currentLine.substr(position + 3, currentLine.length() - 2);
            questions.push_back(std::vector<std::string>{question, answer}); 
        }
        ++lineNumber;
    }
}

void startStudy(std::string filename){
    std::cout << "Answer the questions, and you will recieve feedback.\n";
    std::vector<std::vector<std::string>> questions;
    parseCards("cards/" + filename, questions);
    
    // Shuffles cards
    std::random_device rd;
    std::default_random_engine engine = std::default_random_engine{rd()};
    std::shuffle(std::begin(questions), std::end(questions), engine);
    
    // Displays shuffled cards
    for (std::vector<std::string> question : questions){
        std::cout << question[0] << "\n";

        std::string userInput;
        std::getline(std::cin, userInput);

        if (question[1] == userInput){
            std::cout << "Congrats! You got it right.\n";
        } else {
            std::cout << "Aww... The correct answer was " << question[1] << ".\n";
        }
    }
}
