// OldSchoolFPS.cpp : Defines the entry point for the console application.
//

#include "Common.h"
#include "Scene.h"
#include "SnowmanBehaviour.h"
#include "CubeBehaviour.h"
#include "BillboardBehaviour.h"
#include "TriggerBehaviour.h"
#include "TimedMessage.h"
#include "Camera.h"


//init variables
std::vector<GameObject*> scene;
bool g_key[256];
Camera g_camera;
int WIDTH = 800;
int HEIGHT = 600;

const float g_gravity = 0.5;

const float g_translation_speed = 0.5;
const float g_rotation_speed = M_PI / 180 * 0.2;
const float g_jumpingForce = 100;

btDiscreteDynamicsWorld* dynamicsworld;

// the key states. These variables will be zero
// when no key is being pressesed
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;

// scale of snowman
float scale = 1.0f;

// default font
void *font = GLUT_STROKE_ROMAN;

// width and height of the window
int h, w;

// variables to compute frames per second
int frame = 1;
long time, timebase;
char s[60];
char currentMode[80];

// this string keeps the last good setting
// for the game mode
char gameModeString[40] = "640x480";

void init();

void changeSize(int ww, int hh) {

	h = hh;
	w = ww;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void renderBitmapString(
	float x,
	float y,
	float z,
	void *font,
	char *string) {

	char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void renderStrokeFontString(
	float x,
	float y,
	float z,
	void *font,
	char *string) {

	char *c;
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(0.002f, 0.002f, 0.002f);
	for (c = string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	}
	glPopMatrix();
}

void restorePerspectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	// restore previous projection matrix
	glPopMatrix();

	// get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection
	gluOrtho2D(0, w, h, 0);

	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {


	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	
	//update physics
	dynamicsworld->stepSimulation(btScalar(1.) / btScalar(60.), btScalar(1.) / btScalar(60.));
	// Draw ground
	/*glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();*/

	// Set the camera
	g_camera.UpdatePhysics();
	g_camera.Refresh();
	
	for (auto go : scene)
	{
		if (go->Rigidbody != NULL)
		{
			btTransform trans;
			go->Rigidbody->getMotionState()->getWorldTransform(trans);
			go->GetTransform().pos = glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
			go->GetTransform().rot = glm::vec3(trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ());
		}
	}

	// Draw all models
	for (auto go : scene)
	{
		glPushMatrix();
		glScalef(go->GetTransform().scale.x, go->GetTransform().scale.y, go->GetTransform().scale.z);
		glTranslatef(go->GetTransform().pos.x, go->GetTransform().pos.y, go->GetTransform().pos.z);
		glRotatef(go->GetTransform().rot.x, 1.0, 0.0, 0.0);
		glRotatef(go->GetTransform().rot.y, 1.0, 0.0, 0.0);
		glRotatef(go->GetTransform().rot.z, 1.0, 0.0, 0.0);
		
		if (go->GetBehaviour() != nullptr)
		{
			go->GetBehaviour()->Draw();
		}
		
		//sprintf(number, "%d", (i + 3) * 6 + (j + 3));
		glPopMatrix();
	}

	//char number[3];
	//for (int i = -3; i < 3; i++)
	//	for (int j = -3; j < 3; j++) {
	//		glPushMatrix();
	//		glTranslatef(i*10.0f, 0.0f, j * 10.0f);
	//		drawSnowMan();
	//		sprintf(number, "%d", (i + 3) * 6 + (j + 3));
	//		renderStrokeFontString(0.0f, 0.5f, 0.0f, (void *)font, number);
	//		glPopMatrix();
	//	}

	// Code to compute frames per second
	frame++;

	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(s, "FPS:%4.2f",
			frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}

	setOrthographicProjection();
	void *font = GLUT_BITMAP_8_BY_13;
	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(30, 15, 0, font, (char *)"Old School FPS Engine");
	renderBitmapString(30, 30, 0, font, s);
	char s2[60];
	sprintf(s2, ("Colliders: " + std::to_string(dynamicsworld->getNumCollisionObjects())).c_str());
	
	renderBitmapString(30, 75, 0, font, (char*)std::to_string(time/1000).c_str());
	
	renderBitmapString(30, 45, 0, font, s2);

	//crosshair...lol
	renderBitmapString(WIDTH/2, HEIGHT/2, 0, font, (char*)".");

	
	//update all objects
	for (auto go : scene)
	{
		if (go->GetBehaviour() != NULL)
		{
			go->GetBehaviour()->Update(1 / (frame*1000.0 / (time - timebase)), &g_camera);
		}
	}


	glPopMatrix();

	restorePerspectiveProjection();

	glutSwapBuffers();
}

// -----------------------------------
//             KEYBOARD
// -----------------------------------

void processNormalKeys(unsigned char key, int xx, int yy) {


	if (key == 27)
	{
		glutLeaveGameMode();
		exit(0);
	}

	g_key[key] = true;
}

bool canJump = true;

void Timer(int value)
{
	glm::vec3 amount;
	float actualSpeed = g_translation_speed; // *(1 / (frame*1000.0 / (time - timebase))); //Time.deltaTime...

	if (g_key['w'] || g_key['W']) {
		amount.z = actualSpeed;
	}
	if (g_key['s'] || g_key['S']) {
		amount.z = -actualSpeed;
	}
	if (g_key['a'] || g_key['A']) {
		amount.x = actualSpeed;
	}
	if (g_key['d'] || g_key['D']) {
		amount.x = -actualSpeed;
	}
	if (g_key[' '] && canJump)
	{
		g_camera.Jump(g_jumpingForce);
		canJump = false;
	}

	g_camera.Move(amount);

	//else if (g_mouse_left_down) {
	//	g_camera.Fly(-g_translation_speed);
	//}
	//else if (g_mouse_right_down) {
	//	g_camera.Fly(g_translation_speed);
	//}

	glutTimerFunc(1, Timer, 0);
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
	case GLUT_KEY_UP: deltaMove = 0.5f; break;
	case GLUT_KEY_DOWN: deltaMove = -0.5f; break;
	case GLUT_KEY_F1:
		// define resolution, color depth
		glutGameModeString("640x480:32");
		// enter full screen
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
			glutEnterGameMode();
			sprintf(gameModeString, "640x480:32");
			// register callbacks again
			// and init OpenGL context
			init();
		}
		else
			glutGameModeString(gameModeString);
		break;
	case GLUT_KEY_F2:
		// define resolution, color depth
		glutGameModeString("800x600:32");
		// enter full screen
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
			glutEnterGameMode();
			sprintf(gameModeString, "800x600:32");
			// register callbacks again
			// and init OpenGL context
			init();
		}
		else
			glutGameModeString(gameModeString);
		break;
	case GLUT_KEY_F3:
		// define resolution, color depth
		glutGameModeString("1024x768:32");
		// enter full screen
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
			glutEnterGameMode();
			sprintf(gameModeString, "1024x768:32");
			// register callbacks again
			// and init OpenGL context
			init();
		}
		else
			glutGameModeString(gameModeString);
		break;
	case GLUT_KEY_F4:
		// define resolution, color depth
		glutGameModeString("1280x1024:32");
		// enter full screen
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
			glutEnterGameMode();
			sprintf(gameModeString, "1280x1024:32");
			// register callbacks again
			// and init OpenGL context
			init();
		}
		else
			glutGameModeString(gameModeString);
		break;
	case GLUT_KEY_F5:
		// define resolution, color depth
		glutGameModeString("1920x1200");
		// enter full screen
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
			glutEnterGameMode();
			sprintf(gameModeString, "1920x1200");
			// register callbacks again
			// and init OpenGL context
			init();
		}
		else
			glutGameModeString(gameModeString);
		break;
	case GLUT_KEY_F6:
		// return to default window
		w = 800; h = 600;
		if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) != 0) {
			glutLeaveGameMode();
			//init();
		}
		break;
	}
	if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) == 0)
		sprintf(currentMode, "Current Mode: Window");
	else
		sprintf(currentMode,
			"Current Mode: Game Mode %dx%d at %d hertz, %d bpp",
			glutGameModeGet(GLUT_GAME_MODE_WIDTH),
			glutGameModeGet(GLUT_GAME_MODE_HEIGHT),
			glutGameModeGet(GLUT_GAME_MODE_REFRESH_RATE),
			glutGameModeGet(GLUT_GAME_MODE_PIXEL_DEPTH));
}

void releaseKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN: deltaMove = 0; break;
	}
}

void releaseNormalKey(unsigned char key, int x, int y)
{
	glm::vec3 amount;

	if (g_key['w'] || g_key['W']) {
		amount.z = 0;
	}
	if (g_key['s'] || g_key['S']) {
		amount.z = 0;
	}
	if (g_key['a'] || g_key['A']) {
		amount.x = 0;
	}
	if (g_key['d'] || g_key['D']) {
		amount.x = 0;
	}
	if (g_key[' '])
	{
		canJump = true;
	}

	g_camera.Move(amount);

	g_key[key] = false;
}

// -----------------------------------
//             MENU
// -----------------------------------

void processMenuEvents(int option) {

	/*switch (option) {
	case 1:
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f; break;
	case 2:
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f; break;
	case 3:
		red = 0.0f;
		green = 0.0f;
		blue = 1.0f; break;
	case 4:
		red = 1.0f;
		green = 0.5f;
		blue = 0.5f; break;
	}*/
}

void createGLUTMenus() {

	int menu;

	// create the menu and
	// tell glut that "processMenuEvents" will
	// handle the events
	menu = glutCreateMenu(processMenuEvents);

	//add entries to our menu
	
	//glutAddMenuEntry("No Hit", 1);
	//glutAddMenuEntry("Blue", 2);
	//glutAddMenuEntry("Green", 3);
	//glutAddMenuEntry("Orange", 4);

	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// -----------------------------------
//             MOUSE
// -----------------------------------

void mouseMove(int x, int y) {

	static bool just_warped = false;

	if (just_warped) {
		just_warped = false;
		return;
	}

	int dx = x - w / 2;
	int dy = y - h / 2;


	if (dx) {
		g_camera.RotateY(-g_rotation_speed*dx);
	}

	if (dy) {
		g_camera.Pitch(g_rotation_speed*dy);
	}

	glutWarpPointer(w / 2, h / 2);

	//Depending on offset, apply movement to camera.
	/*if (dx > 0.0f)
	{
		g_camera.RotateY(-g_rotation_speed);
		glutWarpPointer(w / 2, h / 2);
	}

	if (dx < 0.0f)
	{
		g_camera.RotateY(g_rotation_speed);
		glutWarpPointer(w / 2, h / 2);
	}

	if (dy > 0.0f)
	{
		g_camera.Pitch(-g_rotation_speed);
		glutWarpPointer(w / 2, h / 2);
	}

	if (dy < 0.0f)
	{
		g_camera.Pitch(g_rotation_speed);
		glutWarpPointer(w / 2, h / 2);
	}*/

	just_warped = true;
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			//angle += deltaAngle;
			//xOrigin = -1;
		}
		else {// state = GLUT_DOWN
			//xOrigin = x;
			btVector3 btFrom(g_camera.GetPos().x, g_camera.GetPos().y, g_camera.GetPos().z);
			btVector3 btTo(g_camera.Forward().x, g_camera.Forward().y, g_camera.Forward().z);

			btCollisionWorld::AllHitsRayResultCallback res(btFrom, btTo);
			dynamicsworld->rayTest(btFrom, btTo * 500, res); // m_btWorld is btDiscreteDynamicsWorld

			if (res.hasHit()) {
				for (int i = 0; i < res.m_collisionObjects.size(); i++)
				{
					btCollisionObject* body = (btCollisionObject*)res.m_collisionObjects[i]->getCollisionShape();

					for (auto obj : scene)
					{
						if (obj->Rigidbody != NULL)
							if (obj->Rigidbody->getUserPointer() == body->getUserPointer())
							{
								obj->GetBehaviour()->InteractedByPlayer(&g_camera);
							}
					}
				}
				//str = "Collision at: " + std::to_string(res.m_hitPointWorld.getX()) + ", " + std::to_string(res.m_hitPointWorld.getY()) + ", " + std::to_string(res.m_hitPointWorld.getZ());
			}
			//std::string str = "";
			//char* txt;

			//txt = (char*)str.c_str();
		}
	}
}

