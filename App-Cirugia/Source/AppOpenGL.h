#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <map>

class OpenGLWindow
{
private:
    bool cameraOn = true;

    GLFWwindow *_window = nullptr;
    bool _keyWasPressed[512];
    bool _hasErrorOccured = false;
    glm::mat4 _projMat;
    glm::mat4 _orthoMat;
    void recalculateProjectionMatrix();
    double _lastFrameTime = 0.0;
	double _lastFrameTimeFPS = 0.0; 
    double _timeDelta = 0.0;
    int _FPS = 0;
    int _nextFPS = 0;
    bool _isVerticalSynchronizationEnabled = false;
	void updateDeltaTimeAndFPS();
	static void onWindowSizeChangedStatic(GLFWwindow* window, int width, int height);
    static std::map<GLFWwindow*, OpenGLWindow*> _windows;

public:
    /* generic implementation*/
    OpenGLWindow();
    bool createOpenGLWindow(const std::string &winTitle, int majorVersion, int minorVersion, bool fullscreen);
    GLFWwindow *getWindow() const { return _window; } 
    void runApp();
    bool keyPressed(int keyCode) const;
	bool keyPressedOnce(int keyCode);
	void closeWindow(bool hasErrorOccured = false);
	bool hasErrorOccured() const { return _hasErrorOccured; }
    glm::mat4 getProjectionMatrix() const { return _projMat; }
    glm::mat4 getOrthoProjectionMatrix() const { return _orthoMat; }
    float sof(float value) const { return value * float(_timeDelta); }
	double sof(double value) const { return value * _timeDelta; }
    double getTimeDelta() const { return _timeDelta; }
    int getFPS() const { return _FPS; }
	void setVerticalSynchronization(bool enable);
	bool isVerticalSynchronizationEnabled() const { return _isVerticalSynchronizationEnabled; }
    static OpenGLWindow* getDefaultWindow();
    
    /* specific implementation (abstract) */
    void initializeScene(); 
    void renderScene(); 
    void releaseScene(); 
    void handleInput(); 
    void onWindowSizeChanged(GLFWwindow* window, int width, int height);
};