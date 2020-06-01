#include <cassert>
#include "ruby_common.h"
#define LITERGSS_INCLUDE_RUBY_THREAD
#include "ruby/thread.h"
#include "log.h"
#include "common.h"

#include "Input.h"

#include "GraphicsSingleton.h"

GraphicsSingleton::GraphicsSingleton() : 
	m_gameWindow(m_eventDispatcher) {
}

std::unique_ptr<sf::Shader> GraphicsSingleton::createUniqueShader() const {
	return m_shaderFactory.createUnique();
}

sf::Shader* GraphicsSingleton::createNewShader() const {
	return m_shaderFactory.createNew();
}

bool GraphicsSingleton::areShadersEnabled() const {
	return m_shaderFactory.areEnabled();
}

void GraphicsSingleton::init() {
	m_shaderFactory.warnIfNotAvailable();

	/* Window Loading */
	auto config = m_configLoader.load();
	frame_count = 0;

	m_gameWindow.reload(std::move(config));

	LOG("[GraphicsSingleton] Init");

	/* Input adjustement */
	L_Input_Reset_Clocks();
	L_Input_Setusec_threshold(1000000 / config.frameRate);
}

void GraphicsSingleton::manageErrorMessage(VALUE self, const GraphicsUpdateMessage& message) {
	/* If the error is ClosedWindowError, we manage the window closing 
	 * When @on_close is defined to a proc, @on_close can decide if the window closing is allowed or not
	 * or do things before closing the window
	 * That's a kind of rescue process 
	 */
	if(message.errorObject == rb_eClosedWindow)
	{
		VALUE closeHandle = rb_iv_get(self, "@on_close");
		if(closeHandle != Qnil)
		{
			VALUE handleClass = rb_class_of(closeHandle);
			if(handleClass == rb_cProc)
				if(rb_proc_call(closeHandle, rb_ary_new()) == Qfalse) {
					InsideGraphicsUpdate = false;
					return; /* If the proc returns false we doesn't show the exception */
				}
		}
		stop();
	}
	/* We raise the message */
	InsideGraphicsUpdate = false;
	rb_raise(message.errorObject, "%s", message.message.c_str());
}

void GraphicsSingleton::updateProcessEvent(GraphicsUpdateMessage& message) {
	sf::Event event;
	L_EnteredText.clear();
	while(m_gameWindow.pollEvent(event))
	{
		switch(event.type)
		{
			case sf::Event::EventType::Closed:
				message.errorObject = rb_eClosedWindow;
				message.message = "Game Window has been closed by user.";
				return;
			case sf::Event::EventType::KeyPressed:
				L_Input_Update_Key(event.key.code, true);
				break;
			case sf::Event::EventType::KeyReleased:
				L_Input_Update_Key(event.key.code, false);
				break;
			case sf::Event::EventType::JoystickButtonPressed:
				L_Input_Update_Joy(event.joystickButton.joystickId, event.joystickButton.button, true);
				break;
			case sf::Event::EventType::JoystickButtonReleased:
				L_Input_Update_Joy(event.joystickButton.joystickId, event.joystickButton.button, false);
				break;
			case sf::Event::EventType::JoystickMoved:
				L_Input_Update_JoyPos(event.joystickMove.joystickId,
					event.joystickMove.axis,
					event.joystickMove.position);
				break;
			case sf::Event::EventType::JoystickConnected:
			case sf::Event::EventType::JoystickDisconnected:
				L_Input_Reset_JoyPos(event.joystickConnect.joystickId);
				break;
			case sf::Event::EventType::MouseMoved:
				L_Input_Mouse_Pos_Update(event.mouseMove.x, event.mouseMove.y);
				break;
			case sf::Event::EventType::MouseButtonPressed:
				L_Input_Mouse_Button_Update(event.mouseButton.button, true);
				L_Input_Mouse_Pos_Update(event.mouseButton.x, event.mouseButton.y);
				break;
			case sf::Event::EventType::MouseButtonReleased:
				L_Input_Mouse_Button_Update(event.mouseButton.button, false);
				L_Input_Mouse_Pos_Update(event.mouseButton.x, event.mouseButton.y);
				break;
			case sf::Event::EventType::MouseWheelScrolled:
				if(event.mouseWheelScroll.wheel == sf::Mouse::Wheel::VerticalWheel)
					L_Input_Mouse_Wheel_Update(static_cast<long>(event.mouseWheelScroll.delta));
				L_Input_Mouse_Pos_Update(event.mouseWheelScroll.x, event.mouseWheelScroll.y);
				break;
			case sf::Event::EventType::MouseLeft:
				L_Input_Mouse_Pos_Update(-256, -256);
				break;
			case sf::Event::EventType::TextEntered:
				L_EnteredText.append((char*)sf::String(event.text.unicode).toUtf8().c_str());
				break;
		}
	}
}

