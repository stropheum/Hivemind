#include "pch.h"
#include "CollisionGrid.h"

/**
*	@Author: Dale Diaz
*	@Date: 7/21/2017
*/

using namespace std;

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

vector<CollisionNode*> CollisionGrid::NeighborsOf(CollisionNode* const node) const
{
	vector<CollisionNode*> neighbors;

	for (int i = 0; i < mGridSize; i++)
	{
		for (int j = 0; j < mGridSize; j++)
		{
			if (&mGrid[i][j] == node)
			{
				if ((i - 1) >= 0)
				{	// left
					neighbors.push_back(&mGrid[i - 1][j]);

					if ((j - 1) >= 0)
					{	// top-left
						neighbors.push_back(&mGrid[i - 1][j - 1]);
					}
					if ((j + 1) < mGridSize)
					{	// bottom-left
						neighbors.push_back(&mGrid[i - 1][j + 1]);
					}
				}
				if ((i + 1) < mGridSize)
				{	// right
					neighbors.push_back(&mGrid[i + 1][j]);

					if ((j + 1) < mGridSize)
					{	// bottom-right
						neighbors.push_back(&mGrid[i + 1][j + 1]);
					}
					if (j - 1 >= 0)
					{	// top-right
						neighbors.push_back(&mGrid[i + 1][j - 1]);
					}
				}
				if ((j - 1 >= 0))
				{	// top
					neighbors.push_back(&mGrid[i][j - 1]);
				}
				if ((j + 1 < mGridSize))
				{	// bottom
					neighbors.push_back(&mGrid[i][j + 1]);
				}
				break;
			}
		}
	}

	return neighbors;
}
