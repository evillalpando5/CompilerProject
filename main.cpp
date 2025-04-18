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
void dump(); // prints vartable, instable, symboltable
// You may need a few additional global methods to manipulate the global variables
vector<Stmt *>::iterator instrItr; // ADDED for Stmt vector
// Classes Stmt and Expr
// It is assumed some methods in statement and expression objects will change and
// you may need to add a few new ones.
class Expr{ // expressions are evaluated!
public:
	virtual int evalInt() = 0;
	virtual string evalStr()= 0;
	virtual string toString() = 0;
	virtual ~Expr();
};
class ConstIntExpr : public Expr{
private:
	int value;
public:
	ConstIntExpr(int val) {value = val;}
	int eval() {
		return value;
	}
	string toString() {
		return to_string(value);
	}
};
class ConstStringExpr : public Expr {
private:
	string value;
public:
	ConstStringExpr(string val) {value = val;}
	string eval() {
		return value;
	}
	string toString() {
		return value;
	}
};
class IdIntExpr : public Expr{
private:
	string id;
public:
	IdIntExpr(string s){id = s;}
	int eval() {
		return stoi(vartable[id]);

	}
	string toString(){return "id: " + id;}
};

class IdStringExpr : public Expr{
private:
	string id;
public:
	IdStringExpr(string s){id = s;}
	string eval() {
		return vartable[id];
	}
	string toString(){return "id: " + id;}
};
class PostIntFixExpr : public Expr {
	private:
		vector<string> postfixExpr;  // tokens of operators
	public:
		PostIntFixExpr(string expr) {	}
		int eval() {	}
		string toString() {
			vector<string>::iterator postfixExprItr;
			while (postfixExprItr != postfixExpr.end()) {
				cout << *postfixExprItr ;
			}
		}
};
class PostStringFixExpr : public Expr {
private:
	vector<string> postfixExpr;  // tokens of operators
public:
	PostStringFixExpr(string expr) {	}
	int eval() {	}
	string toString() {
		vector<string>::iterator postfixExprItr;
		while (postfixExprItr != postfixExpr.end()) {
			cout << *postfixExprItr ;
		}
	}
};

class Stmt{ // statements are executed!
private:
	string name;
public:
	Stmt(string n) {
		name = n;
	}
	// virtual ~Stmt(){};
	virtual string toString() = 0;
	virtual void execute() = 0;
	string getName() {
		return name;
	}
};
class AssignStmt : public Stmt{ //erika
	// TO DO : figure out evaluating
private:
	string var;
	Expr* p_expr;
public:
	AssignStmt(string variable, Expr* expression)
		: Stmt("s_assign"), var(variable), p_expr(expression){
	}
	~AssignStmt() {
		delete p_expr;
	}
	string toString() {
		return "var: " + var + " expr: " + p_expr->toString();
	}
	void execute() {
		if (symboltable[var] == "t_integer"){
			vartable[var] = to_string(p_expr->evalInt());
		}
		else {
			vartable[var] = p_expr->evalStr();
		}
	}
};

class InputStmt : public Stmt{
private:
	string var;
public:
	InputStmt(string varaible)
		: Stmt("t_input"), var(varaible){
	}
	~InputStmt();
	string toString();
	void execute();
};

class StrOutStmt : public Stmt{
private:
	string value;
public:
	StrOutStmt(string val)
		: Stmt("t_output"), value(val){
	}
	~StrOutStmt();
	string toString() {
		return "output(" + value + ")";
	}
	void execute() {
		cout << value << endl;
	}
};