bool GraphicsSingleton::isGameWindowOpen() const {
	return m_gameWindow.isOpen();
}

void GraphicsSingleton::stop() {
	m_gameWindow.stop();
}

void GraphicsSingleton::windowDraw() {
	m_gameWindow.draw();
}

void* GraphicsDraw_Update_Internal(void* dataPtr) {
	//NO RUBY API ACCESS MUST BE DONE HERE
	auto& self = *reinterpret_cast<GraphicsSingleton*>(dataPtr);
	if(self.isGameWindowOpen()) {
		self.windowDraw();
		return nullptr;
	}

	auto message = std::make_unique<GraphicsUpdateMessage>();
	message->errorObject = rb_eStoppedGraphics;
	message->message = "Game Window was closed during Graphics.update by a unknow cause...";
	return message.release();
}

std::unique_ptr<GraphicsUpdateMessage> GraphicsSingleton::draw() {
	auto* result = rb_thread_call_without_gvl(GraphicsDraw_Update_Internal, static_cast<void*>(this), NULL, NULL);
	return std::unique_ptr<GraphicsUpdateMessage>(reinterpret_cast<GraphicsUpdateMessage*>(result));
}

void GraphicsSingleton::update(VALUE self, bool input) {
	// Prevent a Thread from calling Graphics.update during Graphics.update process
	if(InsideGraphicsUpdate) { 
		return;
	}
	InsideGraphicsUpdate = true;

	/* Graphics.update real process */
	auto message = draw();
	
	/* Message Processing */
	GraphicsUpdateMessage localMessage{};
	if(input) {
		updateProcessEvent(message == nullptr ? localMessage : *message);
	}
	localMessage = message == nullptr ? localMessage : *message;
	
	if(!localMessage.message.empty()) {
		manageErrorMessage(self, localMessage);
	}

	/* End of Graphics.update process */
	InsideGraphicsUpdate = false;

	if(input) {
		/* Update the frame count */
		frame_count++;
	}
}

void GraphicsSingleton::updateOnlyInput(VALUE self) {
	if (InsideGraphicsUpdate) {
		return;
	}
	InsideGraphicsUpdate = true;

	GraphicsUpdateMessage message;
	updateProcessEvent(message);
	if (!message.message.empty()) {
		manageErrorMessage(self, message);
	}

	InsideGraphicsUpdate = false;
}

void GraphicsSingleton::resizeScreen(int width, int height) {
	m_gameWindow.resizeScreen(width, height);
}

void GraphicsSingleton::setShader(sf::RenderStates* shader) {
	//TODO
	//m_gameWindow.setShader(shader);
}

VALUE GraphicsSingleton::takeSnapshot() {
	auto outputTexture = m_gameWindow.takeSnapshot();
	//TODO
	return Qnil;
}

void GraphicsSingleton::transition(VALUE self, int argc, VALUE* argv) {
	//TODO
	//m_gameWindow.transition(self, argc, argv);
}

void GraphicsSingleton::freeze(VALUE self) {
	//TODO
	//m_gameWindow.freeze(*game_window, self);
}

void GraphicsSingleton::warnIfGraphicsUpdate() const {
	if(InsideGraphicsUpdate) {
		rb_warn("WARNING : Accessing Ruby data while updating the screen");
	}
}
