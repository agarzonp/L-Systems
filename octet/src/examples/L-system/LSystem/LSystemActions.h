#ifndef L_SYSTEM_ACTIONS_H
#define L_SYSTEM_ACTIONS_H

#include "Actions/Actions.h"

#include <map>
#include <string>
#include <cassert>

class LSystemActions
{
	std::map<std::string, LSystemAction*> actionsMap;

public:
	LSystemActions()
	{
		RegisterActions();
	}

	~LSystemActions()
	{
		for (auto& pair : actionsMap)
		{
			delete pair.second;
		}
	}

	void Execute(const std::string& symbols, LSystemGraphic& graphic, LSystemConfig& config)
	{
		for (int i = 0; i < symbols.size(); i++)
		{
			AlphabetSymbol symbol = symbols[i];

			const std::string& actionId = config.symbolsToActions[symbol];

			// Execute the action
			ExecuteAction(actionId.c_str(), graphic, config);
		}
	}

private:

	void ExecuteAction(const char* actionId, LSystemGraphic& graphic, const LSystemConfig& config)
	{
		LSystemAction* action = GetAction(actionId);
		if (action)
		{
			action->Execute(graphic, config);
		}
	}

	LSystemAction* GetAction(const char* id)
	{
		auto& mapItr = actionsMap.find(id);
		if (mapItr != actionsMap.end())
		{
			return mapItr->second;
		}

		assert(false); // Has the requested action being registered?

		// return the dummy action if no action was found 
		return actionsMap[0];
	}

	void RegisterActions()
	{
		// Dummy action
		actionsMap.insert(std::pair<std::string, LSystemAction*> (DummyAction::GetID(), new DummyAction()));

		// Below all the actions to be added
		actionsMap.insert(std::pair<std::string, LSystemAction*>(MoveForwardAction::GetID(), new MoveForwardAction()));
		actionsMap.insert(std::pair<std::string, LSystemAction*>(PopStateAction::GetID(), new PopStateAction()));
		actionsMap.insert(std::pair<std::string, LSystemAction*>(PushStateAction::GetID(), new PushStateAction()));
		actionsMap.insert(std::pair<std::string, LSystemAction*>(TurnLeftAction::GetID(), new TurnLeftAction()));
		actionsMap.insert(std::pair<std::string, LSystemAction*>(TurnRightAction::GetID(), new TurnRightAction()));
	}

};

#endif // !L_SYSTEM_ACTIONS_H
