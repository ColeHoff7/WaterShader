#include "Main.h"

MyScene scene;

const GLint FRAMES_PER_SECOND = 60; // Desired maximum number of frames per second
const GLint FRAME_INTERVAL = 1000 / FRAMES_PER_SECOND; // Interval in milliseconds between frames
float zTrans = -25.0f; float rotationX = 0.0f; float rotationY = 0.0f;
bool view7 = false;
/**
* Acts as the display function for the window. 
*/
static void RenderSceneCB()
{
	// time in milliseconds of last frame render
	static GLint lastRenderTime = 0;

	int currentTime = glutGet(GLUT_ELAPSED_TIME); // Get current time
	int elapsedTime = currentTime - lastRenderTime; // Calc time since last frame
	
	// Check if enough time has elapsed since the last render.
	if (elapsedTime >= FRAME_INTERVAL) {
	
		// Save time for this frame render
		lastRenderTime = currentTime;
		checkOpenGLErrors("RenderSceneCB1");

		// Clear the color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		checkOpenGLErrors("RenderSceneCB2");
		// Draw the scene objects
		scene.draw();

		checkOpenGLErrors("RenderSceneCB3");
		// Flush all drawing commands and swapbuffers
		glutSwapBuffers();

		float deltaTime = (float)elapsedTime / 1000;

		if ( deltaTime > 2.0f/FRAMES_PER_SECOND ) {
			deltaTime = 1.0f / FRAMES_PER_SECOND;
		}

		scene.update(deltaTime);
		// Query OpenGL for errors.
		checkOpenGLErrors("RenderSceneCB4");
	}

	if (view7) {
		glm::mat4 transView = glm::translate(glm::vec3(0.0f, 0.0f, zTrans));
		glm::mat4 rotateViewX = glm::rotate(glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotateViewY = glm::rotate(glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		SharedProjectionAndViewing::setViewMatrix(transView * rotateViewX * rotateViewY);
	}

} // end RenderSceneCB


/**
* Handle changes in window size.
*/
static void ResizeCB(int width, int height)
{
	// Set viewport to entire client area of the window
	glViewport(0, 0, width, height);
	
	// Set the projection matrix based don the window aspect ratio
	scene.resize(width, height);

	// Signal the operating system to re-render the window
	glutPostRedisplay();

} // end ResizeCB


/**
* Calls a method of the the OpenGLApplicationBase class. Registered with GLUT by calling glutKeyboardFunc.
*/
static void KeyboardCB(unsigned char key, int x, int y)
{
	switch (key) {

	case('f') : case('F') : // 'f' key to toggle full screen
		glutFullScreenToggle();
		break;
	case(27) : // Escape key
		glutLeaveMainLoop();
		break;
	case('0') :
		scene.toggleLights(0);
		break;
	case('1') :
		scene.toggleLights(1);
		break;
	case('2') :
		scene.toggleLights(2);
		break;
	case('w') :
		zTrans++;
		break;
	case('s') :
		zTrans--;
		break;
	default:
		cout << key << " key pressed." << endl;
	}

	// Signal the operating system to re-render the window
	glutPostRedisplay();

} // end KeyboardCB


static void SpecialKeysCB(int key, int x, int y)
{
	switch (key) {

	case(GLUT_KEY_RIGHT) :
		rotationY--;
		break;
	case(GLUT_KEY_LEFT) :
		rotationY++;
		break;
	case(GLUT_KEY_UP) :
		rotationX--;
		break;
	case(GLUT_KEY_DOWN) :
		rotationX++;
		break;

	default:
		cout << key << " key pressed." << endl;
	}

	// Signal the operating system to re-render the window
	glutPostRedisplay();

} // end SpecialKeysCB


static void IdleCB()
{
	glutPostRedisplay(); // Mark the current window as needing to be redisplayed.

} // end IdleCB


static void mainMenuCB(int value)
{
	switch (value) {

	case(0) :

		// "Quit" selected on the menu
		glutLeaveMainLoop();
		break;
	case(1) :

		// Toggle full screen
		glutFullScreenToggle();
		break;

	default:
		cout << "Invalid view selection " << endl; 
	}

	// Signal GLUT to call display callback
	glutPostRedisplay();

} // end mainMenuCB


static void polygonModeMenuCB(int value)
{
	switch (value) {
	case(1) :
		// Render polygon fronts and fill them
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case(2) :
		// Render polygon fronts in wire frame
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case(3) :
		// Render polygon fronts in wire frame
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	default:
		cout << "Unknown Polygon Mode!" << endl;
	}
	// Signal GLUT to call display callback
	glutPostRedisplay();

} // end polygonModeMenuCB


static void pointSizeMenuCB(int value)
{
	glPointSize((GLfloat)value);
	cout << "Point Size is " << value << " pixels." << endl;
	glutPostRedisplay();

} // end pointSizeMenuCB


static void lineWidthMenuCB(int value)
{
	glLineWidth((GLfloat)value);
	cout << "Line width is " << value << " pixels." << endl;

} // end lineWidthMenuCB


static void antiAliasMenuCB(int value)
{
	switch (value) {
	case(1) :
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		break;
	case(2) :
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
		break;
	default:
		cout << "Unknown Main Menu Selection!" << endl;
	}

} // end antiAliasMenuCB


static void frontFaceMenuCB(int value)
{
	switch (value) {
	case(1) :
		glFrontFace(GL_CCW);
		break;
	case(2) :
		glFrontFace(GL_CW);
		break;

	default:
		cout << "Unknown winding order!" << endl;
	}
	// Signal GLUT to call display callback
	glutPostRedisplay();

} // end frontFaceMenuCB


static void cullFaceMenuCB(int value)
{
	switch (value) {
	case(1) :

		glCullFace(GL_FRONT);

		break;
	case(2) :

		glCullFace(GL_BACK);

		break;
	case(3) :
		glCullFace(GL_FRONT_AND_BACK);
		break;

	default:
		cout << "Unknown Face Culling Mode!" << endl;
	}
	// Signal GLUT to call display callback
	glutPostRedisplay();

} // end cullFaceMenuCB


static void faceCullingMenuCB(int value)
{
	switch (value) {
	case(1) :

		glEnable(GL_CULL_FACE);

		break;
	case(2) :

		glDisable(GL_CULL_FACE);

		break;

	default:
		cout << "Unknown Face Culling Mode!" << endl;
	}

} // end faceCullingMenuCB


static void depthTestingMenuCB(int value)
{
	switch (value) {
	case(1) :

		glEnable(GL_DEPTH_TEST);

		break;
	case(2) :

		glDisable(GL_DEPTH_TEST);

		break;

	default:
		cout << "Unknown DEPTH TEST Mode!" << endl;
	}

} // end depthTestingMenuCB


static void textureModeMenuCB(int value)
{
	scene.setTextureMode(value);

} // end textureModeMenuCB

static void viewTransformationMenu(int value) {
	view7 = false;
	switch (value) {
	case(1) :
		SharedProjectionAndViewing::setViewMatrix(glm::translate(glm::vec3(0.0f, 0.0f, -25.0f)));
		break;
	case(2) :
		SharedProjectionAndViewing::setViewMatrix(glm::translate(glm::vec3(0.0f, 0.0f, -25.0f))* glm::rotate(M_PI / 4.0f, glm::vec3(1.0, 0.0, 0.0)));
		break;
	case(3) :
		SharedProjectionAndViewing::setViewMatrix(glm::translate(glm::vec3(0.0f,0.0f, -55.0f)) * glm::rotate(M_PI / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(-M_PI/2.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
		break;
	case(4) :
		SharedProjectionAndViewing::setViewMatrix(glm::lookAt(glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		break;
	case(5) :
		SharedProjectionAndViewing::setViewMatrix(glm::lookAt(glm::vec3(0.0f, 17.6776f, 17.6776f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		break;
	case(6) :
		SharedProjectionAndViewing::setViewMatrix(glm::lookAt(glm::vec3(0.0f, 55.0f ,0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
		break;
	case(7) :
		view7 = true;
		// Set the viewing transformation based on the values held in the global // varaiables zTrans, rotationX, and rotationY. 
		glm::mat4 transView = glm::translate(glm::vec3(0.0f, 0.0f, zTrans)); 
		glm::mat4 rotateViewX = glm::rotate(glm::radians(rotationX),glm::vec3(1.0f, 0.0f, 0.0f)); 
		glm::mat4 rotateViewY = glm::rotate(glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f)); 
		SharedProjectionAndViewing::setViewMatrix(transView * rotateViewX * rotateViewY);
		break;
	}
}

static void projectionMenu(int value) {
	switch (value) {
	case(0) :
		scene.setProj(0);
		break;
	case(1) :
		scene.setProj(1);
		break;
	case(2) :
		scene.setProj(2);
		break;
	}
}


static void createMenus()
{
	// Create polygon submenu
	GLuint menu1id = glutCreateMenu(polygonModeMenuCB);
	// Specify menu items and integer identifiers
	glutAddMenuEntry("Filled", 1);
	glutAddMenuEntry("Outline", 2);
	glutAddMenuEntry("Point", 3);

	// Create point size submenu
	GLuint menu2id = glutCreateMenu(pointSizeMenuCB);
	// Specify menu items and  integer identifiers
	glutAddMenuEntry("1", 1);
	glutAddMenuEntry("10", 10);
	glutAddMenuEntry("50", 50);
	glutAddMenuEntry("100", 100);
	glutAddMenuEntry("500", 500);

	GLuint menu3id = glutCreateMenu(lineWidthMenuCB);
	glutAddMenuEntry("1", 1);
	glutAddMenuEntry("10", 10);
	glutAddMenuEntry("20", 20);
	glutAddMenuEntry("50", 50);

	GLuint menu4id = glutCreateMenu(frontFaceMenuCB);
	glutAddMenuEntry("CCW", 1);
	glutAddMenuEntry("CW", 2);

	GLuint menu5id = glutCreateMenu(cullFaceMenuCB);
	glutAddMenuEntry("Front", 1);
	glutAddMenuEntry("Back", 2);
	glutAddMenuEntry("Front and Back", 3);

	GLuint menu6id = glutCreateMenu(faceCullingMenuCB);
	glutAddMenuEntry("On", 1);
	glutAddMenuEntry("Off", 2);

	GLuint menu7id = glutCreateMenu(depthTestingMenuCB);
	glutAddMenuEntry("On", 1);
	glutAddMenuEntry("Off", 2);

	GLuint menu8id = glutCreateMenu(antiAliasMenuCB);
	glutAddMenuEntry("On", 1);
	glutAddMenuEntry("Off", 2);

	GLuint menu9id = glutCreateMenu(textureModeMenuCB);
	glutAddMenuEntry("None", 0);
	glutAddMenuEntry("Decal", 1);
	glutAddMenuEntry("Replace Ambient and Diffuse", 2);

	GLuint menu10id = glutCreateMenu(viewTransformationMenu);
	glutAddMenuEntry("View 1", 1);
	glutAddMenuEntry("View 2", 2);
	glutAddMenuEntry("View 3", 3);
	glutAddMenuEntry("View 4", 4);
	glutAddMenuEntry("View 5", 5);
	glutAddMenuEntry("View 6", 6);
	glutAddMenuEntry("View 7", 7);

	GLuint menu11id = glutCreateMenu(projectionMenu);
	glutAddMenuEntry("Orthographic", 0);
	glutAddMenuEntry("Perspective", 1);
	glutAddMenuEntry("Split", 2);
	
	
	// Create main menu
	GLuint topMenu = glutCreateMenu(mainMenuCB);
	// Attach polygon Menu
	glutAddSubMenu("Polygon Mode", menu1id);
	// Attach point size Menu
	glutAddSubMenu("Point Size", menu2id);
	glutAddSubMenu("Line Width", menu3id);
	glutAddSubMenu("Front Face", menu4id);
	glutAddSubMenu("Face to be Culled", menu5id);
	glutAddSubMenu("Face Culling", menu6id);
	glutAddSubMenu("Depth Testing", menu7id);
	glutAddSubMenu("Point and Line Antialising", menu8id);
	glutAddSubMenu("Texture Mode", menu9id);
	glutAddSubMenu("View Transformation", menu10id);
	glutAddSubMenu("Projection", menu11id);

	// Specify menu items and integer identifiers
	glutAddMenuEntry("Toggle Full Screen", 1);
	glutAddMenuEntry("Quit", 0);

	// Attach menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


int main(int argc, char** argv)
{
	view7 = true;
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE |GLUT_RGBA | GLUT_DEPTH);


	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Set the initial window size
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Specify window placement relative to the upper left hand corner of the screen.
	glutInitWindowPosition(100, 100);

	// Create a window 
	GLuint world_Window = glutCreateWindow("Water");


	glewExperimental = GL_TRUE;
	// Intilize GLEW. This must be done after glut is initialized.
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return false; // GLEW could not be initialized.
	}


	displayOpenGlInfo();


	scene.initialize();

	// Register callback functions to handle various window events.
	glutDisplayFunc(RenderSceneCB);
	glutReshapeFunc(ResizeCB);
	glutKeyboardFunc(KeyboardCB);
	glutSpecialFunc(SpecialKeysCB);
	glutIdleFunc(IdleCB);

	// Create popup menus to control rendering pipeline
	createMenus();

	checkOpenGLErrors("main");


	glutMainLoop();


	return 0;

} // end main


