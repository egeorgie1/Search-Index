#include <iostream>
#include <fstream>
#include <cctype>
#include <list>
#include <vector>
#include <iterator>
#include "dirent.h"
#include <string>
#include <cstring>
#include <stack>
#include <sstream>
#include "sets.h"

void tolowerCase(std::string&);
int weight(char);
bool isOperation(char);

class InvertedIndex{
public:
//private:
	struct entry {
		std::string word;
	    std::list<int> l;
	};

	std::vector < std::string> files;
	std::vector<entry> words;

	void parseFolder(const char* folder) {
		DIR* dir;
		dirent* ent;
		dir = opendir(folder);
		ent = readdir(dir);
		while (ent != NULL) {
			files.push_back(std::string(ent->d_name));
			ent = readdir(dir);
		}
	}

	int searchPos(const std::string& word, int left, int right, bool& is_in) {
		//Binary Search
		if (right <= left) { //bottom of recursion
			if (word == words[left].word) {
				is_in = true;
				return left;
			}
			else {/*dumata e nova, searchPos shte vurne posiciqta, predi koqto da bude insert-nata*/
				if (word < words[left].word)
					return left;
				else //word > words[left].word
					return left + 1;
			}
		}
		else {//right>left
			int mid = left + (right - left) / 2;
			if (word == words[mid].word) {
				is_in = true;
				return mid;
			}
			else if (word < words[mid].word)
				return searchPos(word, left, mid-1, is_in);
			else  //word > words[mid].word
				return searchPos(word, mid+1, right, is_in);

		}
	}
	void parseSingleFile(const std::string& file, int index) {
		char buf[200];
		strcpy_s(buf, file.c_str());
		std::ifstream in(buf);
		std::string next;    bool inserted;

		while (in>>next) {
			tolowerCase(next);
			inserted = false;

			entry e;
			e.word = next;
			e.l.push_back(index);

			if (words.empty())
				words.push_back(e);
			else {
				int pos = searchPos(next, 0, words.size()-1, inserted);
				if (inserted) {
					if (words[pos].l.back() == index)
						continue;
					else words[pos].l.push_back(index);
				}
				else {
					if (pos == words.size())
						words.push_back(e);
					else {
						auto it = words.begin();
						std::advance(it, pos);
						words.insert(it, e);
					}
				}
			}
		}

	}
//public:
	InvertedIndex(const char* path) {
		parseFolder(path);
	}

	void parseDocuments() {
		for (unsigned int i = 2; i < files.size(); i++) /*first two are . and ..*/
			parseSingleFile(files[i], i-1);
	}

	std::list<int> simpleQuery(const std::string& str) {
		bool in=false;
		std::list<int> empty;

		int pos = searchPos(str, 0, words.size() - 1, in);
		if (in)
			return words[pos].l;
		else
			return empty;
	}

	void print(std::list<int>& lst) {
		for (int i : lst)
			std::cout << files[i+1] << ' '; /*elementi 0 i 1 vuv files sa . i ..=> file 1 ima index 2 vuv files*/
		std::cout << std::endl;
	}

	std::list<int> advancedQuery(const std::string& str) {
		std::list<int> query1, query2;
		std::string strquery;
		char op;
		std::stack<char> st;
		std::istringstream q(str);

		std::stack<std::list<int>> rpn;

		while (!q.eof())
		{
			char next = q.peek();
			if (isOperation(next))
			{
				 q >> op;
				while (!st.empty() && st.top() != '(' && weight(st.top()) >= weight(op))
				{
					char op2 = st.top();
					st.pop();
					query2 = rpn.top();
					rpn.pop();
					query1 = rpn.top();
					rpn.pop();
					switch (op2)
					{
					case '+': rpn.push(query1 + query2); break;
					case '*': rpn.push(query1 * query2); break;
					case '-': rpn.push(query1 - query2); break;
					}
					
				}

				st.push(op);
			}
			else if (isalpha(next))
			{
				q >> strquery;
				rpn.push(simpleQuery(strquery));
			}
			else if (next == '(')
			{
				 q.ignore();
				 st.push('(');
			}
			else if (next == ')')
			{
				q.ignore();
				while (st.top() != '(')
				{
					op = st.top();
					st.pop();
					query2 = rpn.top();
					rpn.pop();
					query1 = rpn.top();
					rpn.pop();
					switch (op)
					{
					case '+': rpn.push(query1 + query2); break;
					case '*': rpn.push(query1 * query2); break;
					case '-': rpn.push(query1 - query2); break;
					}
					
				}

				st.pop();
			}
			else
			{
				q.ignore();
			}
		}
		while (!st.empty()) {
			op = st.top();
			st.pop();
			query2 = rpn.top();
			rpn.pop();
			query1 = rpn.top();
			rpn.pop();
			switch (op)
			{
			case '+': rpn.push(query1 + query2); break;
			case '*': rpn.push(query1 * query2); break;
			case '-': rpn.push(query1 - query2); break;
			
			}
			
		}

		return rpn.top();

	}
	
};

