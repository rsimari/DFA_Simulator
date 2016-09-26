#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <map>
#include <boost/tokenizer.hpp>
using namespace std;

struct state {
	string value;
	map<string, pair<string, int> > delta;	
	bool accepting = false;
};

struct dfa {
	string name;
	vector<string> language;
	map<string, state> states;
	string start; // key of start and current state
	string curr;
};

int main(int argc, char *argv[]) {
	if (argc < 3) {
		cout << "Usage: ./dfa dfa_file test_strings" << endl;
		return 1;
	}
	// initialize input file defining dfa
	ifstream dfa_file(argv[1]);
	dfa machine;
	string line;
	state s;
	// reads in dfa description file
	if (dfa_file.is_open()) {
		// read machines name
		getline(dfa_file, machine.name);
		cout << "DFA: " << machine.name << endl;
		// read chars in language
		dfa_file >> line;
		boost::tokenizer<> tok(line);
		// puts all the strings in the language into dfa.language vector
		for (auto beg = tok.begin(); beg != tok.end(); ++beg) {
			machine.language.push_back(*beg);
		}
		// gets next line
		dfa_file >> line;
		boost::tokenizer<> t(line);
		// get all the states and push them into the states hash table of the dfa
		state s;
		for (auto beg = t.begin(); beg != t.end(); ++beg) {
			s.value = *beg;
			machine.states.insert(pair<string, state>(*beg, s));
		}
		// declares the intial state of machine
		dfa_file >> line;
		machine.start = line;
		// gets another line which are the accepting states
		dfa_file >> line;
		boost::tokenizer<> t2(line);
		state acc;
		for (auto beg = t2.begin(); beg != t2.end(); ++beg) {
			machine.states[*beg].accepting = true;
		}
		int i = 0;
		string input;
		string next_state;
		string curr;
		while (dfa_file >> line) {
			boost::tokenizer<> tok(line);
			auto it = tok.begin();
			curr = *it;
			input = *(++it);
			next_state = *(++it);
			cout << "RULE #" << i << ": (" << machine.states[curr].value << ", " << input << ") -> " << machine.states[next_state].value << endl;
			// inserts a pair into transition function
			// the pair is (input, (next_state, transition number))
			machine.states[curr].delta.insert(pair<string, pair<string, int> >(input, make_pair(machine.states[next_state].value, i++)));
		}
		dfa_file.close();
	} else {
		cout << argv[1] << " cannot be opened" << endl;
		return 1;
	}
	// takes in input and checks if the machine accepts string
	ifstream in_file(argv[2]);
	string test;
	string input;
	int transition;
	string next_state;
	while (in_file >> test) {
	in_file >> test;
		cout << "TEST: " << test << endl;
		// set current state to starting state
		machine.curr = machine.start;
		cout << "Starting at: " << machine.curr << endl;
		for (int i = 0; i < test.size(); i++) {
			input = test.at(i);
			next_state = machine.states[machine.curr].delta[input].first;
			transition = machine.states[machine.curr].delta[input].second;
			cout << "Step #" << i+1 << " - Rule " << transition << ": " << machine.curr << ", " << input << ", ";
			machine.curr = machine.states[next_state].value;
			cout << machine.curr << endl;
		}
		cout << endl;
		if (machine.states[machine.curr].accepting) cout << "ACCEPTED" << endl;
		else cout << "REJECTED" << endl;
	}
	in_file.close();
	return 0;
}