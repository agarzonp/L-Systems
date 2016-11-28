#ifndef L_SYSTEM_CONFIG_H
#define L_SYSTEM_CONFIG_H

#include <map>

// typedefs for convenience
typedef char AlphabetSymbol;
typedef std::string RuleDerivation;
typedef std::string RuleParam;

// typedef for RuleDerivationParam
typedef std::pair< RuleDerivation, RuleParam> RuleDerivationParam;
// ForExample:
// F(0.5) -> FXF
//
// Will be:
// 
//std::pair< "FXF" , "0.5"> 

// typedef for RuleDerivations
typedef std::vector<RuleDerivationParam> RuleDerivations;

// typedef for ProductionRules
typedef std::map<AlphabetSymbol, RuleDerivations > ProductionRules;

// maps a symbol of the alphabet to the set of symbols that the rule produce
// Each symbol can produce more than one rule. 
// The system will evaluate all the RuleParam and decide that RuleDerivation to apply
// For example:
//
// A deterministic LSystem will be:
//
// F -> XFX
// X -> F
//
// std::map< 'F', [ ["XFX", ""] ] > 
// std::map< 'X', [ ["F", ""] ] > 
//
// A stochastic LSystem will be:
//
// F (0.5)-> FF
// F (0.5)-> X
// X -> F
//
// std::map< 'F', [ ["FF", "0.5"], ["X", "0.5"] ] >
// std::map< 'X', [ ["F", ""] ] > 
//
// A context senstive LSystem will be:
//
// F (X < F > F) -> FF
// F -> X
// X -> F
//
// std::map< 'F', [ ["FF", "X<F>F"], ["X", ""] ] >
// std::map< 'X', [ ["F", ""] ] > 

// Struct for defining the configuration of the LSystem
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

	// production rules
	ProductionRules rules;

	// reset to original values
	void Reset()
	{
		n = n_;
		leftAngle = leftAngle_;
		rightAngle = rightAngle_;
	}
};

#endif // !L_SYSTEM_CONFIG_H

