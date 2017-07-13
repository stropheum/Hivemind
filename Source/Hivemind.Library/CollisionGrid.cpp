#include "pch.h"
#include "CollisionGrid.h"


CollisionGrid* CollisionGrid::sInstance = nullptr;

CollisionGrid::CollisionGrid():
	mGridSize(50), mNodeSize(2000), mGridOrigin(-(mGridSize / 2.0f * mNodeSize), -(mGridSize / 2.0f * mNodeSize)),
	mVisible(false)
{
	mGrid = new CollisionNode*[mGridSize];
	for (int i = 0; i < mGridSize; i++)
	{
		mGrid[i] = new CollisionNode[mGridSize];
	}

	for (int i = 0; i < mGridSize; i++)
	{
		for (int j = 0; j < mGridSize; j++)
		{
			mGrid[i][j].SetSize(mNodeSize);
			sf::Vector2f position(i * mNodeSize, j * mNodeSize);
			mGrid[i][j].SetPosition(mGridOrigin + position);
		}
	}
}

CollisionGrid::~CollisionGrid()
{
	for (int i = 0; i < mGridSize; i++)
	{
		delete[] mGrid[i];
	}
	delete[] mGrid;
}

CollisionGrid* CollisionGrid::GetInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new CollisionGrid();
	}
	return sInstance;
}

void CollisionGrid::Render(sf::RenderWindow& window) const
{
	if (mVisible)
	{
		for (int i = 0; i < mGridSize; i++)
		{
			for (int j = 0; j < mGridSize; j++)
			{
				mGrid[i][j].Render(window);
			}
		}
	}
}

void CollisionGrid::ToggleGridVisualization()
{
	mVisible = !mVisible;
}

CollisionNode* CollisionGrid::CollisionNodeFromPosition(const sf::Vector2f& position) const
{
	sf::Vector2f nodeOffset = (position - mGridOrigin) / static_cast<float>(mNodeSize);
	return &mGrid[static_cast<int>(nodeOffset.x)][static_cast<int>(nodeOffset.y)];
}
