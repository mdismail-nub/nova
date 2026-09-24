#include "interpreter.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cout << "Usage: nova <file.mi>\n";
        return 1;
    }

    std::string filename = argv[1];

    if (
        filename.size() < 3 ||
        filename.substr(filename.size() - 3) != ".mi"
    ) {
        std::cerr
            << "Error: Nova files must use the .mi extension.\n";

        return 1;
    }

    std::ifstream file(filename);

    if (!file) {
        std::cerr
            << "Error: Could not open file: "
            << filename << '\n';

        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string source = buffer.str();

    try {

        // 1. Lexing
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        // 2. Parsing
        Parser parser(tokens);
        Program program = parser.parse();

        // 3. Running
        Interpreter interpreter;
        interpreter.run(program);

    }
    catch (const std::exception& error) {

        std::cerr
            << "Nova error: "
            << error.what()
            << '\n';

        return 1;
    }

    return 0;
}
