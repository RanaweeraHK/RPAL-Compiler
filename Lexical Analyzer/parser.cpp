#include <iostream>
#include <vector>
#include <memory>
#include <sstream>
#include <fstream>
#include <utility>

using namespace std;

// Define the types of AST nodes
enum class NodeType {
    LET,
    FUNCTION_FORM,
    WHERE,
    GAMMA,
    TAU,
    REC,
    IDENTIFIER,
    INTEGER
};

// AST Node structure
struct Node {
    NodeType type;
    vector<shared_ptr<Node>> children;
    string value; // For leaf nodes (e.g., INTEGER, IDENTIFIER)
};

// Function to create a new AST node
shared_ptr<Node> makeNode(NodeType type, const string& value = "") {
    return make_shared<Node>(Node{type, {}, value});
}

// Function to print the AST in the desired format
void printAST(const shared_ptr<Node>& root, stringstream& ss) {
    if (!root) return;

    switch (root->type) {
        case NodeType::LET:
            ss << "let" << endl;
            break;
        case NodeType::FUNCTION_FORM:
            ss << ".function_form" << endl;
            break;
        case NodeType::WHERE:
            ss << "where" << endl;
            break;
        case NodeType::GAMMA:
            ss << "gamma" << endl;
            break;
        case NodeType::TAU:
            ss << "tau" << endl;
            break;
        case NodeType::REC:
            ss << "rec" << endl;
            break;
        case NodeType::IDENTIFIER:
            ss << "<ID:" << root->value << ">" << endl;
            break;
        case NodeType::INTEGER:
            ss << "<INT:" << root->value << ">" << endl;
            break;
        default:
            ss << "UNKNOWN" << endl;
            break;
    }

    for (const auto& child : root->children) {
        printAST(child, ss);
    }
}

// Function to parse the input program and construct the AST
shared_ptr<Node> parseInputProgram(vector<pair<string, string>>& tokens, size_t& index) {
    shared_ptr<Node> root = makeNode(NodeType::LET);

    // Skip 'IDENTIFIER let'
    index++;

    // Parse function definition
    auto functionNode = makeNode(NodeType::FUNCTION_FORM);
    while (tokens[index].first != "IDENTIFIER") { // Read function parameters
        functionNode->children.push_back(makeNode(NodeType::IDENTIFIER, tokens[index].second));
        index++;
    }

    // Skip 'IDENTIFIER where'
    index++;

    // Parse 'where' block
    root->children.push_back(functionNode);
    root->children.push_back(makeNode(NodeType::WHERE));

    // Parse recursive definition
    if (tokens[index].second == "rec") {
        root->children.push_back(makeNode(NodeType::REC));
        index++; // Skip 'rec'

        // Parse recursive function definition
        auto recFunctionNode = makeNode(NodeType::FUNCTION_FORM);
        while (tokens[index].first != "OPERATOR" || tokens[index].second != "->") {
            recFunctionNode->children.push_back(makeNode(NodeType::IDENTIFIER, tokens[index].second));
            index++;
        }
        root->children.back()->children.push_back(recFunctionNode);
    }

    // Skip 'OPERATOR ->'
    index++;

    // Parse expression after 'OPERATOR ->'
    while (index < tokens.size()) {
        if (tokens[index].first == "DELETE") {
            index++; // Skip 'DELETE'
            index++; // Skip next token (e.g., 'N-1')
            continue;
        }
        if (tokens[index].first == "IDENTIFIER" && tokens[index + 1].first == "PUNCTION" && tokens[index + 2].second == "(") {
            auto gammaNode = makeNode(NodeType::GAMMA);
            gammaNode->children.push_back(makeNode(NodeType::IDENTIFIER, tokens[index].second)); // Function name
            index++; // Skip function name
            index++; // Skip '('
            while (tokens[index].first != "PUNCTION" || tokens[index + 1].first != "PUNCTION" || tokens[index + 2].first != "PUNCTION" || tokens[index + 3].second != ")") {
                gammaNode->children.push_back(makeNode(NodeType::IDENTIFIER, tokens[index].second)); // Arguments
                index++;
            }
            root->children.push_back(gammaNode);
        } else {
            root->children.push_back(makeNode(NodeType::IDENTIFIER, tokens[index].second));
        }
        index++;
    }

    return root;
}

int main() {
    // Read tokens from file "lexical_tokens.txt"
    ifstream inputFile("lexical_tokens.txt");
    if (!inputFile) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }

    vector<pair<string, string>> tokens;
    string tokenType, tokenValue;
    while (inputFile >> tokenType >> tokenValue) {
        tokens.emplace_back(tokenType, tokenValue);
    }
    inputFile.close();

    size_t index = 0;
    // Parse the sequence of tokens
    auto ast = parseInputProgram(tokens, index);

    // Write the AST to an output file
    ofstream outputFile("syntax_tree.txt");
    if (!outputFile) {
        cerr << "Error: Unable to open output file." << endl;
        return 1;
    }

    stringstream ssWithAst;
    printAST(ast, ssWithAst);
    outputFile <<  ssWithAst.str() << endl;
    outputFile.close();

    cout << "Syntax tree has been stored in syntax_tree.txt" << endl;

    return 0;
}
