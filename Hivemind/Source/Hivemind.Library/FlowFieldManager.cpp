#include "pch.h"
#include "FlowFieldManager.h"

/**
*	@Author: Dale Diaz
*	@Date: 7/21/2017
*/

FlowFieldManager* FlowFieldManager::sInstance = nullptr;

FlowFieldManager::FlowFieldManager()
{
	std::random_device device;
	mGenerator = std::default_random_engine(device());
}

FlowFieldManager::~FlowFieldManager()
{
	for (auto iter = mFlowFields.begin(); iter != mFlowFields.end(); ++iter)
	{
		delete *iter;
	}
	mFlowFields.clear();
}

FlowFieldManager* FlowFieldManager::GetInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new FlowFieldManager();
		sInstance->Init();
	}
	return sInstance;
}

FlowField FlowFieldManager::GetField()
{
	std::uniform_int_distribution<int> distribution(0, static_cast<int>(mFlowFields.size() - 1));
	return *mFlowFields[distribution(mGenerator)];
}

void FlowFieldManager::Init()
{
	for (int i = 0; i < 100; i++)
	{
		mFlowFields.push_back(new FlowField(sf::Vector2f()));
	}
}
