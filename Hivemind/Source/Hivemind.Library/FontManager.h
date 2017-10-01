#pragma once


class FontManager
{

public:

	/**
	 * Singleton accessor
	 * @Return: A pointer to the instance of the singleton
	 */
	static FontManager* GetInstance();

#pragma region Construction/Copy/Assignment

private:

    FontManager();

public:

	~FontManager();

    FontManager(const FontManager& rhs) = delete;

    FontManager& operator=(const FontManager& rhs) = delete;

    FontManager(FontManager&& rhs) = delete;

    FontManager& operator=(FontManager&& rhs) = delete;

#pragma endregion

	/**
	 * Accessor method for hack font
	 * @Return: Reference to the locally stored hack font
	 */
	sf::Font& Hack();

private:

	// Singleton instance
	static FontManager* sInstance;

	/**
	 *  Loads all fonts and stores them locally
	 */
	void Init();

	sf::Font mHack;
};

