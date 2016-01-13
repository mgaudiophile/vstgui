#pragma once

#include "vstgui/standalone/helpers/windowlistener.h"

//------------------------------------------------------------------------
namespace MyApp {

//------------------------------------------------------------------------
class About : public VSTGUI::Standalone::WindowListenerAdapter
{
public:
	static void show ();

private:
	static std::unique_ptr<About> gInstance;

	using IWindow = VSTGUI::Standalone::IWindow;
	using WindowPtr = VSTGUI::Standalone::WindowPtr;

	void onClosed (const IWindow& window) override;

	WindowPtr window;
};

//------------------------------------------------------------------------
} // MyApp
