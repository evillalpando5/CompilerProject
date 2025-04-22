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
vector<Stmt *>::iterator instrItr; // ADDED for Stmt vector
map<string, int> precMap;

// Runtime Global Methods
void dump(); // prints vartable, instable, symboltable
// You may need a few additional global methods to manipulate the global variables
// Classes Stmt and Expr
// It is assumed some methods in statement and expression objects will change and
// you may need to add a few new ones.
class Expr{ // expressions are evaluated!
public:
	virtual string toString() = 0;
};
class ConstIntExpr : public Expr{
private:
	int value;
public:
	ConstIntExpr(const int val) {value = val;}
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
	IdIntExpr(const string s){id = s;}
	int eval() {
		return stoi(vartable[id]);
	}
	string toString(){return "id: " + id;}
};

class IdStringExpr : public Expr{
private:
	string id;
public:
	IdStringExpr(const string s){id = s;}
	string eval() {
		return vartable[id];
	}
	string toString(){return "id: " + id;}
};
class PostIntFixExpr : public Expr { // erika
	private:
		vector<string> postfixExpr;  // tokens of operators
	public:
		PostIntFixExpr(vector<string> inFixConverted) {
			postfixExpr = inFixConverted;
		}
		int eval() {
			stack<string> postfixStack;
			vector<string> output;
			for (int i = 0; i < output.size(); i++) {
				if (isOperand(output[i])) {
					postfixStack.push(output[i]);
				}
				else {
					int right = stoi(postfixStack.top());
					postfixStack.pop();
					int left = stoi(postfixStack.top());
					postfixStack.pop();
					int total = applyOperator(left, right, output[i]);
					postfixStack.push(to_string(total));
				}
			}
			if (postfixStack.size() == 1) {
				cout << "evaluted successfully: " << postfixStack.top() << endl;
				return stoi(postfixStack.top());
			}
		}
		bool isOperand(string term) {
			for (int i =0; i < term.size(); i++) {
				if (!isdigit(term[i])) {
					return false;
				}
			}
			return true;
		}
		int applyOperator(int a, int b, string oper) {
			if (oper == "+") { return a + b; }
			else if (oper == "-") { return a - b; }
			else if (oper == "*") { return a * b; }
			else if (oper == "/") { return a / b; }
			else if (oper == "%") { return a % b; }
			else if (oper == "<="){ return a <= b; }
			else if (oper == ">="){ return a >= b; }
			else if (oper == ">"){ return a > b; }
			else if (oper == "<"){ return a < b; }
			else if (oper == "=="){ return a == b; }
			else if (oper == "!=" ){ return a != b; }
			else if (oper == "and"){ return a && b; }
			else if (oper == "or"){ return a || b; }
			else { return NULL; }
		}
		string toString() {
			vector<string>::iterator postfixExprItr;
			while (postfixExprItr != postfixExpr.end()) {
				cout << *postfixExprItr ;
			}
		}
};
class PostStringFixExpr : public Expr { // erika
private:
	vector<string> postfixExpr;  // tokens of operators
public:
	PostStringFixExpr(vector<string> postfixExpr) {	}
	string toString() {
		vector<string>::iterator postfixExprItr;
		while (postfixExprItr != postfixExpr.end()) {
			cout << *postfixExprItr ;
		}
	}
	string eval() {
		stack<string> postfixStack;
		vector<string> output;
		for (int i = 0; i < output.size(); i++) {
			if (isOperand(output[i])) {
				postfixStack.push(output[i]);
			}
			else {
				string right = postfixStack.top();
				postfixStack.pop();
				string left = postfixStack.top();
				postfixStack.pop();
				string total = applyOperator(left, right, output[i]);
				postfixStack.push(total);
			}
		}
		if (postfixStack.size() == 1) {
			cout << "evaluted successfully: " << postfixStack.top() << endl;
			return postfixStack.top();
		}
	}
	bool isOperand(string term) {
		for (int i =0; i < term.size(); i++) {
			if (!isdigit(term[i])) {
				return false;
			}
		}
		return true;
	}
	string applyOperator(string a, string b, string oper) {
		if (oper == "+") { return a + b; }
		// else if (oper == "-") { return a - b; }
		// else if (oper == "*") { return a * b; }
		// else if (oper == "/") { return a / b; }
		// else if (oper == "%") { return a % b; }
		else if (oper == "<="){ if ( a <= b) return "";
		else return NULL; }
		else if (oper == ">="){ if ( a >= b) return "";
		else return NULL;}
		else if (oper == ">") {if ( a > b) return "";
		else return NULL;
		}
		else if (oper == "<") {
			if ( a < b ) return "";
			else return NULL;
		}
			else if (oper == "==") {
				if (a == b) return "";
				else return NULL;
			}
			else if (oper == "!=" ) {
				if (a != b) return "";
				else return NULL;
			}
			else if (oper == "and"){ if (!a.empty() && !b.empty()) return "";
				else return NULL; }
			else if (oper == "or") {
				if ( !a.empty() || !b.empty()) return "";
				else return NULL;
			}
			else { return NULL; }
	}
};

