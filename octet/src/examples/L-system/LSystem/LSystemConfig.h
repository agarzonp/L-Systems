#ifndef L_SYSTEM_CONFIG_H
#define L_SYSTEM_CONFIG_H

#include <map>

typedef char AlphabetSymbol;
typedef std::map<AlphabetSymbol, std::string> ProductionRules;
typedef std::map<AlphabetSymbol, std::string> Actions;

struct LSystemConfig
{
	// number of iterations
	int n;

	// angle of variation
	float d;

	// maps a symbol of the alphabet to an id of an LSystemAction
	std::map<AlphabetSymbol, std::string> symbolsToActions;

	// starting symbol
	std::string axiom;

	// production rules: maps a symbol of the alphabet to the set of symbols that the rule derives
	std::map<AlphabetSymbol, std::string> rules;
};

#endif // !L_SYSTEM_CONFIG_H

