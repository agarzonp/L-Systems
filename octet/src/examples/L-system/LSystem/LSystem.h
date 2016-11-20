#ifndef L_SYSTEM_H
#define L_SYSTEM_H

#include "LSystemConfig.h"
#include "LSystemConfigParser.h"
#include "LSystemActions.h"

class LSystem
{
	// number of iterations
	int iterations;

	// contains the result for each iteration
	std::string result;

	// the actions
	LSystemActions actions;

public:
	LSystem()
	{
		Clear();
	}

	~LSystem()
	{
	}


	void Execute(const LSystemConfig& config)
	{
		Clear();

		// start with the axiom
		result = config.axiom;

		PrintResult();

		while (iterations < config.n)
		{
			DoStep(config.rules);
			iterations++;

			PrintResult();
		}
	}

	void DoStep(const ProductionRules& rules)
	{
		std::string newResult;

		for (int i = 0; i < result.size(); i++)
		{
			AlphabetSymbol symbol = result[i];

			auto& itr = rules.find(symbol);
			if (itr != rules.end())
			{
				newResult += itr->second;
			}
			else
			{
				// Treat the symbol as a constant
				newResult += symbol;
			}
		}

		result = newResult;
	}

private:

	void Clear()
	{
		iterations = 0;
	}

	void PrintResult()
	{
		printf("\nIteration %d", iterations);
		printf("\n=============");
		printf("\n%s\n", result.c_str());
	}

};

#endif // !L_SYSTEM_H

