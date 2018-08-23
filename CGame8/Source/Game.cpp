/*
 *Now that we've seen how much more effective the fast single call update is, we've refactored the code to make that the way to move forward.
 *Also to clean up what was now a complex and untidy Game class, we've moved all the set up and draw functions for tiles (and Bob) to their own class.
 *
 *It's important to try to keep code in the class that it's relevent too, having Tile setup, calculation and render code in the Game class, let us get it working
 *but it makes the code a lot harder to read and keep track of.
 *
 *Now we've stripped things back again, we can simply focus on getting the game to inititalise and go through an update loop.
 *
 **/


#include "Game.h"
#include "SimpleObj.h"
#include "Bob.h"
#include "PointToPoint.h"
#include "WaitAndChase.h"
/*****************************************************************/


// These are called static variables they exist in the SimpleObj class, and mean that all Instances of a SimpleObj will share the values that are put in here
// the are defaulted to 0 or -1 but should be set to their default values as soon as an instance of a SimpleObj is uses.
// take care that if you change one of these values, for 1 object, you change it for all objects of the same type/base

GLint SimpleObj::positionLoc =0;
GLint SimpleObj::texCoordLoc = 0;

// if we want to position it somewhere we need to ask for the handle for the position uniform and other unfirms
GLint SimpleObj::positionUniformLoc = 0;
GLint SimpleObj::samplerLoc = 0;
GLint SimpleObj::ScreenCoordLoc = 0;
GLint SimpleObj::ScaleLoc = 0;
GLuint SimpleObj::VBO = -1;

// we are going to reflect the size of our textures, so we need a square that has size 1x1
// since the distance from the centre to the corners is 0.5x and 0.5y, two tringles will use offsets of 0.5f
static const GLfloat QuadVerticesWithTex[] =
{
	// square vertices with texture coordinates added, so we have 3 floats of vertex,
   // then 2 floats of texture coords (called uv coords), this is called interlacing data
	0.5f,0.5f,0,
	1.0f,0.0f,	// texture coordinate
	-0.5f,0.5f,0,
	0.0f,0.0f,	// texture coordinate
	0.5f,-0.5f,  0,
	1.0f,1.0f, 	// texture coordinate

	//2nd tri
	-0.5f,0.5f,0,
	0.0f,0.0f,	// texture coordinate
	-0.5f,-0.5f,0,
	0.0f,1.0f, 	// texture coordinate
	0.5f,-0.5f,0,
	1.0f,1.0f // texture coordinate
};


/**********************************************************************************************
 We should consider moving these values above out of Game class too ^^^^ where's a good place?
***********************************************************************************************/

//now into the meat of our Game class.


std::vector<SimpleObj*> Game::MyObjects; // because we defined this as static variables in our header we have to make sure it exists

Game::Game()
{
	OGLES.Init(); // make sure we initialise the OGL instance we called OGLES

// make our Bob	here, this ensures there is at least one instance of a simpleObj which makes our init of static variables safer (they can be destoyed by a later instance generation)

	Bob* MyBob = new Bob();
	MyBob->TheOGLInstance = &OGLES;  // Bob's render system needs to know where the OGELS class is, since it has info on the basic shader
	MyObjects.push_back(MyBob);   // put our Bob characer into a processing list of characters

// put our vertices into our VBO
	glGenBuffers(1, &SimpleObj::VBO);  // first we generate the buffer and store the handle in VBO
	glBindBuffer(GL_ARRAY_BUFFER, SimpleObj::VBO);   //now we mind that, which we can leave as bound since we use the same VBO
	glBufferData(GL_ARRAY_BUFFER, // now we send the vertex data to the GPU one time
		6*5 * sizeof(float),
		QuadVerticesWithTex,
		GL_STATIC_DRAW);


// All our sprites use the same shader and quad box to draw, so lets set them up, so they can get the values needed when they are asked to draw
	glUseProgram(OGLES.programObject);
	//	 Get the attribute locations, we'll store these in values we can call handles.
	SimpleObj::positionLoc = glGetAttribLocation(OGLES.programObject, "a_position");
	SimpleObj::texCoordLoc = glGetAttribLocation(OGLES.programObject, "a_texCoord");

// set up the uniform locations too
	SimpleObj::positionUniformLoc = glGetUniformLocation(OGLES.programObject, "u_position");
	SimpleObj::samplerLoc = glGetUniformLocation(OGLES.programObject, "s_texture");
	SimpleObj::ScreenCoordLoc = glGetUniformLocation(OGLES.programObject, "u_Screensize");
	SimpleObj::ScaleLoc = glGetUniformLocation(OGLES.programObject, "u_Scale");

// So the Game ckass constructor, basically sets up the OGL, and provides the sprites with the values its shader needs.

};

