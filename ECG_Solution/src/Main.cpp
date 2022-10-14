#include "Utils.h"
#include <sstream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwTerminate();
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS){
        glTranslatef(0.0f, 0.0f, 0.0f);
    }
        
}

static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg) {
    std::stringstream stringStream;
    std::string sourceString;
    std::string typeString;
    std::string severityString;

    switch (source) {
    case GL_DEBUG_SOURCE_API: {
        sourceString = "API";
        break;
    }
    case GL_DEBUG_SOURCE_APPLICATION: {
        sourceString = "Application";
        break;
    }
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
        sourceString = "Window System";
        break;
    }
    case GL_DEBUG_SOURCE_SHADER_COMPILER: {
        sourceString = "Shader Compiler";
        break;
    }
    case GL_DEBUG_SOURCE_THIRD_PARTY: {
        sourceString = "Third Party";
        break;
    }
    case GL_DEBUG_SOURCE_OTHER: {
        sourceString = "Other";
        break;
    }
    default: {
        sourceString = "Unknown";
        break;
    }
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR: {
        typeString = "Error";
        break;
    }
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
        typeString = "Deprecated Behavior";
        break;
    }
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
        typeString = "Undefined Behavior";
        break;
    }
    case GL_DEBUG_TYPE_PORTABILITY_ARB: {
        typeString = "Portability";
        break;
    }
    case GL_DEBUG_TYPE_PERFORMANCE: {
        typeString = "Performance";
        break;
    }
    case GL_DEBUG_TYPE_OTHER: {
        typeString = "Other";
        break;
    }
    default: {
        typeString = "Unknown";
        break;
    }
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH: {
        severityString = "High";
        break;
    }
    case GL_DEBUG_SEVERITY_MEDIUM: {
        severityString = "Medium";
        break;
    }
    case GL_DEBUG_SEVERITY_LOW: {
        severityString = "Low";
        break;
    }
    default: {
        severityString = "Unknown";
        break;
    }
    }

    stringStream << "OpenGL Error: " << msg;
    stringStream << " [Source = " << sourceString;
    stringStream << ", Type = " << typeString;
    stringStream << ", Severity = " << severityString;
    stringStream << ", ID = " << id << "]";

    return stringStream.str();
}

static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam)
{
    if(id == 131185 || id == 131218) return; // ignore performance warnings (buffer uses GPU memory, shader recompilation) from nvidia
	std::string error = FormatDebugOutput(source, type, id, severity, message);
	std::cout << error << std::endl;
}

/* --------------------------------------------- */
// Prototypes
/* --------------------------------------------- */



/* --------------------------------------------- */
// Global variables
/* --------------------------------------------- */



/* --------------------------------------------- */
// Main
/* --------------------------------------------- */

int main(int argc, char** argv)
{

	/* --------------------------------------------- */
	// Load settings.ini
	/* --------------------------------------------- */

	// init reader for ini files
	INIReader reader("assets/settings.ini");

	// load values from ini file
	// first param: section [window], second param: property name, third param: default value
	int width = reader.GetInteger("window", "width", 800);
	int height = reader.GetInteger("window", "height", 800);
	std::string window_title = reader.Get("window", "title", "ECG 2022");


	/* --------------------------------------------- */
	// Init framework
	/* --------------------------------------------- */
    glewExperimental = true;
	if(!glfwInit()){
        EXIT_WITH_ERROR("Failed to init GLFW");
	}

#if _DEBUG
	// Create a debug OpenGL context or tell your OpenGL library (GLFW, SDL) to do so.
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	
	GLFWwindow* window = glfwCreateWindow(width, height, window_title.c_str(), nullptr, nullptr);
	if (!window) {
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	
	if (GLEW_OK != glewInit())
	{
        EXIT_WITH_ERROR("Failed to init GLEW");
	}

#if _DEBUG
    // Register your callback function.
    glDebugMessageCallback(DebugCallback, NULL);
    // Enable synchronous callback. This ensures that your callback function is called
    // right after an error has occurred. 
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

	if (!initFramework()) {
		EXIT_WITH_ERROR("Failed to init framework");
	}

	
	/* --------------------------------------------- */
	// Initialize scene and render loop
	/* --------------------------------------------- */
    glClearColor(1, 1, 1, 1);
	glfwSetKeyCallback(window,key_callback);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
		drawTeapot();
		glfwSwapBuffers(window);
		if (argc > 1 && std::string(argv[1]) == "--run_headless")
		{
			saveScreenshot("screenshot", width, height);
			break;
		}
		
	}


	/* --------------------------------------------- */
	// Destroy framework
	/* --------------------------------------------- */

	
	

	destroyFramework();


	/* --------------------------------------------- */
	// Destroy context and exit
	/* --------------------------------------------- */


	return EXIT_SUCCESS;
}




