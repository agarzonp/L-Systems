#ifndef L_SYSTEM_H
#define L_SYSTEM_H

#include "LSystemGraphic.h"
#include "LSystemActions.h"
#include "LSystemConfig.h"
#include "LSystemConfigParser.h"

#include <stack>

class LSystem
{
	// number of iterations
	int iterations;

	// contains the result for each iteration
	std::stack<std::string> results;

	// the actions
	LSystemActions actions;

	// configuration
	LSystemConfig* config;

	// the graphic to be drawn
	LSystemGraphic graphic;

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

	void Execute(LSystemConfig& config, ExecuteType type = EXECUTE_STEP_BY_STEP)
	{
		Clear();

		this->config = &config;

		// start with the axiom
		results.push(config.axiom);

		// create the graphic
		CreateGraphic();

		PrintResult();

		if (type == EXECUTE_IMMEDIATE)
		{
			while (iterations < config.n)
			{
				IncreaseIteration();
			}
		}
	}

	void IncreaseIteration()
	{
		if (iterations < config->n)
		{
			DoStep(config->rules);
			iterations++;

			PrintResult();
		}
	}

	void DecreaseIteration()
	{
		if (iterations > 0)
		{
			UndoStep();
			iterations--;

			PrintResult();
		}
	}

	void Draw()
	{
		if (results.size() > 0)
		{
			graphic.Draw();
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
		
		const std::string& lastResult = results.top();

		for (int i = 0; i < lastResult.size(); i++)
		{
			AlphabetSymbol symbol = lastResult[i];

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

		results.push(newResult);

		CreateGraphic();
	}

	void UndoStep()
	{
		results.pop();
		CreateGraphic();
	}

	void CreateGraphic()
	{
		graphic.Clear();

		const std::string& result = results.top();

		for (int i = 0; i < result.size(); i++)
		{
			AlphabetSymbol symbol = result[i];

			const std::string& actionId = config->symbolsToActions[symbol];

			LSystemAction* action = actions.GetAction(actionId.c_str());
			action->Execute(graphic, *config);
		}
	}

	void PrintResult()
	{
		printf("\nIteration %d", iterations);
		printf("\n=============");
		printf("\n%s\n", results.top().c_str());
		
		graphic.Print();
	}

};

#endif // !L_SYSTEM_H

