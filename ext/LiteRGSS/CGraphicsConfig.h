#ifndef CGraphicsConfig_H
#define CGraphicsConfig_H

#include <tuple>
#include <SFML/Graphics.hpp>
#include "LiteCGSS/Configuration/DisplayWindowSettings.h"

/*
struct CGraphicsVideoConfig {
	sf::VideoMode vmode;
	unsigned int width = 640;
	unsigned int height = 480;
	double scale = 1.0;
};

struct CGraphicsConfig {
	CGraphicsVideoConfig video;
	bool smoothScreen = false;
	sf::String title;
	unsigned int frameRate = 60;
	bool vSync = false;
	bool fullscreen = false;
};
*/

class CGraphicsConfigLoader {
public:
	cgss::DisplayWindowSettings load() const;

private:
	cgss::DisplayWindowVideoSettings loadVideoFromConfigs() const;
	bool loadSmoothScreenFromConfigs() const;
	std::string loadTitleFromConfigs() const;
	unsigned int loadFrameRateFromConfigs() const;
	bool loadVSYNCFromConfigs() const;
	bool loadFullScreenFromConfigs() const;
};

#endif