class Stmt{ // statements are executed!
private:
	string name;
public:
	Stmt(string n) {
		name = n;
	}
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
	AssignStmt(const string variable, Expr* expression)
		: Stmt("s_assign"), var(variable), p_expr(expression){
	}
	~AssignStmt() {
		delete p_expr;
	}
	string toString() {
		return "var: " + var + " expr: " + p_expr->toString();
	}
	void execute() {
		if (symboltable[var] == "t_integer") {
			PostIntFixExpr* IntFixExpr = dynamic_cast<PostIntFixExpr*>(p_expr);
			if (IntFixExpr) {
				vartable[var] = IntFixExpr->eval();
			}
			// ConstIntExpr* IntConstExpr = dynamic_cast<ConstIntExpr*>(p_expr);
			// else if(IntConstExpr) {
			// 	vartable[var] = IntConstExpr->eval();
			// }
			else {
				IdIntExpr* IntIDExpr = dynamic_cast<IdIntExpr*>(p_expr);
				vartable[var] = IntIDExpr->eval();
			}
		}
		// if (ConstIntExpr* e = dynamic_cast<ConstIntExpr*>(p_expr)) {
		// 	if (e->eval() == 0) {pc = elsetarget;}
		// 	else {pc++;}
		// }
		// else if (IdIntExpr* e = dynamic_cast<IdIntExpr*>(p_expr)) {
		// 	if (e->eval() == 0){pc = elsetarget;}
		// 	else {pc++;}
		// }
		// else if (PostIntFixExpr* e = dynamic_cast<PostIntFixExpr*>(p_expr)) {
		// 	if (e->eval() == 0) {pc = elsetarget;}
		// 	else {pc++;}
		// }
		// else if (PostStringFixExpr* e = dynamic_cast<PostStringFixExpr*>(p_expr)) {
		// 	if (e->eval() == "NULL") {pc = elsetarget;}
		// 	else {pc++;}
		// }
		// else if (IdStringExpr* e = dynamic_cast<IdStringExpr*>(p_expr)) {
		// 	pc++;
		// }
		// else if (ConstStringExpr* e = dynamic_cast<ConstStringExpr*>(p_expr)) {
		// 	pc++;
		// }
	}
};

class InputStmt : public Stmt{
private:
	string var;
public:
	InputStmt(string variable)
		: Stmt("t_input"), var(variable){
	}
	string toString() {
		return "inputted var:" + var;
	}

	void execute() {
		string type = symboltable[var];
		if (type == "t_number") {
			int x;
			cin >> x;
			vartable[var] = x;
		}
		if (type == "t_string") {
			string  x;
			cin >> x;
			vartable[var] = x;
		}
	}
};

class StrOutStmt : public Stmt{
private:
	string value;
public:
	StrOutStmt(const string val)
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
		 //p_expr->evalInt();
	}
};
class IfStmt : public Stmt {
	private:
		Expr *p_expr;
		int elsetarget;

	public:
		IfStmt();

		~IfStmt() {
			if (p_expr != nullptr)
				delete p_expr;
		}

		string toString() {
			return "Expr:" + p_expr->toString() + " elsetarget:" + to_string(elsetarget);
		}

		void execute() {
			if (ConstIntExpr *c = dynamic_cast<ConstIntExpr *>(p_expr)) {
				if (c->eval() == 0) { pc = elsetarget; } else
					pc++;
			}
			// if (ConstStringExpr *c = dynamic_cast<ConstStringExpr *>(p_expr)) {
			// 	if (c->eval() != NULL) { pc = elsetarget; } else
			// 		pc++;
			// }
			if (IdIntExpr *c = dynamic_cast<IdIntExpr *>(p_expr)) {
				if (c->eval() == 0) { pc = elsetarget; } else
					pc++;
			}
			// if (IdStringExpr *c = dynamic_cast<IdStringExpr *>(p_expr)) {
			// 	if (c->eval() != NULL) { pc = elsetarget; } else
			// 		pc++;
			// }
			if (PostIntFixExpr *c = dynamic_cast<PostIntFixExpr *>(p_expr)) {
				if (c->eval() == 0) { pc = elsetarget; } else
					pc++;
			}
			// if (PostStringFixExpr *c = dynamic_cast<PostStringFixExpr *>(p_expr)) {
			// 	if (c->eval() != NULL) { pc = elsetarget; } else
			// 		pc++;
			// }
		}
	};
