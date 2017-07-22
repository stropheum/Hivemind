#pragma once
#include "FlowField.h"

/**
*	@Author: Dale Diaz
*	@Date: 7/21/2017
*/

class FlowFieldManager
{
private:

	/**
	 *  Constructor
	 */
	FlowFieldManager();

public:

	/**
	 *  Destructor
	 */
	~FlowFieldManager();

	/**
	 * Accessor for singleton instance
	 * @Return: A pointer to the instance of the singleton
	 */
	static FlowFieldManager* GetInstance();

	/**
	 * Gets a random field from the vector of generated flow fields
	 * @Return: A copy of one of the generated flow fields
	 */
	FlowField GetField();

private:

	// Singleton instance
	static FlowFieldManager* sInstance;

	/**
	 *  Initializes the flow fields for later access
	 */
	void Init();

	std::vector<FlowField*> mFlowFields;
	std::default_random_engine mGenerator;
};

