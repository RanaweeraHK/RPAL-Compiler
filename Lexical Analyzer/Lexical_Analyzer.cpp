#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// Define the token types
enum TokenType {
    IDENTIFIER,
    INTEGER,
    OPERATOR,
    STRING,
    DELETE,
    PUNCTION
};

// Define the token structure
struct Token {
    TokenType type;
    string value;
};

// Define the lexical analyzer class
class LexicalAnalyzer {
public:
    unordered_map<string, TokenType> keywords;

public:
    LexicalAnalyzer() {
        // Populate keywords map
        keywords["let"] = IDENTIFIER;
        keywords["where"] = IDENTIFIER;
        keywords["rec"] = IDENTIFIER;
        keywords["in"] = IDENTIFIER;
        keywords["Print"] = IDENTIFIER;
        keywords["eq"] = IDENTIFIER;
        keywords["->"] = IDENTIFIER;
    }

    // Check if the token is a keyword or identifier
    TokenType getTokenType(const string &token) {
        if (keywords.find(token) != keywords.end())
            return keywords[token];
        else if (isInteger(token))
            return INTEGER;
        else if (isString(token))
            return STRING;
        else if (isOperator(token))
            return OPERATOR;
        else if (isIdentifier(token))
            return IDENTIFIER;
        else
            return DELETE;
    }

    // Check if the token is an integer
    bool isInteger(const string &token) {
        for (char c : token) {
            if (!isdigit(c)) {
                return false;
            }
        }
        return true;
    }

    // Check if the token is a string
    bool isString(const string &token) {
        return token.size() >= 2 && token.front() == '\'' && token.back() == '\'';
    }

    // Check if the token is an operator
    bool isOperator(const string &token) {
        static const string operators = "+-*<>&.@/:=~|$!#%^_[}{\"'?";
        return token.size() == 1 && operators.find(token) != string::npos;
    }

    // Check if the token is an identifier
    bool isIdentifier(const string &token) {
        if (token.empty() || !isalpha(token[0]))
            return false;
        for (char c : token.substr(1)) {
            if (!isalnum(c) && c != '_')
                return false;
        }
        return true;
    }

    // Check if the token is a punctuation
    bool isPunctuation(const string &token) {
        static const string punctuation = "();,";
        return token.size() == 1 && punctuation.find(token) != string::npos;
    }
};

// Tokenize function
vector<Token> tokenize(const string& input, LexicalAnalyzer& lexer) {
    vector<Token> tokens;
    string token;
    bool inString = false;

    for (char c : input) {
        if (isspace(c)) {
            if (!token.empty()) {
                TokenType type = lexer.getTokenType(token);
                tokens.push_back({type, token});
                token.clear();
            }
            continue;
        }

        if (lexer.isPunctuation(string(1, c))) {
            if (!token.empty()) {
                TokenType type = lexer.getTokenType(token);
                tokens.push_back({type, token});
                token.clear();
            }
            tokens.push_back({PUNCTION, string(1, c)});
            continue;
        }

        if (c == '\'' && !inString) {
            inString = true;
            token += c;
            continue;
        }

        if (inString) {
            token += c;
            if (c == '\'') {
                TokenType type = lexer.getTokenType(token);
                tokens.push_back({type, token});
                token.clear();
                inString = false;
            }
            continue;
        }

        token += c;
    }

    if (!token.empty()) {
        TokenType type = lexer.getTokenType(token);
        tokens.push_back({type, token});
    }

    return tokens;
}

int main() {
    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }

    string input((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());

    LexicalAnalyzer lexer;
    vector<Token> tokens = tokenize(input, lexer);

    // Output tokens
    for (const auto& token : tokens) {
        cout << "<" << token.value << "> : " << static_cast<int>(token.type) << endl;
    }

    return 0;
}
