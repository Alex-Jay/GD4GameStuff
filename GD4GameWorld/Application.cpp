#include "Application.hpp"
#include "Utility.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "TitleState.hpp"
#include "MenuState.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"
#include "SettingsState.hpp"


const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
	:mWindow(sf::VideoMode(640, 480), "States", sf::Style::Close)
	, mTextures()
	, mFonts()
	, mPlayer()
	, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer))
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
{
	mWindow.setKeyRepeatEnabled(false);
	mFonts.load(FontIDs::Main, "Media/Sansation.ttf");
	mTextures.load(TextureIDs::TitleScreen, "Media/Textures/TitleScreen.png");
	mTextures.load(TextureIDs::ButtonNormal, "Media/Textures/ButtonNormal.png");
	mTextures.load(TextureIDs::ButtonSelected, "Media/Textures/ButtonSelected.png");
	mTextures.load(TextureIDs::ButtonPressed, "Media/Textures/ButtonPressed.png");
	mStatisticsText.setFont(mFonts.get(FontIDs::Main));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(20);
	registerStates();
	mStateStack.pushState(StateIDs::Title);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processInput();
			update(TimePerFrame);
			if (mStateStack.isEmpty())
			{
				mWindow.close();
			}
		}
		updateStatistics(elapsedTime);
		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);
		
		if (event.type == sf::Event::Closed)
		{
			mWindow.close();
		}
	}
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Application::render()
{
	mWindow.clear();
	mStateStack.draw();
	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);
	mWindow.display();
}

void Application::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString("Frames/Second = " + std::to_string(mStatisticsNumFrames) + "\n" + "Time/Update = " + std::to_string((mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames)) + "us");
		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(StateIDs::Title);
	mStateStack.registerState<MenuState>(StateIDs::Menu);
	mStateStack.registerState<GameState>(StateIDs::Game);
	mStateStack.registerState<PauseState>(StateIDs::Pause);
	mStateStack.registerState<SettingsState>(StateIDs::Settings);
}