void tolowerCase(std::string& str) {
	for (unsigned int i = 0; i < str.length(); i++) {
		str[i] = tolower(str[i]);
	}
}
int weight(char c) {
	switch (c) {
	   case '*': return 2;
	   case '+':
	   case '-': return 1;
	   default: return 10;
	}
}
bool isOperation(char c) {
	return c == '*' || c == '+' || c == '-';
}
int main() {
	InvertedIndex ind("C:\\Users\\elena\\Desktop\\Data");
	
	ind.parseDocuments();
	
	std::list<int> l1 = ind.simpleQuery("our");
	std::cout << "our -> ";
	if (l1.empty())
		std::cout << "No results found" << std::endl;
	else ind.print(l1);

	std::list<int> l2 = ind.simpleQuery("my");
	std::cout << "my -> ";
	if (l2.empty())
		std::cout << "No results found" << std::endl;
	else ind.print(l2);

	std::list<int> l3 = ind.simpleQuery("new");
	std::cout << "new -> ";
	if (l3.empty())
		std::cout << "No results found" << std::endl;
	else ind.print(l3);

	std::list<int> l4 = ind.simpleQuery("old");
	std::cout << "old -> ";
	if (l4.empty())
		std::cout << "No results found" << std::endl;
	else ind.print(l4);

	std::list<int> l5 = ind.simpleQuery("life");
	std::cout << "life -> ";
	if (l5.empty())
		std::cout << "No results found" << std::endl;
	else ind.print(l5);

	std::list<int> l6 = ind.simpleQuery("believe");
	std::cout << "believe -> ";
	if (l6.empty())
		std::cout << "No results found" << std::endl;
	else ind.print(l6);

	std::list<int> l7 = ind.simpleQuery("friend");
	std::cout << "friend -> ";
	if (l7.empty())
		std::cout << "No results found" << std::endl;
	else ind.print(l7);

	std::list<int> l8 = ind.simpleQuery("sun");
	std::cout << "sun -> ";
	if (l8.empty())
		std::cout << "No results found" << std::endl;
	else ind.print(l8);

	std::list<int> l9 = ind.advancedQuery("our - my");
	std::cout << "our - my -> ";
	if (l9.empty())
		std::cout << "No results found" << std::endl;
	else ind.print(l9);

	std::list<int> l10 = ind.advancedQuery("new + old");
	std::cout << "new + old -> ";
	if (l10.empty())
		std::cout << "No results found" << std::endl;
	else ind.print(l10);

	std::list<int> l11 = ind.advancedQuery("new * old");
	std::cout << "new * old -> ";
	if (l11.empty())
		std::cout << "No results found" << std::endl;
	else ind.print(l11);

	std::list<int> l12 = ind.advancedQuery("life * believe");
	std::cout << "life * believe -> ";
	if (l12.empty())
		std::cout << "No results found" << std::endl;
	else ind.print(l12);

	std::list<int> l13 = ind.advancedQuery("life * believe - friend");
	std::cout << "life * believe - friend -> ";
	if (l13.empty())
		std::cout << "No results found" << std::endl;
	else ind.print(l13);

	std::list<int> l14 = ind.advancedQuery("( believe + friend ) * sun");
	std::cout << "( believe + friend ) * sun -> ";
	if (l14.empty())
		std::cout << "No results found" << std::endl;
	else ind.print(l14);

	std::list<int> l15 = ind.advancedQuery("friend * sun");
	std::cout << "friend * sun -> ";
	if (l15.empty())
		std::cout << "No results found" << std::endl;
	else ind.print(l15);

	std::cout << std::endl;
	//user input
	std::string input; std::list<int> linput;
	while (getline(std::cin, input)) {
		linput = ind.advancedQuery(input);
		std::cout << input + " -> ";
		if(linput.empty())
			std::cout << "No results found" << std::endl;
		else ind.print(linput);
	}
	return 0;
}

