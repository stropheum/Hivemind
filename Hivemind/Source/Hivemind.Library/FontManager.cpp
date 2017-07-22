#include "pch.h"
#include "FontManager.h"

/**
*	@Author: Dale Diaz
*	@Date: 7/21/2017
*/

FontManager* FontManager::sInstance = nullptr;

FontManager::FontManager()
{
}

FontManager::~FontManager()
{
}

sf::Font& FontManager::Hack()
{
	return mHack;
}

FontManager* FontManager::GetInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new FontManager();
		sInstance->Init();
	}

	return sInstance;
}

void FontManager::Init()
{
	if (!mHack.loadFromFile("Hack-Regular.ttf"))
	{
		throw new std::exception("Error loading font file");
	}
}