#include "pch.h"
#include "FontManager.h"


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