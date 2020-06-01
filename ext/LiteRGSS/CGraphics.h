#ifndef CGraphics_H
#define CGraphics_H
#include <memory>
#include "ruby.h"
#include "CShaderFactory.h"
#include "CGraphicsConfig.h"
#include "CGraphicsUpdateMessage.h"

#include "Views/DisplayWindow.h"

class CGraphics {
public:
	static CGraphics& Get() {
		static CGraphics cg;
		return cg;
	}

	virtual ~CGraphics() = default;

	unsigned long frameCount() const { return frame_count; }
	void setFrameCount(unsigned long frameCount) { frame_count = frameCount; }
	void setBrightness(unsigned char brightness) { m_gameWindow.setBrightness(brightness); }
	unsigned char brightness() const { return m_gameWindow.brightness(); }
	long screenWidth() const { return m_gameWindow.screenWidth(); }
	long screenHeight() const { return m_gameWindow.screenHeight(); }
	bool smoothScreen() const { return m_gameWindow.smoothScreen(); }
	double scale() const { return m_gameWindow.scale(); }
	auto frameRate() const { return m_gameWindow.frameRate(); }

	void init();
	void stop();
	bool isGameWindowOpen() const;
	void update(VALUE self, bool input = true);
	void updateOnlyInput(VALUE self);

	void windowDraw();

	VALUE takeSnapshot();
	void freeze(VALUE self);
	void transition(VALUE self, int argc, VALUE* argv);
	void resizeScreen(int width, int height); 
	void setShader(sf::RenderStates* shader);

	template <class T, class ... Args>
	T add(Args&& ... args) {
		return T::create(m_gameWindow, std::forward<Args>(args)...);
	}

	template <class T, class ... Args>
	T addView(Args&& ... args) {
		return m_gameWindow.template addView<T>(m_gameWindow, std::forward<Args>(args)...);
	}

	std::unique_ptr<sf::Shader> createUniqueShader() const;
	sf::Shader* createNewShader() const;
	bool areShadersEnabled() const;

private:
	std::unique_ptr<GraphicsUpdateMessage> draw();
	void warnIfGraphicsUpdate() const;
	CGraphics();

	void manageErrorMessage(VALUE self, const GraphicsUpdateMessage& message);
	sf::RenderTarget& updateDrawPreProc(sf::View& defview);
	void updateDrawPostProc();
	void updateProcessEvent(GraphicsUpdateMessage& message);
   
	bool InsideGraphicsUpdate = false;
	unsigned long frame_count = 0;

	cgss::MainEventDispatcher m_eventDispatcher;
	cgss::DisplayWindow m_gameWindow;

	CGraphicsConfigLoader m_configLoader;
	CShaderFactory m_shaderFactory;	
};

#endif
