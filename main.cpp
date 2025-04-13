#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
using namespace std;

// You will need these forward references.
class Expr;
class Stmt;

// Runtime Global Variables
int pc;  // program counter
vector<string> lexemes;
vector<string> tokens;
vector<string>::iterator lexitr;
vector<string>::iterator tokitr;
map<string, string> vartable; 	// map of variables and their values
vector<Stmt *> insttable; 		// table of instructions
map<string, string> symboltable; // map of variables to datatype (i.e. sum t_integer)

// Runtime Global Methods
void dump(); 				// prints vartable, instable, symboltable

// You may need a few additional global methods to manipulate the global variables
vector<Stmt *>::iterator instrItr; // ADDED for Stmt vector


// Classes Stmt and Expr
// It is assumed some methods in statement and expression objects will change and
// you may need to add a few new ones.


class Expr{ // expressions are evaluated!
public:
	virtual int eval() = 0;
	virtual string toString() = 0;
	virtual ~Expr(){}
};

class ConstExpr : public Expr{
private:
	int value;
public:
	ConstExpr(int val);
	int eval();
	string toString();
};

class IdExpr : public Expr{
private:
	string id;
public:
	IdExpr(string s);
	int eval();
	string toString();
};

class InFixExpr : public Expr{ //erika
private:
	vector<Expr *> exprs;
	vector<string> ops;  // tokens of operators
public:
	InFixExpr(string expr) {

	}
	int eval() {

	}
	string toString() {

	}
};

class Stmt{ // statements are executed!
private:
	string name;
public:
	Stmt(){}
	virtual ~Stmt(){};
	virtual string toString() = 0;
	virtual void execute() = 0;
	string getName() {
		return name;
	}
};

class AssignStmt : public Stmt{ //erika
private:
	string var;
	Expr* p_expr;
public:
	AssignStmt(string variable){
		var = variable;
	}
	~AssignStmt();
	string toString() {
		return "var: " + var + " expr: " + p_expr->toString();
	}
	void execute() {

	}
};

class InputStmt : public Stmt{
private:
	string var;
public:
	InputStmt();
	~InputStmt();
	string toString();
	void execute();
};

class StrOutStmt : public Stmt{
private:
	string value;
public:
	StrOutStmt();
	~StrOutStmt();
	string toString();
	void execute();
};

class ExprOutStmt : public Stmt{ //erika
private:
	Expr* p_expr;
public:
	ExprOutStmt();
	~ExprOutStmt();
	string toString();
	void execute();
};

class IfStmt : public Stmt{
private:
	Expr* p_expr;
	int elsetarget;
public:
	IfStmt();
	~IfStmt();
	string toString();
	void execute();
};

class WhileStmt : public Stmt{
private:
	Expr* p_expr;
	int elsetarget;
public:
	WhileStmt();
	~WhileStmt();
	string toString();
	void execute();
};

class GoToStmt : public Stmt{
private:
	int target;
public:
	GoToStmt();
	~GoToStmt();
	void setTarget();
	string toString();
	void execute();
};

class Compiler{
private:
	void buildIf();
	void buildWhile();
	void buildStmt(); // erika
	void buildAssign(); // erika
	void buildInput();
	void buildOutput(); // erika
	// use one of the following buildExpr methods
	void buildExpr(Expr*&);      Expr* buildExpr();
	// headers for populate methods may not change
	void populateTokenLexemes(istream& infile);
	void populateSymbolTable(istream& infile);
public:
	Compiler(){}
	// headers may not change
	Compiler(istream& source, istream& symbols){
		// build precMap - include all logical, relational, arithmetic operators
		populateTokenLexemes(source);
		populateSymbolTable(symbols);
	}
	// The compile method is responsible for getting the instruction
	// table built.  It will call the appropriate build methods.
	bool compile() {
		// erika
		// The run method will execute the code in the instruction
		// table.
		while (tokitr != tokens.end()) {
			if (*tokitr == "s_assign") {
				tokitr--;lexitr--;
				AssignStmt(*lexitr);
			}
			else if (*tokitr == "s_input") {

			}
			else if (*tokitr == "s_output") { // expr and string
				tokitr++;lexitr++; // (
				tokitr++;lexitr++; // inside
				if (tokitr != tokens.end() && *tokitr == "t_text") {
					StrOutStmt();
				}
				else {
					ExprOutStmt();
				}
			}
			else if (*tokitr == "s_if") { // goTo needed
				IfStmt();
			}
			else if (*tokitr == "s_while") { //goTo needed
				WhileStmt();
			}
			tokitr++;lexitr++;
		}
	}
	void run();
};

// prints vartable, instable, symboltable
void dump() {
	cout << "vartable: " << endl;
	for (auto it = vartable.begin(); it != vartable.end(); ++it) {
		std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
	}

	cout << "insttable: " << endl;
	insttable.begin();
	while (instrItr != insttable.end()) {
		cout << (*instrItr)->getName() << " ";
		++instrItr;
	}
	cout << endl;
	cout << "symbol table: " << endl;
	for (auto it = symboltable.begin(); it != symboltable.end(); ++it) {
		std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
	}
}
int main(){
	ifstream source("data.txt");
	ifstream symbols("vars.txt");
	if (!source || !symbols) exit(-1);
	Compiler c(source, symbols);
	c.compile();
	// might want to call dump to check if everything built correctly
	// dump();
	c.run();

	return 0;
}
