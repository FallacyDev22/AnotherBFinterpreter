#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <stack>

using namespace std;

bool WantInput = true;

array<char, 3000> Bytes = {};
int pointer_byte = 0;

vector<char> Tokens = {};
int loop_stack = 0;
array<char, 8> valid_tokens = { '+', '-', '>', '<', '.', ',', '[', ']' };

string code = "";
vector<char> input = {};
int pointer_input = 0;
int pointer_code = 0;

bool verify_token(char Token) {
	for (char i : valid_tokens) {
		if (Token == i) {
			return true;
		}
	}
	return false;
}

bool get_token() {
	for (unsigned int i = 0; i < code.size(); i++) {
		if (!verify_token(code[i])) {
			cout << "SYNTAX ERROR: Invalid Token '" << code[i] << "' at " << i + 1;
			return false;
		}
		Tokens.push_back(code[i]);
	}
	code = "";
	return true;
}

void get_input() {
	char temp = 0;
	while (temp != '\n') {
		cin.get(temp);
		if (temp != '\n') {
			input.push_back(temp);
		}
	}
}
bool get_code() {
	ifstream file;
	file.open("src/data.bf");
	if (!file.is_open()) {
		cout << "DATA ERROR: 'data.bf' not founded.";
		return false;
	}
	char temp = 0;
	while (file >> temp) {
		code += temp;
	}
	return true;
}

int main(int argc, char* argv[])
{
	if (!get_code()) {
		return 1;
	}

	cout << "Input: ";
	if (WantInput) {
		get_input();
	}
	cout << endl;
	cout << "Output: " << endl;

	if (!get_token()) {
		return 1;
	}
	for (int i = 0; i < 3000; i++) {
		Bytes[i] = 0;
	}
	for (int i = 0; i < Tokens.size(); i++) {
		char token = Tokens[i];
		if (token == '+') {
			Bytes[pointer_byte] += 1;
			continue;
		}
		else if (token == '-') {
			Bytes[pointer_byte] -= 1;
			continue;
		}
		else if (token == '>') {
			pointer_byte += 1;
			if (pointer_byte > 2999) {
				cout << "RUNTIME ERROR: Out of positive bounds at " << i + 1;
				return 1;
			}
			continue;
		}
		else if (token == '<') {
			pointer_byte -= 1;
			if (pointer_byte < 0) {
				cout << "RUNTIME ERROR: Negative pointer at " << i + 1;
				return 1;
			}
			continue;
		}
		else if (token == '.') {
			cout << Bytes[pointer_byte];
			continue;
		}
		else if (token == ',') {
			if (input.size() == 0) {
				continue;
			}
			Bytes[pointer_byte] = input[0];
			input.erase(input.begin());
			continue;
		}
		else if (token == '[') {
			loop_stack++;
			for (int j = i + 1; j < Tokens.size(); j++) {
				if (Tokens[j] == '[') {
					loop_stack++;
				}
				if (Tokens[j] == ']') {
					loop_stack--;
				}
				if (loop_stack == 0) {
					if (Bytes[pointer_byte] == 0) {
						i = j;
					}
					break;
				}
				if (j == Tokens.size() - 1) {
					cout << "RUNTIME ERROR: Expected ']' at the end of the loop.";
					return 1;
				}
			}
			continue;
		}
		else if (token == ']') {
			loop_stack++;
			for (int j = i - 1; j > 0; j--) {
				if (Tokens[j] == ']') {
					loop_stack++;

				}
				if (Tokens[j] == '[') {
					loop_stack--;
				}
				if (loop_stack == 0) {
					if (Bytes[pointer_byte] != 0) {
						i = j - 1;
					}
					break;
				}
				if (j == Tokens.size() - 1) {
					cout << "RUNTIME ERROR: Expected '[' at the start of the loop.";
					return 1;
				}
			}
		}
	}
}
