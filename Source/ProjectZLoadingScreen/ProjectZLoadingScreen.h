
#ifndef PROJECTZ_LOADING_SCREEN_H
#define PROJECTZ_LOADING_SCREEN_H

#include "ModuleInterface.h"

/** Module interface for this game's loading screens */
class IProjectZLoadingScreenModule : public IModuleInterface
{
public:
	/** Kicks off the loading screen for in game loading (not startup) */
	virtual void StartInGameLoadingScreen() = 0;
};


#endif // !
