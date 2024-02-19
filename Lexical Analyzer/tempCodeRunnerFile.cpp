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
    LAMBDA,
    WHERE,
    TAU,
    AUG,
    CONDITIONAL,
    OR,
    AND,
    NOT,
    GR,
    GE,
    LS,
    LE,
    EQ,
    NE,
    PLUS,
    MINUS,
    NEG,
    MULTIPLY,
    DIVIDE,
    POWER,
    AT,
    GAMMA,
    INTEGER,
    STRING,
    IDENTIFIER,
    TRUE,
    FALSE,
    NIL,
    DUMMY,
    FUNCTION_FORM,
    REC,
    ANDDEF,
    VARIABLE
};

// AST Node structure
struct Node {
    NodeType type;
    vector<shared_ptr<Node>> children;
    string value; // For leaf nodes (e.g., INTEGER, STRING, IDENTIFIER)
};

// Function to create a new AST node
shared_ptr<Node> makeNode(NodeType type, const string& value = "") {
    cout << "Creating new node of type: " << static_cast<int>(type) << endl;
    return make_shared<Node>(Node{type, {}, value});
}

// Function to print the AST in the desired format
void printAST(const shared_ptr<Node>& root, stringstream& ss, bool withAstSwitch) {
    if (!root) return;

    if (withAstSwitch) {
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
            case NodeType::TAU:
                ss << "tau" << endl;
                break;
            case NodeType::AUG:
                ss << "aug" << endl;
                break;
            case NodeType::CONDITIONAL:
                ss << "->" << endl;
                break;
            case NodeType::OR:
                ss << "or" << endl;
                break;
            case NodeType::AND:
                ss << "&" << endl;
                break;
            case NodeType::NOT:
                ss << "not" << endl;
                break;
            case NodeType::GR:
                ss << "gr" << endl;
                break;
            case NodeType::GE:
                ss << "ge" << endl;
                break;
            case NodeType::LS:
                ss << "ls" << endl;
                break;
            case NodeType::LE:
                ss << "le" << endl;
                break;
            case NodeType::EQ:
                ss << "eq" << endl;
                break;
            case NodeType::NE:
                ss << "ne" << endl;
                break;
            case NodeType::PLUS:
                ss << "+" << endl;
                break;
            case NodeType::MINUS:
                ss << "-" << endl;
                break;
            case NodeType::NEG:
                ss << "neg" << endl;
                break;
            case NodeType::MULTIPLY:
                ss << "*" << endl;
                break;
            case NodeType::DIVIDE:
                ss << "/" << endl;
                break;
            case NodeType::POWER:
                ss << "**" << endl;
                break;
            case NodeType::AT:
                ss << "@" << endl;
                break;
            case NodeType::GAMMA:
                ss << "gamma" << endl;
                break;
            case NodeType::INTEGER:
                ss << "<INT:" << root->value << ">" << endl;
                break;
            case NodeType::STRING:
                ss << "<STR:" << root->value << ">" << endl;
                break;
            case NodeType::IDENTIFIER:
                ss << "<ID:" << root->value << ">" << endl;
                break;
            case NodeType::TRUE:
                ss << "true" << endl;
                break;
            case NodeType::FALSE:
                ss << "false" << endl;
                break;
            case NodeType::NIL:
                ss << "nil" << endl;
                break;
            case NodeType::DUMMY:
                ss << "dummy" << endl;
                break;
            case NodeType::REC:
                ss << "rec" << endl;
                break;
            case NodeType::ANDDEF:
                ss << "and" << endl;
                break;
            case NodeType::VARIABLE:
                ss << "<VAR:" << root->value << ">" << endl;
                break;
            default:
                ss << "UNKNOWN" << endl;
                break;
        }
    }

    for (const auto& child : root->children) {
        printAST(child, ss, withAstSwitch);
    }
}

// Function to parse the input program and construct the AST
shared_ptr<Node> parseInputProgram(vector<pair<string, string>>& tokens, size_t& index) {
    cout << "Parsing input program..." << endl;
    shared_ptr<Node> root = makeNode(NodeType::LET);

    // Parse 'let' token
    cout << "Parsing 'let' token..." << endl;
    if (tokens[index].first != "let") {
        cerr << "Error: Expected 'let' token." << endl;
        return nullptr;
    }
    index++;

    // Parse function definition
    cout << "Parsing function definition..." << endl;
    auto functionNode = makeNode(NodeType::FUNCTION_FORM);
    while (tokens[index].first != "=") { // Read function parameters
        functionNode->children.push_back(makeNode(NodeType::IDENTIFIER, tokens[index].first));
        index++;
    }
    index++; // Skip "="

    // Parse 'where' or function body
    cout << "Parsing 'where' or function body..." << endl;
    if (tokens[index].first == "where") {
        root->children.push_back(functionNode);
        root->children.push_back(makeNode(NodeType::WHERE));
    } else {
        root->children.push_back(functionNode);
        root->children.push_back(makeNode(NodeType::FUNCTION_FORM));
        while (tokens[index].first != "in") { // Read function body
            root->children[1]->children.push_back(makeNode(NodeType::IDENTIFIER, tokens[index].first));
            index++;
        }
    }

    // Skip 'in' token
    cout << "Skipping 'in' token..." << endl;
    index++;

    // Parse expressions after 'in'
    cout << "Parsing expressions after 'in'..." << endl;
    while (index < tokens.size()) {
        root->children.push_back(makeNode(NodeType::IDENTIFIER, tokens[index].first));
        index++;
    }

    cout << "Parsing completed." << endl;
    return root;
}

int main() {
    // Read tokens from file "lexical_tokens.txt"
    cout << "Reading tokens from file 'lexical_tokens.txt'..." << endl;
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
    cout << "Parsing the sequence of tokens..." << endl;
    auto ast = parseInputProgram(tokens, index);

    // Print the AST without -ast switch
    cout << "Printing AST without -ast switch..." << endl;
    stringstream ssWithoutAst;
    printAST(ast, ssWithoutAst, false);
    cout << "Output Format without -ast switch:\n" << ssWithoutAst.str() << endl;

    // Print the AST with -ast switch
    cout << "Printing AST with -ast switch..." << endl;
    stringstream ssWithAst;
    printAST(ast, ssWithAst, true);
    cout << "Output Format with -ast switch:\n" << ssWithAst.str() << endl;

    return 0;
}
