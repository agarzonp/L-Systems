#ifndef L_SYSTEM_CONFIG_PARSER_H
#define L_SYSTEM_CONFIG_PARSER_H

#include "LSystemConfig.h"

#include "../CSVParser/CSVParser.h"

class LSystemConfigParser
{
public:
	LSystemConfigParser() {};
	~LSystemConfigParser() {};

	void LoadLSystemConfig(const char* filePath, LSystemConfig& config)
	{
		agarzonp::CSVParser parser(filePath);

		if (parser.IsValid())
		{
			// FIXME: parse defensively!

			config.n_ = config.n = atoi(parser[0][1]);
			config.leftAngle_  = config.leftAngle = (float)atof(parser[1][1]);
			config.rightAngle_ = config.rightAngle = (float)atof(parser[2][1]);

			std::string symbols = parser[3][1];
			std::string actions = parser[4][1];

			for (int i = 0; i < symbols.size(); i += 2)
			{
				std::string actionId(1, actions[i]); // Note: this will fail if the id is more than one letter

				config.symbolsToActions.insert(std::pair<AlphabetSymbol, std::string>(symbols[i], actionId));
			}

			config.axiom = parser[5][1];

			// rules
			for (int rule = 6; rule < parser.NumRows(); rule++)
			{
				std::string predecessor = parser[rule][0];
				std::string sucessor = parser[rule][1];

				config.rules.insert(std::pair<AlphabetSymbol, std::string>(predecessor[0], sucessor));
			}
		}
		
	}

private:

};

#endif // !L_SYSTEM_CONFIG_PARSER