void init() {

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutKeyboardUpFunc(releaseNormalKey);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMove);

	glutTimerFunc(1, Timer, 0);

	//createGLUTMenus();
	glutSetCursor(GLUT_CURSOR_NONE);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	for (auto go : scene)
	{
		go->GetBehaviour()->Start();
		if (go->Rigidbody != NULL)
			dynamicsworld->addRigidBody(go->Rigidbody);
	}

	g_camera.InitCamera();

	dynamicsworld->addRigidBody(g_camera.Rigidbody);
}

// -----------------------------------
//             UTILS
// -----------------------------------

std::vector<std::string> split(const char *str, char c = ' ')
{
	std::vector<std::string> result;
	do
	{
		const char *begin = str;

		while (*str != c && *str)
			str++;

		result.push_back(std::string(begin, str));
	} while (0 != *str++);

	return result;
}

// -----------------------------------
//             MAIN
// -----------------------------------

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Old School FPS Engine v0.1");

	//load level file
	std::string level = "";
	std::string line;
	std::ifstream myfile("level.txt");
	if (myfile.is_open())
	{
		while (std::getline(myfile, line))
		{
			level += line;
		}
		myfile.close();
	}
	//split level into tiles
	std::vector<std::string> levelTiles = split(level.c_str(), ';');
	std::vector<std::string> loadedTextures = std::vector<std::string>();
	std::vector<GLuint> textureIndeces = std::vector<GLuint>();

	//Configure game
	scene = std::vector<GameObject*>();

	//configure physics
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	
	dynamicsworld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsworld->setGravity(btVector3(0, -g_gravity, 0));

	for (auto tile : levelTiles)
	{
		//process individual tile data
		std::vector<std::string> tileData = split(tile.c_str(), ',');

		//that's our tile, we switch immediately between types
		///f = floor
		///b = billboard
		///w = wall
		GameObject* newObj = new GameObject(glm::vec3(std::stof(tileData[1]) * 4, std::stof(tileData[2]) * 4, std::stof(tileData[3]) * 4));
		
		//first check if we loaded the corresponding texture
		bool textureHasBeenLoaded = false;
		int loadedTextureID = 0;
		if (loadedTextures.size() != 0)
		{
			for (auto tex : loadedTextures)
			{
				if (tex == tileData[0])
				{
					textureHasBeenLoaded = true;
					break;
				}
				loadedTextureID++;
			}
		}

		if (tileData[0][0] == 'p')
		{
			g_camera.SetPos(glm::vec3(std::stof(tileData[1]) * 4, std::stof(tileData[2]) * 4, std::stof(tileData[3]) * 4));
		}

		else if (tileData[0][0] == 'f') //FLOOR
		{
			//if we haven't loaded the textures yet, we do it now
			if (!textureHasBeenLoaded)
			{
				GLuint tex_2d = SOIL_load_OGL_texture
				(
				("res/"+ tileData [0]+".png").c_str(),
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
				);

				loadedTextures.push_back(tileData[0]);
				textureIndeces.push_back(tex_2d);
				newObj->textureID = textureIndeces.size();
			}
			else //we assign the texture that was loaded using its name
			{
				newObj->textureID = textureIndeces.at(loadedTextureID);
			}
			newObj->SetBehaviour(new CubeBehaviour());
			scene.push_back(newObj);
		}

		else if (tileData[0][0] == 'b') //BILLBOARD
		{
			//if we haven't loaded the textures yet, we do it now
			if (!textureHasBeenLoaded)
			{
				GLuint tex_2d = SOIL_load_OGL_texture(("res/" + tileData[0] + ".png").c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MULTIPLY_ALPHA);

				loadedTextures.push_back(tileData[0]);
				textureIndeces.push_back(tex_2d);
				newObj->textureID = textureIndeces.size();
			}
			else //we assign the texture that was loaded using its name
			{
				newObj->textureID = textureIndeces.at(loadedTextureID);
			}
			newObj->SetBehaviour(new BillboardBehaviour());
			scene.push_back(newObj);
		}

		else if (tileData[0][0] == 't') //TRIGGER SPHERE
		{
			newObj->SetBehaviour(new TriggerBehaviour());
			newObj->GetBehaviour()->m_messageToShow = tileData[4];
			scene.push_back(newObj);
		}

		//load gun
		if (!textureHasBeenLoaded)
		{
			GLuint tex_2d = SOIL_load_OGL_texture("res/w10.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MULTIPLY_ALPHA);

			loadedTextures.push_back(tileData[0]);
			textureIndeces.push_back(tex_2d);
			g_camera.m_gun.texture = textureIndeces.size();
		}
		else //we assign the texture that was loaded using its name
		{
			g_camera.m_gun.texture = textureIndeces.at(loadedTextureID);
		}
	}

	// register callbacks
	init();

	// enter GLUT event processing cycle
	glutMainLoop();

	//clean up
	for (auto go : scene)
	{
		if (go->GetBehaviour() != nullptr)
		{
			if (go->Rigidbody != NULL)
			{
				dynamicsworld->removeRigidBody(go->Rigidbody);
				delete go->Rigidbody->getMotionState();
				delete go->Rigidbody;
			}
			delete go->GetBehaviour();
		}
		delete go;
	}

	// Clean up behind ourselves like good little programmers
	delete dynamicsworld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;

	return 1;
}