Game::~Game() {}; // We should close down some things here but can do it later.


/**********************************************************************************

 Here's he main code..since the game starts here, we can do a bit of set up and init
 Then go into a loop to control the game

 ***********************************************************************************/

void Game::Update()
{
// we create bob in our constructor, and in the process set up some important SimpleObj static variables,
// now lets put a couple of differnt baddies on screen, first up,  these are point to point types
	PointToPoint* P2P1 = new PointToPoint();
	P2P1->TheOGLInstance = &OGLES;
	P2P1->Xpos = 280;
	P2P1->Ypos = 280;
	P2P1->SetXPoints(280, 50, Right); // << what do you think will happen if we make it move further? Try a count of 150 pixels, and try to predict, what goes "wrong", can you think of a way to fix things?
	MyObjects.push_back(P2P1);

	PointToPoint* P2P2 = new PointToPoint();
	P2P2->TheOGLInstance = &OGLES;
	P2P2->Xpos = 280;
	P2P2->Ypos = 80;
	P2P2->SetXPoints(280, 350, Right);
	MyObjects.push_back(P2P2);

// and these are wait and chase
	WaitAndChase* WaC1 = new WaitAndChase();
	WaC1->TheOGLInstance = &OGLES;
	WaC1->Xpos = 380;
	WaC1->Ypos = 80;
	MyObjects.push_back(WaC1);

	WaitAndChase* WaC2 = new WaitAndChase();
	WaC2->TheOGLInstance = &OGLES;
	WaC2->Xpos = 480;
	WaC2->Ypos = 280;
	MyObjects.push_back(WaC2);


// all that messy init code for FastUpdate is now contained in the Tiles Class, so lets make an instance of Tiles
// and initialise them ready to be rendered
	Tiles MyTiles;
	MyTiles.InitTiles(this); //<<<<see how much cleaner this is? At least in this file!

	struct timeval t1, t2;
	struct timezone tz;
	float deltatime;
	float totaltime = 0.0f;
	unsigned int frames = 0;
	gettimeofday(&t1, &tz);


/**************************************************************************/
// Here's where the main loop occurs
// now we will do a loop to run 10000 cyles to see the result this gives.
// Normally of course you would not run a counter to play a game, but this will let us test a few things
// that guarantee a return in case something stops us breaking into it. (unless code locks somewhere),
// we will replace this with a key test or something better.
	for (int i = 0; i < 10000; i++)
	{
		gettimeofday(&t2, &tz);
		deltatime = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 0.0000001f);
		t1 = t2;

		glClear(GL_COLOR_BUFFER_BIT); // clear the screen, though since you are covering the screen with tiles, you might not need to!

// call our new simple Tile render, all the work is now hidden in the Tiles class
		MyTiles.Render(this);

		int vecSize = MyObjects.size();
// we can go through a vector the same way we can an array to update the objects we put in the vector
		for (int i = 0; i < MyObjects.size(); i++)
		{
			bool IsItAlive =	MyObjects[i]->Update(this);
			MyObjects[i]->Draw();
			if (IsItAlive == false) //if we killed something we should remove it from the list
			{
				delete MyObjects[i]; // it was new'd into existance so we must delete it
				MyObjects.erase(MyObjects.begin() + i); // then remove it from the vector
			}

		}

// put our image on screen
		eglSwapBuffers(OGLES.state.display, OGLES.state.surface);

	//	let our user know how long that all took
		totaltime += deltatime;
		frames++;
		if (totaltime >  1.0f)
		{ // printf is a nice function that allows us to format data, %4d means format to 4 digits integer values
		 // %1.4f means 1 integer and 4 decimal points for a float value
		 // %3.4f means......what do you think?
			printf("%4d frames rendered in %1.4f seconds -> FPS=%3.4f\n", frames, totaltime, frames / totaltime);
			totaltime -= 1.0f;
			frames = 0;
		}

	}

	// clean the memory out before we escape, we new'ed a lot of things, lets delete them, now in fairness, the OS will clear all this up for you when you exit the program
	// but its a very good habit to get into, and can sometimes help you to locae memory leaks you didn't know you had. This loop though is very simple and just removes
	// all those simple player and enemy objects we created, MyTiles was not new'ed so its local and will be cleared on exit.

		for (int i = 0; i < MyObjects.size(); i++)
			delete MyObjects[i];  // delete the objects
		MyObjects.erase(MyObjects.begin(), MyObjects.end());  // clear the vector


	return; // we don't really need the return but it helps us to see the end of the method was reached and we can breakpoint it

};

