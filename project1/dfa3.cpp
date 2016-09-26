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
	map<string, pair<shared_ptr<state>, int> > delta;	
	bool accepting = false;
};

struct dfa {
	string name;
	vector<string> language;
	map<string, shared_ptr<state> > states;
	shared_ptr<state> start;
	shared_ptr<state> curr;
};

int main(int argc, char *argv[]) {
	if (argc < 3) {
		cout << "Usage: ./dfa dfa_file test_strings" << endl;
		return 1;
	}
	// initialize input file defining dfa
	ifstream dfa_file(argv[1]);
	dfa machine = {};
	string line;
	// reads in dfa description file
	if (dfa_file.is_open()) {
		// read machines name
		dfa_file >> machine.name;
		cout << "DFA: " << machine.name << endl;
		// read chars in language
		dfa_file >> line;
		cout << line << endl;
		boost::tokenizer<> tok(line);
		// puts all the strings in the language into dfa.language vector
		for (auto beg = tok.begin(); beg != tok.end(); ++beg) {
			machine.language.push_back(*beg);
		}
		// gets next line
		dfa_file >> line;
		cout << line << endl;
		// shared_ptr<state> s = {};
		state s;
		boost::tokenizer<> t(line);
		for (auto beg = t.begin(); beg != t.end(); ++beg) {
			s->value = *beg;
			// cout << *beg << endl;
			machine.states.insert(pair<string, shared_ptr<state> >(*beg, s));
		}
		// get all the states and push them into the states hash table of the dfa
		// shared_ptr<state> s;
		
		// declares the intial state of machine
		dfa_file >> line;
		cout << line << endl;
		machine.start = machine.states[line];
		// gets another line which are the accepting states
		dfa_file >> line;
		boost::tokenizer<> t2(line);
		// shared_ptr<state> acc;
		for (auto beg = t2.begin(); beg != t2.end(); ++beg) {
			shared_ptr<state> acc = machine.states[*beg];
			acc->accepting = true;
		}
		int i = 0;
		string input;
		while (dfa_file >> line) {
			boost::tokenizer<> tok(line);
			auto it = tok.begin();
			shared_ptr<state> curr = machine.states[*it];
			input = *(++it);
			shared_ptr<state> next_state = machine.states[*(++it)];
			cout << "RULE #" << i++ << ": (" << curr->value << ", " << input << ") -> " << next_state->value << endl;
			// inserts a pair into transition function
			// the pair is (input, (next_state, transition number))
			curr->delta.insert(pair<string, pair<shared_ptr<state>, int> >(input, make_pair(next_state, i)));
			// cout << machine.states["q1"].delta["0"].value << endl;
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
	shared_ptr<state> next_state;
	/*
	// while (in_file >> test) {
	in_file >> test;
		cout << "TEST: " << test << endl;
		// set current state to starting state
		machine.curr = machine.start;
		cout << "Starting at: " << machine.start->value;
		for (int i = 0; i < test.size(); i++) {
			input = test.at(i);
			next_state = machine.curr->delta[input];
			machine.curr = next_state;
			cout << " -> " << machine.curr->value;
		}
		cout << endl;
		if (machine.curr->accepting) cout << "ACCEPTED" << endl;
		else cout << "REJECTED" << endl;
	// }
	*/
	in_file.close();
	return 0;
}