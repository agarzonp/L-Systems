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

	// configuration
	const LSystemConfig* config;

public:
	LSystem()
	{
		Clear();
	}

	~LSystem()
	{
	}

	enum ExecuteType
	{
		EXECUTE_STEP_BY_STEP,
		EXECUTE_IMMEDIATE
	};

	void Execute(const LSystemConfig& config, ExecuteType type = EXECUTE_STEP_BY_STEP)
	{
		Clear();

		this->config = &config;

		// start with the axiom
		result = config.axiom;

		PrintResult();

		if (type == EXECUTE_IMMEDIATE)
		{
			while (iterations < config.n)
			{
				Iterate();
			}
		}
	}

	void Iterate()
	{
		if (iterations < config->n)
		{
			DoStep(config->rules);
			iterations++;

			PrintResult();
		}
	}

private:

	void Clear()
	{
		config = nullptr;
		iterations = 0;
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

	void PrintResult()
	{
		printf("\nIteration %d", iterations);
		printf("\n=============");
		printf("\n%s\n", result.c_str());
	}

};

#endif // !L_SYSTEM_H