class WhileStmt : public Stmt{
private:
	Expr* p_expr;
	int elsetarget;
public:
	WhileStmt();
	WhileStmt(Expr* expr, int target): Stmt("t_while"), p_expr(expr), elsetarget(target) {}
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
			if (e->eval() == nullptr) {pc = elsetarget;}
			else {pc++;}
		}
		else if (IdStringExpr* e = dynamic_cast<IdStringExpr*>(p_expr)) {
			if (e->eval() == nullptr) {pc = elsetarget;}
			else {pc++;}
		}
		else if (ConstStringExpr* e = dynamic_cast<ConstStringExpr*>(p_expr)) {
			if (e->eval() == nullptr) {pc = elsetarget;}
			else {pc++;}
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
class GoToStmt : public Stmt {
private:
	int target;
public:
	GoToStmt() : Stmt("s_goto"), target(-1) {}
	void setTarget(int t) {target = t;}
	string toString() { return "Go To: " + target; }
	void execute() { pc = target; }
};
class Compiler {
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
	bool findFirstType() {
		vector<string>::iterator tempTokItr = tokitr;
		vector<string>::iterator tempLexItr = lexitr;

		while (*tempTokItr == "s_lparen") {
			++tempTokItr;
			++tempLexItr;
		}
		string firstToken = *tempTokItr;
		string firstLexeme = *tempLexItr;

		if (firstToken == "t_number") {return 1;}
		if (firstToken == "t_text") {return 0;}
		if (firstToken == "t_id") {
			if (symboltable[firstLexeme] == "t_number") {
				return 1;
			}
			return 0;
		};
	}
	void typeCheck(const string& token, const string& lexeme, bool isInteger) {
		if (token == "t_number" && !isInteger) {
			cout << "Error: Expected string, found number" << endl;
			exit(-1);
		}
		if (token == "t_text" && isInteger) {
			cerr << "Error: Expected integer, found text"<<endl;
			exit(-1);
		}
		if (token == "t_id") {
			bool isInt = symboltable[lexeme] == "t_integer";
			if (isInt != isInteger) {
				cout << "Error:type mismatch with variable " << lexeme << endl;
				exit(-1);
			}
		}
	}
	vector<string> toPostfix(bool isInteger) {
		vector<string> output;
		stack<string> opStack;

		while (tokitr != tokens.end() && *tokitr != "s_rparen") {
			string token = *tokitr;
			string lexeme = *lexitr;

			if (token == "t_number" || token == "t_text" || token == "t_id") {
				typeCheck(token, lexeme, isInteger);
				output.push_back(lexeme);
			} else if (precMap.count(token)) {
				while (!opStack.empty() && precMap.count(opStack.top()) &&
					   precMap[opStack.top()] >= precMap[token]) {
					output.push_back(opStack.top());
					opStack.pop();
					   }
				opStack.push(token);
			} else if (token == "s_lparen") {
				opStack.push(token);
			} else if (token == "s_rparen") {
				while (!opStack.empty() && opStack.top() != "s_lparen") {
					output.push_back(opStack.top());
					opStack.pop();
				}
				if (!opStack.empty()) opStack.pop();
			}

			++tokitr;
			++lexitr;
		}

		while (!opStack.empty()) {
			output.push_back(opStack.top());
			opStack.pop();
		}

		return output;
	}
	Expr* buildExpr(){
		bool isInteger = findFirstType();

		vector<string> postfix = toPostfix(isInteger);

		if (isInteger) {
			return new PostIntFixExpr(postfix);
		} else {
			return new PostStringFixExpr(postfix);
		}
	}
	// headers for populate methods may not change
	void populateTokenLexemes(istream& infile) {
		string line;
		getline(infile, line);
		while(!infile.eof()){
			int pos = line.find(" ");
			tokens.push_back(line.substr(0,pos));
			lexemes.push_back(line.substr(pos + 1));
			getline(infile, line);
		}
	}
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
	// ifstream source("data.txt");
	// ifstream symbols("vars.txt");
	// if (!source || !symbols) exit(-1);
	// Compiler c(source, symbols);
	// c.compile();
	// // might want to call dump to check if everything built correctly
	// // dump();
	// c.run();
	// return 0;
	// // if evalutes to false return NULL
	// // true "
	//
	string x = NULL;

	if (1) {
		cout << "false";
	}
	else {
		cout << "true";
	}
}
