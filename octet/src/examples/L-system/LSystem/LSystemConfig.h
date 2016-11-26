#ifndef L_SYSTEM_CONFIG_H
#define L_SYSTEM_CONFIG_H

#include <map>

typedef char AlphabetSymbol;
typedef std::map<AlphabetSymbol, std::string> ProductionRules;
typedef std::map<AlphabetSymbol, std::string> Actions;

struct LSystemConfig
{
	// original number of iterations
	int n_;

	// original left angle
	float leftAngle_;

	// original right angle
	float rightAngle_;

	// variable number of iterations
	int n;

	// variable left angle for heading variation
	float leftAngle;

	// variable right angle for heading variation
	float rightAngle;

	// maps a symbol of the alphabet to an id of an LSystemAction
	std::map<AlphabetSymbol, std::string> symbolsToActions;

	// starting symbol
	std::string axiom;

	// production rules: maps a symbol of the alphabet to the set of symbols that the rule derives
	std::map<AlphabetSymbol, std::string> rules;

	void Reset()
	{
		n = n_;
		leftAngle = leftAngle_;
		rightAngle = rightAngle_;
	}
};

#endif // !L_SYSTEM_CONFIG_H

