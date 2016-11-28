#ifndef L_SYSTEM_H
#define L_SYSTEM_H

#include "LSystemGraphic.h"
#include "LSystemActions.h"
#include "LSystemConfig.h"

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

	// context of the LSystem
	struct LSystemContext
	{
		// current result
		std::string currentResult;

		// current symbol index in result
		int currentSymbolIndex;
	};

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

		//PrintResult();

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

			//PrintResult();
		}
	}

	void DecreaseIteration()
	{
		if (iterations > 0)
		{
			UndoStep();
			iterations--;

			//PrintResult();
		}
	}

	void Draw(octet::color_shader& shader)
	{
		if (results.size() > 0)
		{
			graphic.Draw(shader);
		}
	}

	void Clear()
	{
		config = nullptr;
		iterations = 0;
		graphic.Clear();
	}

private:

	void DoStep(const ProductionRules& rules)
	{
		std::string newResult; 
		
		const std::string& lastResult = results.top();

		// Apply the rules that each symbol creates
		for (int i = 0; i < lastResult.size(); i++)
		{
			AlphabetSymbol symbol = lastResult[i];

			auto& itr = rules.find(symbol);
			if (itr != rules.end())
			{
				// get the context
				LSystemContext context;
				context.currentResult = lastResult;
				context.currentSymbolIndex = i;

				// Append the derivation for this symbol
				AppendRuleDerivation(newResult, itr->second, context);
			}
			else
			{
				// The symbol does not have a rule attached so treat it as a constant
				newResult += symbol;
			}
		}

		// push the result
		results.push(newResult);

		// create the graphic
		CreateGraphic();
	}

	void AppendRuleDerivation(std::string& result, const RuleDerivations& ruleDerivations, const LSystemContext& context)
	{
		int derivation = 0;
		if (ruleDerivations.size() > 1)
		{
			// The symbol has more than one rule that could apply, so evaluate witch one to apply

			// Note: Assume that all input data has been validated and that is correct.
			// This is mainly to quickly check if we are dealing with an stochastic or context sensitive case
			bool isStochastic = false;
			bool isContextSensitive = false;

			for (const RuleDerivationParam& ruleDerivationParam : ruleDerivations)
			{
				if (!ruleDerivationParam.second.empty())
				{
					char firstDigit = ruleDerivationParam.second[0];
					if (isdigit(firstDigit)) // Note: We do not allow symbols that are numbers in order to make this work
					{
						isStochastic = true;
						break;
					}
					else
					{
						isContextSensitive = true;
						break;
					}
				}
			}

			// append the derivation according to each case
			if (isStochastic)
			{
				derivation = GetStochasticDerivation(ruleDerivations);
			}
			else if (isContextSensitive)
			{
				derivation = GetContextSensitiveDerivation(ruleDerivations, context);
			}
		}
	
		if (derivation >= 0)
		{
			result += ruleDerivations[derivation].first;
		}
		else
		{
			// no change so just add the current symbol
			result += context.currentResult[context.currentSymbolIndex];
		}
		
	}

	int GetStochasticDerivation(const RuleDerivations& ruleDerivations)
	{
		// convert all the probabilities to floats
		std::vector<float> probabilities;
		for (const RuleDerivationParam& ruleDerivationParam : ruleDerivations)
		{
			float prob = std::stof(ruleDerivationParam.second);
			probabilities.push_back(prob);
		}

		// create a random number
		srand(GetTickCount());
		float threshold = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));

		// add up probabilities until exceed or equal the threshold
		float sum = 0.0f;
		for (size_t i = 0; i < probabilities.size(); i++)
		{
			sum += probabilities[i];
			if (sum >= threshold)
			{
				return (int)i;
			}
		}

		return 0;
	}

	int GetContextSensitiveDerivation(const RuleDerivations& ruleDerivations, const LSystemContext& context)
	{
		const std::string& result = context.currentResult;

		// build derivation string
		AlphabetSymbol currentSymbol = result[context.currentSymbolIndex];
		AlphabetSymbol previousSymbol = context.currentSymbolIndex - 1 > 0 ? result[context.currentSymbolIndex - 1] : ' ';
		AlphabetSymbol nextSymbol = context.currentSymbolIndex + 1 < result.size() ? result[context.currentSymbolIndex + 1] : ' ';

		std::string derivation(1, previousSymbol);
		derivation.append("<" + currentSymbol);
		derivation.append(">" + nextSymbol);

		// return the derivation that matches with the built one
		for (size_t i = 0; i < ruleDerivations.size(); i++)
		{
			if (derivation == ruleDerivations[i].second)
			{
				return (int)i;
			}
		}

		return -1;
	}

	void UndoStep()
	{
		results.pop();
		CreateGraphic();
	}

	void CreateGraphic()
	{
		// clear current graphic
		graphic.Clear();

		// execute all the actions attached to current result
		const std::string& result = results.top();
		actions.Execute(result, graphic, *config);

		// create the graphic
		graphic.Create();
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

