#ifndef L_SYSTEM_TREE_COLOUR_H
#define L_SYSTEM_TREE_COLOUR_H

#include <vector>

class  LSystemTreeColour
{
	// colours
	std::vector<octet::vec4> branchColours;
	std::vector<octet::vec4> leafColours;

	int branchColourIndex;
	int leafColourIndex;

	static LSystemTreeColour* s_instance;

public:

	// Singleton
	static LSystemTreeColour* GetInstance()
	{
		if (!s_instance)
		{
			s_instance = new LSystemTreeColour();
		}

		return s_instance;
	}

	octet::vec4 GetLeafColour(int branchDepth)
	{
		size_t index = branchDepth >= leafColours.size() ? leafColours.size() - 1 : branchDepth;
		return leafColours[index];
	}

	octet::vec4 GetBranchColour(int branchDepth)
	{
		size_t index = branchDepth >= branchColours.size() ? branchColours.size() - 1: branchDepth;
		return branchColours[index];
	}

protected:
	LSystemTreeColour()
		: branchColourIndex(0)
		, leafColourIndex(0)
	{
		RegisterBranchColours();
		RegisterLeafColours();

		s_instance = this;
	}

private:

	void RegisterBranchColours()
	{
		branchColours.push_back(octet::vec4(0.35f, 0.16f, 0.14f, 1));
		branchColours.push_back(octet::vec4(0.13f, 0.37f, 0.31f, 1)); 
		branchColours.push_back(octet::vec4(0.36f, 0.25f, 0.20f, 1));
		branchColours.push_back(octet::vec4(0.419608f, 0.556963f, 0.137255f, 1)); 
		branchColours.push_back(octet::vec4(0.13f, 0.37f, 0.31f, 1));
		branchColours.push_back(octet::vec4(0.59f, 0.41f, 0.31f, 1));
		branchColours.push_back(octet::vec4(0.35f, 0.16f, 0.14f, 1));
		branchColours.push_back(octet::vec4(0.65f, 0.5f, 0.39f, 1));
		branchColours.push_back(octet::vec4(0.89f, 0.46f, 0.2f, 1));
		
	}

	void RegisterLeafColours()
	{
		//leafColours.push_back(octet::vec4(0.13f, 0.37f, 0.31f, 1));
		//leafColours.push_back(octet::vec4(0.29f, 0.46f, 0.43f, 1));
		leafColours.push_back(octet::vec4(0.32f, 0.49f, 0.46f, 1));
		//leafColours.push_back(octet::vec4(0.85f, 0.53f, 0.1f, 1));
		//leafColours.push_back(octet::vec4(0.6f, 0.8f, 0.196078f, 1));
		leafColours.push_back(octet::vec4(0.0f, 1.0f, 0.498039f, 1)); 
		//leafColours.push_back(octet::vec4(0.498039f, 1.0f, 0.0f, 1));
		leafColours.push_back(octet::vec4(0.419608f, 0.556963f, 0.137255f, 1)); 
		//leafColours.push_back(octet::vec4(0.576471f, 0.858824f, 0.439216f, 1));
		leafColours.push_back(octet::vec4(0.137255f, 0.556863f, 0.137255f, 1)); 
		leafColours.push_back(octet::vec4(0.419608f, 0.556963f, 0.137255f, 1));
		//leafColours.push_back(octet::vec4(0.184314f, 0.309804f, 0.184314f, 1));
		leafColours.push_back(octet::vec4(0.52f, 0.37f, 0.26f, 1));
	}

};


LSystemTreeColour* LSystemTreeColour::s_instance = nullptr;

#endif // !L_SYSTEM_TREE_COLOUR_H

