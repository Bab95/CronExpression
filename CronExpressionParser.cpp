// CronExpressionParser.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <sstream>
using namespace std;
/// <summary>
/// types of expressions possible
/// </summary>
enum ExpressioTypes {
	RANGE,
	COMMA,
	EVERY,
	WILDCARD,
	INVALID,
	SINGLEVALUE
};
/// <summary>
/// Time kind
/// </summary>
enum TIME{
	MINUTES,
	HOURS,
	MONTHS,
	DAYSOFWEEKS,
	DAYSOFMONTHS
};
/// <summary>
/// returns true for range expression type
/// </summary>
/// <param name="exp"></param>
/// <returns></returns>
bool isRangeExpression(string exp) {
	if (regex_match(exp, regex("[0-9]+-[0-9]+"))) {
		return true;
	}
	return false;
}
/// <summary>
/// returns true for comma separated expression type
/// </summary>
/// <param name="exp"></param>
/// <returns></returns>
bool isCommaExpression(string exp) {
	if (regex_match(exp, regex("^[0-9]+(,[0-9]+)*,[0-9]+"))) {
		return true;
	}
	return false;
}
/// <summary>
/// returns true if expression contains / in the begining 
/// </summary>
/// <param name="exp"></param>
/// <returns></returns>
bool isEveryExpression(string exp) {
	if (regex_match(exp, regex("^\/[0-9]+"))) {
		return true;
	}
	return false;
}
/// <summary>
/// returns true if expression is a single numerical value
/// </summary>
/// <param name="exp"></param>
/// <returns></returns>
bool IsSingleValue(string exp) {
	if (regex_match(exp, regex("^[0-9]+"))) {
		return true;
	}
	return false;
}
/// <summary>
/// returns true if range is valid
/// </summary>
/// <param name="range"></param>
/// <returns></returns>
bool isValidRange(pair<int, int>& range) {
	if (range.first <= range.second) {
		return true;
	}
	return false;
}
/// <summary>
/// converts string to integer
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
int toInt(string str) {
	int data;
	stringstream ss;
	ss << str;
	ss >> data;
	return data;
}
/// <summary>
/// get range start and end from range expression
/// </summary>
/// <param name="exp"></param>
/// <returns></returns>
pair<int, int> getRangeElements(string exp) {
	pair<int, int> result;
	size_t pos = exp.find('-');
	if (pos != string::npos) {
		string _a = exp.substr(0, pos);
		string _b = exp.substr(pos + 1, exp.length() - pos - 1);
		int a = toInt(_a);
		int b = toInt(_b);
		result = make_pair(a, b);
		if (isValidRange(result)) {
			return result;
		}
	}
	return make_pair(-1, -1);
}
/// <summary>
/// returns true if expression is a wildcard
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
bool isWildCard(string str) {
	if (str.length() > 1) {
		return false;
	}
	if (str == "*" || str == "?") {
		return true;
	}
	return false;
}
/// <summary>
/// get type of the expression
/// </summary>
/// <param name="exp"></param>
/// <returns>Type of the expression</returns>
ExpressioTypes GetExpressionType(string exp) {
	if (isRangeExpression(exp)) {
		return RANGE;
	}
	else if (isWildCard(exp)) {
		return WILDCARD;
	}
	else if (isCommaExpression(exp)) {
		return COMMA;
	}
	else if (isEveryExpression(exp)) {
		return EVERY;
	}
	else if (IsSingleValue(exp)) {
		return SINGLEVALUE;
	}
	return INVALID;
}
/// <summary>
/// get comma seperated elements from the expression 
/// </summary>
/// <param name="exp"></param>
/// <returns></returns>
vector<int> GetCommaSeperatedElements(string exp) {
	string current = "";
	int i = 0;
	vector<int> result;
	while (i < exp.length()) {
		if (exp[i] == ',') {
			result.push_back(toInt(current));
			current = "";
		}
		else {
			current += exp[i];
		}
		i++;
	}
	return result;
}
/// <summary>
/// return values for an expression
/// </summary>
/// <param name="exp"></param>
/// <param name="time"></param>
/// <returns></returns>
vector<int> getValues(string exp, TIME time) {
	vector<int> result;
	int MAX = -1;
	ExpressioTypes expression_type = GetExpressionType(exp);
	if (expression_type == EVERY) {
		string _num = exp.substr(1, exp.length() - 1);
		int diff = toInt(_num);
		int _start = -1;
		if (time == MINUTES) {
			MAX = 60;
			_start = 0;
		}
		else if (time == HOURS) {
			MAX = 24;
			_start = 0;
		}
		else if (time == MONTHS) {
			MAX = 12;
			_start = 1;
		}
		else if (time == DAYSOFMONTHS) {
			_start = 1;
			MAX = 30;
		}
		else if (time == DAYSOFWEEKS) {
			_start = 0;
			MAX = 7;
		}
		while (_start < MAX) {
			result.push_back(_start);
			_start += diff;
		}
	}
	else if (expression_type == RANGE) {
		pair<int, int> _range = getRangeElements(exp);
		int _start = _range.first;
		int _end = _range.second;
		while (_start <= _end) {
			result.push_back(_start);
			_start++;
		}
	}
	else if (expression_type == WILDCARD) {
		int _start = -1;
		if (time == MINUTES) {
			MAX = 60;
			_start = 0;
		}
		else if (time == HOURS) {
			MAX = 24;
			_start = 0;
		}
		else if (time == MONTHS) {
			MAX = 13;
			_start = 1;
		}
		else if (time == DAYSOFMONTHS) {
			_start = 1;
			MAX = 31;
		}
		else if (time == DAYSOFWEEKS) {
			_start = 0;
			MAX = 7;
		}
		while (_start < MAX) {
			result.push_back(_start);
			_start += 1;
		}
	}
	else if (expression_type == COMMA) {
		result = GetCommaSeperatedElements(exp);
	}
	else if (expression_type == SINGLEVALUE) {
		result.clear();
		result.push_back(toInt(exp));
	}
	return result;
}
/// <summary>
/// print error logs
/// </summary>
/// <param name="i"></param>
void printErrorCode(int i) {
	switch (i) {
	case 0:
		cout << "Invalid Minutes expression!" << endl;
		break;
	case 1:
		cout << "Invalud Hours expression!" << endl;
		break;
	case 2:
		cout << "Invalud Day of Month expression!" << endl;
		break;
	case 3:
		cout << "Invalud Month expression!" << endl;
		break;
	case 4:
		cout << "Invalud Day of Week expression!" << endl;
		break;
	case 5:
		cout << "Invalid no of arguments!" << endl;
		break;
	case 6:
		cout << "Invalid Expression [Day of Months] and [Day of Week] can't use same WildCard!" << endl;
		break;
	default:
		cout << " unknown ERROR" << endl;
	}
}
/// <summary>
/// get values from expression and time kind
/// </summary>
/// <param name="exp"></param>
/// <param name="i"></param>
/// <returns></returns>
vector<int> _getValues(string exp, int i) {
	TIME time;
	switch (i) {
	case 0:
		time = MINUTES;
		break;
	case 1:
		time = HOURS;
		break;
	case 2:
		time = DAYSOFMONTHS;
		break;
	case 3:
		time = MONTHS;
		break;
	case 4:
		time = DAYSOFWEEKS;
		break;
	case 5:
		time = MINUTES;
		break;
	default:
		break;
	}
	return getValues(exp, time);
}
/// <summary>
/// formats output
/// </summary>
/// <param name="results"></param>
/// <param name="command"></param>
void formatoutput(vector<vector<int>>& results, string command) {
	vector<string> headlines;
	headlines.push_back("minute        ");
	headlines.push_back("hour          ");
	headlines.push_back("day of month  ");
	headlines.push_back("month         ");
	headlines.push_back("day of week   ");
	headlines.push_back("command       ");
	for (int i = 0; i < headlines.size() - 1; i++) {
		cout << headlines[i];
		for (int j = 0; j < results[i].size(); j++) {
			cout << results[i][j] << " ";
		}
		cout << endl;
	}
	cout << headlines[headlines.size() - 1] << command;
}
void validateCron(int argc, vector<string>& arguments) {
	vector<vector<int>> results;
	vector<int> minutes, hours;
	int i = 0;
	if (argc != 6) {
		printErrorCode(5);
		exit(5);
	}

	// removing seconds part..... we don't consider that 
	arguments[0] = arguments[0].substr(1, arguments[0].length() - 1);
	
	for (int i = 0; i < argc - 1; i++) {
		if (GetExpressionType(arguments[i]) == INVALID) {
			printErrorCode(i);
		}
	}
	if (GetExpressionType(arguments[2]) == WILDCARD && GetExpressionType(arguments[4]) == WILDCARD) {
		if (arguments[2] == arguments[4]) {
			printErrorCode(6);
			exit(6);
		}
	}
	for (int i = 0; i < argc - 1; i++) {
		results.push_back(_getValues(arguments[i], i));
	}
	formatoutput(results, arguments[argc-1]);
}
void testcode() {
	int argc = 6;
	vector<string> arguments = { "*/15", "0", "1,15", "*", "1-5", "/usr/bin/find" };
	validateCron(argc, arguments);
}
int main(int argc, char* argv[]){
	vector<string> arguments;
	for (int i = 0; i < argc; i++) {
		arguments.push_back(string(argv[i]));
	}
	validateCron(argc, arguments);
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
