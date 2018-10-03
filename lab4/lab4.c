// Demo of heavily simplified sprite engine
// by Ingemar Ragnemalm 2009
// used as base for lab 4 in TSBK03.
// OpenGL 3 conversion 2013.

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif

#include <stdlib.h>
#include <math.h>
#include "LoadTGA.h"
#include "SpriteLight.h"
#include "GL_utilities.h"


// L�gg till egna globaler h�r efter behov.

float kMaxDistance = 200; // maximum neighbor distance
float kAlignWeight = 0.005;
float kCohesionWeight = 0.00001;
float kAvoidWeight = 0.01;

FPoint calculateAvoidance(FPoint i_pos, FPoint j_pos)
{
		FPoint avoidVec;
		avoidVec.h = 0;
		avoidVec.v = 0;

		// Make a vector pointing from i to j
		float diff_h = i_pos.h - j_pos.h;
		float diff_v = i_pos.v - j_pos.v;

		// If the other boid is too close return the vector, otherwise return zero.
		float distance = sqrt(diff_h*diff_h + diff_v*diff_v);
		if(distance < 50)
		{
			avoidVec.h = diff_h;
			avoidVec.v = diff_v;
		}

		return avoidVec;
}

void SpriteBehavior() // Din kod!
{
// L�gg till din labbkod h�r. Det g�r bra att �ndra var som helst i
// koden i �vrigt, men mycket kan samlas h�r. Du kan utg� fr�n den
// globala listroten, gSpriteRoot, f�r att kontrollera alla sprites
// hastigheter och positioner, eller arbeta fr�n egna globaler.

	SpritePtr sp_i = gSpriteRoot;
	int count = 0;

	// For all boids...
	do
	{
		count = 0;
		sp_i->speedDiff.h = 0;
		sp_i->speedDiff.v = 0;
		sp_i->avgPos.h = 0;
		sp_i->avgPos.v = 0;
		sp_i->avoidanceVec.h = 0;
		sp_i->avoidanceVec.v = 0;

		// Add randomness to busig sheep
		if(sp_i->busig)
		{

		}
		else
		{
			SpritePtr sp_j = gSpriteRoot;
			do
			{
				if(sp_i != sp_j)
				{
					float diff_h = sp_j->position.h - sp_i->position.h;
					float diff_v = sp_j->position.v - sp_i->position.v;
					float distance = sqrt(diff_h*diff_h + diff_v*diff_v);
					if(distance < kMaxDistance)
					{
						// For Alignment
						sp_i->speedDiff.h += sp_j->speed.h - sp_i->speed.h;
						sp_i->speedDiff.v += sp_j->speed.v - sp_i->speed.v;

						// For cohesion
						sp_i->avgPos.h += sp_j->position.h;
						sp_i->avgPos.v += sp_j->position.v;

						// For separation: Make a vector pointing away from j
						FPoint avoidance = calculateAvoidance(sp_i->position, sp_j->position);
						sp_i->avoidanceVec.h += avoidance.h;
						sp_i->avoidanceVec.v += avoidance.v;

						count += 1;
					}
				}
				sp_j = sp_j->next;
			} while (sp_j != NULL);

			if( count > 1 )
			{
				sp_i->speedDiff.h /= count;
				sp_i->speedDiff.v /= count;

				sp_i->avgPos.h /= count;
				sp_i->avgPos.v /= count;

				sp_i->avoidanceVec.h /= count;
				sp_i->avoidanceVec.v /= count;
			}

		}

		sp_i = sp_i->next;
	} while (sp_i != NULL);

	// Add the resulting contributions for each boid.
	sp_i = gSpriteRoot;
	do
	{
		sp_i->speed.h += kAlignWeight * sp_i->speedDiff.h
											+ kCohesionWeight * sp_i->avgPos.h
											+ kAvoidWeight * sp_i->avoidanceVec.h;

		sp_i->speed.v += kAlignWeight * sp_i->speedDiff.v
											+ kCohesionWeight * sp_i->avgPos.v
											+ kAvoidWeight * sp_i->avoidanceVec.v;

	  sp_i->position.h += sp_i->speed.h;
		sp_i->position.v += sp_i->speed.v;

		sp_i = sp_i->next;
	} while (sp_i != NULL);
}

// Drawing routine
void Display()
{
	SpritePtr sp;

	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	DrawBackground();

	SpriteBehavior(); // Din kod!

// Loop though all sprites. (Several loops in real engine.)
	sp = gSpriteRoot;
	do
	{
		HandleSprite(sp); // Callback in a real engine
		DrawSprite(sp);
		sp = sp->next;
	} while (sp != NULL);

	glutSwapBuffers();
}

void Reshape(int h, int v)
{
	glViewport(0, 0, h, v);
	gWidth = h;
	gHeight = v;
}

void Timer(int value)
{
	glutTimerFunc(20, Timer, 0);
	glutPostRedisplay();
}

// Example of user controllable parameter
float someValue = 0.0;

void Key(unsigned char key,
         __attribute__((unused)) int x,
         __attribute__((unused)) int y)
{
  switch (key)
  {
    case '+':
    	someValue += 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case '-':
    	someValue -= 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case 0x1b:
      exit(0);
  }
}

void Init()
{
	TextureData *sheepFace, *blackFace;//, *dogFace, *foodFace;

	LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund

	sheepFace = GetFace("bilder/sheep.tga"); // Ett f�r
	blackFace = GetFace("bilder/blackie.tga"); // Ett svart f�r
	// dogFace = GetFace("bilder/dog.tga"); // En hund
	// foodFace = GetFace("bilder/mat.tga"); // Mat

	NewSprite(sheepFace, 562, 200, 1, 1, false);
	NewSprite(sheepFace, 400, 250, -1, 1, false);
	NewSprite(sheepFace, 100, 200, -0.5, 1, false);
	NewSprite(sheepFace, 600, 200, -1, -1, false);
	NewSprite(sheepFace, 100, 582, 1, 1, false);
	NewSprite(sheepFace, 760, 200, 0.2, 1, false);
	NewSprite(sheepFace, 800, 600, -2, -2, false);
	NewSprite(sheepFace, 200, 100, 1.5, -1, false);
	NewSprite(sheepFace, 250, 200, -1, 1.5, false);
	NewSprite(blackFace, 0, 600, -1, -1, true);
	NewSprite(blackFace, 0, 200, 1, 1, true);
	NewSprite(blackFace, 300, 600, -3, -1, true);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SpriteLight demo / Flocking");

	glutDisplayFunc(Display);
	glutTimerFunc(20, Timer, 0); // Should match the screen synch
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);

	InitSpriteLight();
	Init();

	glutMainLoop();
	return 0;
}
