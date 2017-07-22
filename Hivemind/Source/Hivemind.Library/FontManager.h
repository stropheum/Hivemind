#pragma once
class FontManager
{
public:

	/// Singleton accessor
	/// @Return: A pointer to the instance of the singleton
	static FontManager* GetInstance();

	/// Destructor
	~FontManager();

	/// Accessor method for hack font
	/// @Return: Reference to the locally stored hack font
	sf::Font& Hack();

private:

	/// Singleton instance
	static FontManager* sInstance;

	/// Constructor
	FontManager();

	/// Loads all fonts and stores them locally
	void Init();

	sf::Font mHack;
};