class ExprOutStmt : public Stmt{ //erika
private:
	Expr* p_expr;
public:
	ExprOutStmt(Expr * expression)
		: Stmt("t_output"), p_expr(expression){
	}
	~ExprOutStmt() {
		delete p_expr;
	}
	string toString() {
		return p_expr->toString();
	}
	void execute() {
		 p_expr->evalInt();
	}
};
class IfStmt : public Stmt{
private:
	Expr* p_expr;
	int elsetarget;
public:
	IfStmt();
	~IfStmt();
	string toString() {

	};
	void execute();
};
class WhileStmt : public Stmt{
private:
	Expr* p_expr;
	int elsetarget;
public:
	WhileStmt(Expr* expr, int target)
		: Stmt("t_while"), p_expr(expr),elsetarget(target){
	}
	~WhileStmt() {
		if (p_expr != nullptr) delete p_expr;
	}
	string toString() {
		return "while (" + p_expr->toString() + ")elsetarget:" + to_string(elsetarget);
	}
	void execute() {
		if (ConstIntExpr* e = dynamic_cast<ConstIntExpr*>(p_expr)) {
			if (e->eval() == false) {pc = elsetarget;}
			else {pc++;}
		}
		else if (IdIntExpr* e = dynamic_cast<IdIntExpr*>(p_expr)) {
			if (e->eval() == false){pc = elsetarget;}
			else {pc++;}
		}
		else if (PostIntFixExpr* e = dynamic_cast<PostIntFixExpr*>(p_expr)) {
			if (e->eval() == false) {pc = elsetarget;}
			else {pc++;}
		}
		else if (PostStringFixExpr* e = dynamic_cast<PostStringFixExpr*>(p_expr)) {
			if (e->eval() == false) {pc = elsetarget;}
			else {pc++;}
		}
		else if (IdStringExpr* e = dynamic_cast<IdStringExpr*>(p_expr)) {
			pc++;
		}
		else if (ConstStringExpr* e = dynamic_cast<ConstStringExpr*>(p_expr)) {
			pc++;
		}
		else {
			cout << "Error non supported expr in while condition" << endl;
			exit(-1);
		}
	}
	void setElseTarget(int t) {
		elsetarget = t;
	}
};
class GoToStmt : public Stmt{
private:
	int target;
public:
	GoToStmt();
	~GoToStmt();
	void setTarget(int tar) {
		target = tar;
	};
	string toString();
	void execute();
};
class Compiler{
private:
	void buildIf();
	void buildWhile() {
		tokitr++;lexitr++; //skip (
		Expr* condition = buildExpr();

		tokitr++;lexitr++; //skip )

		int whileLocation = insttable.size();
		WhileStmt* whileStmt = new WhileStmt(condition, -1);
		insttable.push_back(whileStmt);

		tokitr++;lexitr++; //skip {

		while (*tokitr != "s_rbrace") {
			buildStmt();
		}
		tokitr++;lexitr++;//skip }
		GoToStmt* gotoStmt = new GoToStmt();
		gotoStmt->setTarget(whileLocation);
		insttable.push_back(gotoStmt);

		whileStmt->setElseTarget(insttable.size());
	}
	void buildStmt() {
		while (tokitr != tokens.end()) {
			if (tokitr != tokens.end() && *tokitr == "s_assign") {
				tokitr--;lexitr--;
				buildAssign();
			}
			else if (*tokitr == "s_input") {
				tokitr++;lexitr++;
				buildInput();
			}
			else if (*tokitr == "s_output") {
				tokitr++;lexitr++;
				buildOutput();
			}
			else if (*tokitr == "s_if") {
				tokitr++;lexitr++;
				buildIf();

			}
			else if (*tokitr == "s_while") {
				tokitr++;lexitr++;
				buildWhile();
			}
			else {
				tokitr++;lexitr++;
			}
		}

	}// erika
	void buildAssign(){// erika
		string variable = *lexitr;
		tokitr++;lexitr++; // =
		tokitr++;lexitr++; // start of expression
		Expr * p_expr = buildExpr();
		insttable.push_back(new AssignStmt(variable,  p_expr));
	}
	void buildInput() {
		string var = "";
		tokitr++; lexitr++;
		tokitr++; lexitr++;
		var = *lexitr;
		tokitr++; lexitr++;
		tokitr++; lexitr++;
		insttable.push_back(new InputStmt(var));
	}
	void buildOutput(){
		// erika
		if (tokitr!=tokens.end() && *tokitr=="t_text") {
			insttable.push_back(new StrOutStmt(*lexitr));
		}
		else {
			Expr * p_expr = buildExpr();
			insttable.push_back(new ExprOutStmt(p_expr));
		}
	}
	// use one of the following buildExpr methods
	// void buildExpr(Expr*&);
	Expr* buildExpr();
	// headers for populate methods may not change
	void populateTokenLexemes(istream& infile);
	void populateSymbolTable(istream& infile) {
		string line;
		getline(infile, line);
		while(!infile.eof()){
			int pos = line.find(" ");
			string var = line.substr(0,pos);
			string tok = line.substr(pos + 1);
			symboltable[var] = tok;
			getline(infile, line);
		}
	}
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
		// erika good I think
		// The run method will execute the code in the instruction
		// table.
		buildStmt();
		if (tokitr == tokens.end()) {
			return true;
		}
		return false;
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
	// if evalutes to false return NULL
	// true "
}
