#include "AppOpenGL.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

std::map<GLFWwindow*, OpenGLWindow*> OpenGLWindow::_windows;

OpenGLWindow::OpenGLWindow()
{
    for (bool &kwp : _keyWasPressed)
    {
        kwp = false;
    }
}

bool OpenGLWindow::createOpenGLWindow(const std::string &winTitle, int majorVersion, int minorVersion, bool fullscreen)
{
    if (!glfwInit())
    {
        exit(1);
        std::cout << "GLFW Error" << std::endl;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    const auto primaryMonitor = glfwGetPrimaryMonitor();
    const auto videoMode = glfwGetVideoMode(primaryMonitor);

    _window = glfwCreateWindow(videoMode->width, videoMode->height, winTitle.c_str(), fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

    if (_window == nullptr)
    {
        return false;
    }

    glfwMakeContextCurrent(_window);

    if (glewInit() != GLEW_OK)
    {
        exit(1);
        std::cout << "GLEW Error" << std::endl;
    }

    glfwSwapInterval(1);

    glfwSetWindowSizeCallback(_window, onWindowSizeChangedStatic);

	if (!fullscreen)
	{
		glfwMaximizeWindow(_window);
		// After calling glfwMaximizeWindow, the onWindowSizeChanged somehow does not get called
		// Therefore I call it manually
		int width, height;
		glfwGetWindowSize(_window, &width, &height);
		onWindowSizeChanged(_window, width, height);
	}
	_windows[_window] = this;

	return true;
}

void OpenGLWindow::runApp()
{
	setVerticalSynchronization(true);
	recalculateProjectionMatrix();
	initializeScene();

	// Update time at the beginning, so that calculations are correct
	_lastFrameTime = _lastFrameTimeFPS = glfwGetTime();

	while (glfwWindowShouldClose(_window) == 0)
	{
		updateDeltaTimeAndFPS();
		renderScene();

		glfwSwapBuffers(_window);
		glfwPollEvents();
		handleInput();
	}

	releaseScene();

	glfwDestroyWindow(_window);
	_windows.erase(_windows.find(_window));
	
	if (_windows.empty())
	{
		glfwTerminate();
	}
}

bool OpenGLWindow::keyPressed(int keyCode) const
{
	return  glfwGetKey(_window, keyCode) == GLFW_PRESS;
}

bool OpenGLWindow::keyPressedOnce(int keyCode)
{
	bool result = false;
	if (keyPressed(keyCode))
	{
		if (!_keyWasPressed[keyCode])
		{
			result = true;
		}
		
		_keyWasPressed[keyCode] = true;
	}
	else
	{
		_keyWasPressed[keyCode] = false;
	}

	return result;
}

void OpenGLWindow::closeWindow(bool hasErrorOccured)
{
	glfwSetWindowShouldClose(_window, true);
	_hasErrorOccured = hasErrorOccured;
}

void OpenGLWindow::setVerticalSynchronization(bool enable)
{
	glfwSwapInterval(enable ? 1 : 0);
	_isVerticalSynchronizationEnabled = enable;
}

OpenGLWindow* OpenGLWindow::getDefaultWindow()
{
    return _windows.size() == 0 ? nullptr : (*_windows.begin()).second;
}

void OpenGLWindow::recalculateProjectionMatrix()
{
	int width, height;
	glfwGetWindowSize(getWindow(), &width, &height);
    _projMat = glm::perspective(45.0f, float(width) / float(height), 0.5f, 1500.0f);
	_orthoMat = glm::ortho(0.0f, float(width), 0.0f, float(height));
}

void OpenGLWindow::updateDeltaTimeAndFPS()
{
	const auto currentTime = glfwGetTime();
	_timeDelta = currentTime - _lastFrameTime;
	_lastFrameTime = currentTime;
	_nextFPS++;

	if(currentTime - _lastFrameTimeFPS > 1.0)
	{
		_lastFrameTimeFPS = currentTime;
		_FPS = _nextFPS;
		_nextFPS = 0;
	}
}

void OpenGLWindow::onWindowSizeChangedStatic(GLFWwindow* window, int width, int height)
{
	if (_windows.count(window) != 0)
	{
		_windows[window]->recalculateProjectionMatrix();
		_windows[window]->onWindowSizeChanged(window, width, height);
	}
}