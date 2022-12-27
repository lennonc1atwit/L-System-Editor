#pragma once
#define _USE_MATH_DEFINES
#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <set>
#include <stack>
#include <cmath>
#include <iostream>

class App
{
public:
	App(sf::RenderWindow* win);
	~App();

	void renderUI();
	void renderImage();

	void popupSymbol();
	void popupRule();
	void showMainMenuBar();

	void generateSystem();

private:
	sf::RenderWindow* mRWin;
	sf::RenderTexture mRt;

	ImVec2 viewportSize;
	int mIterations;
	std::string mSystemString;

	bool symbolPopupOpen = false;
	bool rulePopupOpen = false;

	std::string mAxiom;
	std::set<char> mSymbols;
	std::map<char, std::string> mRules;
};

