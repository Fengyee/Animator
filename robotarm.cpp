// The sample robotarm model.  You should build a file
// very similar to this for when you make your model.
#pragma warning (disable : 4305)
#pragma warning (disable : 4244)
#pragma warning(disable : 4786)

#define GLEW_STATIC
#include <FL/gl.h>
#include <FL/glut.h>

#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "particleSystem.h"
#include "modelerui.h"
#include "mat.h"
#include "camera.h"
#include "bitmap.h"

#include <stdlib.h>

#define M_DEFAULT 2.0f
#define M_OFFSET 3.0f
#define P_OFFSET 0.3f
#define MAX_VEL 200
#define MIN_STEP 0.1
/*
#define MIKU_CLOTHES true
#define HAVE_HAT = true
#define MIKU_SHOES = true
#define MIKU_HAIR = true
#define DRAW_LEVEL = 3
#define DRAW_TEXTURE = 1
*/
#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.2f, 0.4f, 0.2f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f
#define COLOR_YELLOW    1.0f, 1.0f, 0.5f
#define COLOR_CYAN		0.6f, 0.8f, 0.4f
#define COLOR_GRAY		0.5f, 0.5f, 0.5f
#define COLOR_BROWN		0.6f, 0.2f, 0.2f
#define COLOR_DARK		0.3f, 0.3f, 0.3f
#define COLOR_WHITE		1.0f, 1.0f, 1.0f
#define COLOR_PINK		1.0f, 0.5f, 0.5f
#define COLOR_SKIN		1.0f, 0.8f, 0.6f
#define COLOR_HAIR		1.0f, 0.6f, 0.2f
#define COLOR_DEC		0.4f, 0.4f, 0.2f
#define COLOR_WAIST		0.2f, 0.4f, 0.0f
#define COLOR_HAT		0.8f, 0.4f, 0.2f
#define COLOR_BLACK		0.1f, 0.1f, 0.1f
#define COLOR_MIKU      0.4f, 0.6f, 0.6f
#define COLOR_RIBBON    0.6f, 0.2f, 0.6f

Mat4f WorldMatrix;
Mat4f CameraMatrix;

Mat4f getModelViewMatrix()
{
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	Mat4f matMV(m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]);
	return matMV.transpose(); // because the matrix GL returns is column major
}
void drawParticles(float t, int isMirror) {
	// WorldMatrix = CameraMatrix.inverse() * getModelViewMatrix();
	/*{
		glPushMatrix();
		Vec4f worldpoint = WorldMatrix * Vec4f(0, 0, 0, 1);
		glTranslatef(worldpoint[0], worldpoint[1], worldpoint[2]);
		setDiffuseColor(COLOR_YELLOW);
		drawSphere(0.3);
		glPopMatrix();
	}
	{
		glPushMatrix();
		setDiffuseColor(COLOR_GREEN);
		drawSphere(0.3);
		glPopMatrix();
	}*/
	glPushMatrix();
	// Draw particles
	// glTranslatef(WorldPoint[0], WorldPoint[1], WorldPoint[2]);
	ParticleSystem* ps = ModelerApplication::Instance()->GetParticleSystem();
	if (ps != NULL)
	{
		ps->drawParticles(t, isMirror, 0);
	}
	// printf("current world point: %f, %f, %f, %f\n", WorldPoint[0], WorldPoint[1], WorldPoint[2], WorldPoint[3]);
	glPopMatrix();
}

void drawFire(float t, int isMirror, GLint texName) {
	// WorldMatrix = CameraMatrix.inverse() * getModelViewMatrix();
	/*{
	glPushMatrix();
	Vec4f worldpoint = WorldMatrix * Vec4f(0, 0, 0, 1);
	glTranslatef(worldpoint[0], worldpoint[1], worldpoint[2]);
	setDiffuseColor(COLOR_YELLOW);
	drawSphere(0.3);
	glPopMatrix();
	}
	{
	glPushMatrix();
	setDiffuseColor(COLOR_GREEN);
	drawSphere(0.3);
	glPopMatrix();
	}*/
	glPushMatrix();
	// Draw particles
	// glTranslatef(WorldPoint[0], WorldPoint[1], WorldPoint[2]);
	ParticleSystem* fire = ModelerApplication::Instance()->GetFire();
	if (fire != NULL)
	{
		fire->drawParticles(t, isMirror, texName);
	}
	// printf("current world point: %f, %f, %f, %f\n", WorldPoint[0], WorldPoint[1], WorldPoint[2], WorldPoint[3]);
	glPopMatrix();
}

// This is a list of the controls for the RobotArm
// We'll use these constants to access the values 
// of the controls from the user interface.
/*enum RobotArmControls
{ 
    BASE_ROTATION=0, LOWER_TILT, UPPER_TILT, CLAW_ROTATION,
        BASE_LENGTH, LOWER_LENGTH, UPPER_LENGTH, PARTICLE_COUNT, NUMCONTROLS, 
};*/
enum RobotArmControls
{
	XPOS, YPOS, ZPOS,
	LIGHT0_X, LIGHT0_Y, LIGHT0_Z, LIGHT0_DIFFUSE,
	LIGHT1_X, LIGHT1_Y, LIGHT1_Z, LIGHT1_DIFFUSE,
	ENABLE_LIGHT,
	HEAD_ROTATION_X, HEAD_ROTATION_Y, HEAD_ROTATION_Z,
	HEAD_WIDTH, HEAD_HEIGHT, HEAD_DEPTH,
	LEFT_UPPER_ARM_ROTATION_X, LEFT_UPPER_ARM_ROTATION_Y, LEFT_UPPER_ARM_ROTATION_Z, LEFT_LOWER_ARM_ROTATION_X,
	RIGHT_UPPER_ARM_ROTATION_X, RIGHT_UPPER_ARM_ROTATION_Y, RIGHT_UPPER_ARM_ROTATION_Z, RIGHT_LOWER_ARM_ROTATION_X,
	LEFT_UPPER_LEG_ROTATION_X, LEFT_UPPER_LEG_ROTATION_Y, LEFT_UPPER_LEG_ROTATION_Z, LEFT_LOWER_LEG_ROTATION_X, LEFT_FOOT_ROTATION_X,
	RIGHT_UPPER_LEG_ROTATION_X, RIGHT_UPPER_LEG_ROTATION_Y, RIGHT_UPPER_LEG_ROTATION_Z, RIGHT_LOWER_LEG_ROTATION_X, RIGHT_FOOT_ROTATION_X,

	WAIST_ROTATION_X, WAIST_ROTATION_Y, WAIST_ROTATION_Z,
	DRAW_LEVEL, DRAW_TEXTURE,
	DISPLAY_LSYSTEM, 
	HAVE_HAT, MIKU_CLOTHES, MIKU_SHOES, MIKU_HAIR, HAPPINESS,
	NUMCONTROLS,
};


// To make a RobotArm, we inherit off of ModelerView
class RobotArm : public ModelerView 
{
public:
    RobotArm(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label), textureImage(NULL) {}
	GLuint texName;
	unsigned char* textureImage;
	int tex_height;
	int tex_width;

	void createTexture(int _width);
    virtual void draw();
};

void RobotArm::createTexture(int _width)
{
	static int width = 0;
	static int height = 0;

	if (textureImage == NULL)
	{
		unsigned char *image;
		const char* filename = "C:\\Users\\feng8\\Documents\\GitHub\\Animator\\fire.bmp";
		
		if ((image = readBMP(filename, width, height)) == NULL)
		{
			fl_alert("Can't load bitmap file...");
			// ModelerApplication::Instance()->SetControlValue(DRAW_TEXTURE, 0);
			return;
		}
		// printf("height and width is: %d, %d\n", height, width);
		textureImage = image;
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glShadeModel(GL_FLAT);
		glEnable(GL_DEPTH_TEST);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &texName);
		glBindTexture(GL_TEXTURE_2D, texName);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	/* 
	if (_width <= 0) _width = width;
	if (width > 0 && height > 0)
	{
		double hwfactor = height / (double)width;
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glBindTexture(GL_TEXTURE_2D, texName);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.0, _width * hwfactor, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(_width, _width * hwfactor, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(_width, 0.0, 0.0);
		glEnd();
		glFlush();
		glDisable(GL_TEXTURE_2D);
	}
	*/
}

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createRobotArm(int x, int y, int w, int h, char *label)
{ 
    return new RobotArm(x,y,w,h,label); 
}

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))





// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out RobotArm
void RobotArm::draw()
{
	createTexture(5);
    // This call takes care of a lot of the nasty projection 
    // matrix stuff
    ModelerView::draw(); 
	CameraMatrix = getModelViewMatrix();

	// static GLfloat lmodel_ambient[] = {0.4,0.4,0.4,1.0};

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	glStencilFunc(GL_NEVER, 1, 0xFF);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)

												// draw stencil pattern
	glStencilMask(0xFF);
	glClear(GL_STENCIL_BUFFER_BIT);  // needs mask=0xFF

	/* Now drawing the floor just tags the floor pixels
	as stencil value 1. */
	// draw the floor
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_RED, 0.4);
	glPushMatrix();
	glTranslated(-5, -2, -5);
	drawBox(10, 0.01f, 10);
	glPopMatrix();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	glStencilMask(0x00);
	// draw where stencil's value is 0
	glStencilFunc(GL_EQUAL, 0, 0xFF);
	/* (nothing to draw) */
	// draw only where stencil's value is 1
	glStencilFunc(GL_EQUAL, 1, 0xFF);

	// draw the sample model
	setAmbientColor(.1f,.1f,.1f);
	int isMirror = 1;

	glPushMatrix();
	if (isMirror == 1)
	{
		glTranslated(0, -4, 0);
		glScaled(1, -1, 1);
	}
drawModel:
	{
		glPushMatrix();

		int level_of_detail = (int)VAL(DRAW_LEVEL);
		glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));

		   //set variables
		float unit_block = 0.75;
		float body_x = 2.0;
		float body_y = 3.0;
		float body_z = 1.0;
		float head = 2.0;
		float arm_y = 1.5;
		float arm_x = 0.9;
		float arm_z = 1.0;
		float foot_y = 0.5;
		float foot_x = 0.9;
		float foot_z = 1.1;
		float body_init_y = -2 + unit_block * (arm_y * 2 + foot_y);
		float body_init_x = -1 * body_y / 2 * unit_block;
		float body_init_z = -1;
		float thick = 0.01;
		float hair_thick = 0.1;
		float eye_x = 0.5;
		float eye_y = 0.25;
		float mouth_x = 0.5;
		float mouth_y = 0.25;
		float sleeve_y = 0.25;
		// draw body
		glPushMatrix();
		glTranslated(body_init_x, body_init_y, body_init_z);

		if (VAL(DRAW_LEVEL) > 2)
		{
			if (VAL(MIKU_CLOTHES))
			{
				// waist
				setDiffuseColor(COLOR_BLACK);
				glPushMatrix();
				glTranslated(0, 0, -1 * thick);
				glScaled(body_x * unit_block, 1 * unit_block, 1);
				drawBox(1, 1, thick);
				glTranslated(0, 0, body_z * unit_block + thick);
				glScaled(1, 1, 1);
				drawBox(1, 1, thick);
				glPopMatrix();
				glPushMatrix();
				glTranslated(-1 * thick, 0, -1 * thick);
				glScaled(1, 1 * unit_block, body_z * unit_block);
				drawBox(thick, 1, 1);
				glTranslated(body_x * unit_block + thick, 0, 0);
				glScaled(1, 1, 1);
				drawBox(thick, 1, 1);
				glPopMatrix();

				// tie
				setDiffuseColor(COLOR_MIKU);
				glPushMatrix();
				glTranslated(body_x * unit_block / 2, (body_y - 0.3) * unit_block, body_z * unit_block);
				glRotated(45, 0.0, 0.0, 1.0);
				glScaled(0.3, 0.3, thick);
				drawTriangle(1, 0, 0, 0, 1, 0, 0, 0, 1);
				glPopMatrix();
				glPushMatrix();
				glTranslated(body_x * unit_block / 2, (body_y - 0.3) * unit_block, body_z * unit_block);
				glRotated(-135, 0.0, 0.0, 1.0);
				glScaled(1, 1, thick);
				drawTriangle(0.75, 0.25, 0, 0.25, 0.75, 0, 0, 0, 1);
				glPopMatrix();
				glPushMatrix();
				glTranslated(body_x * unit_block / 2, (body_y - 1.7) * unit_block, body_z * unit_block);
				glRotated(45, 0.0, 0.0, 1.0);
				glScaled(0.5, 0.5, thick);
				drawTriangle(1, 0, 0, 0, 1, 0, 0, 0, 1);
				glPopMatrix();
			}
			else
			{
				// waist
				setDiffuseColor(COLOR_WAIST);
				glPushMatrix();
				glTranslated(0, 0.5 * unit_block, -1 * thick);
				glScaled(body_x * unit_block, sleeve_y * unit_block, 1);
				drawBox(1, 1, thick);
				glTranslated(0, 0, body_z * unit_block + thick);
				glScaled(1, 1, 1);
				drawBox(1, 1, thick);
				glPopMatrix();
				glPushMatrix();
				glTranslated(-1 * thick, 0.5 * unit_block, -1 * thick);
				glScaled(1, sleeve_y * unit_block, body_z * unit_block);
				drawBox(thick, 1, 1);
				glTranslated(body_x * unit_block + thick, 0, 0);
				glScaled(1, 1, 1);
				drawBox(thick, 1, 1);
				glPopMatrix();

				// NECK
				glPushMatrix();
				glTranslated(body_x * unit_block / 2, (body_y - 0.95) * unit_block, body_z * unit_block);
				glRotated(45, 0.0, 0.0, 1.0);
				setDiffuseColor(COLOR_DEC);
				glPushMatrix();
				glTranslated(-1 * sleeve_y * unit_block, -1 * sleeve_y * unit_block, 0);
				glScaled(sleeve_y * unit_block, 1 + sleeve_y * unit_block * 2, thick);
				drawBox(1, 1, 1);
				glPopMatrix();
				glPushMatrix();
				glTranslated(-1 * sleeve_y * unit_block, -1 * sleeve_y * unit_block, 0);
				glScaled(1 + sleeve_y * unit_block * 2, sleeve_y * unit_block, thick);
				drawBox(1, 1, 1);
				glPopMatrix();
				setDiffuseColor(COLOR_SKIN);
				glScaled(1, 1, thick);
				drawTriangle(1, 0, 0, 0, 1, 0, 0, 0, 1);
				glPopMatrix();

			}
		}
		// draw head
		glPushMatrix();

		glTranslated(0, body_y * unit_block, -1 * (head - body_z) / 2 * unit_block);

		// head resize implementation
		glTranslated(head / 2 * unit_block, 0, head / 2 * unit_block);
		glScaled(VAL(HEAD_WIDTH), VAL(HEAD_HEIGHT), VAL(HEAD_DEPTH));
		glTranslated(-1 * head / 2 * unit_block, 0, -1 * head / 2 * unit_block);
		// HEAD ROTATION IMPLEMENTATION
		glTranslated(head / 2 * unit_block, 0, head / 2 * unit_block);
		glRotated(VAL(HEAD_ROTATION_X) * (VAL(HAPPINESS) + 2) / 2, 1.0, 0.0, 0.0);
		glRotated(VAL(HEAD_ROTATION_Y) * (VAL(HAPPINESS) + 2) / 2, 0.0, 1.0, 0.0);
		glRotated(VAL(HEAD_ROTATION_Z) * (VAL(HAPPINESS) + 2) / 2, 0.0, 0.0, 1.0);
		glTranslated(-1 * head / 2 * unit_block, 0, -1 * head / 2 * unit_block);

		// torus
		if (VAL(DRAW_LEVEL) > 2)
		{
			glPushMatrix();
			setDiffuseColor(COLOR_YELLOW);
			glTranslated((head - 0.1) * unit_block, (head + 1) * unit_block, head / 2 * unit_block);
			//drawTorus(1.2, 1.0);
			glPopMatrix();
		}

		// hat
		if (VAL(HAVE_HAT))
		{
			glPushMatrix();
			glTranslated(head / 2 * unit_block, head * unit_block, head / 2 * unit_block);
			for (int i = 0; i < 8; i++)
			{
				if (i % 2 == 0)
					setDiffuseColor(COLOR_BROWN);
				else
					setDiffuseColor(COLOR_HAT);
				glRotated(45, 0, 1, 0);
				drawTriangle(1.5, -0.2, 1.5, 0, 0.5, 0, 0, -0.2, sqrt(4.5));
			}
			glPopMatrix();
		}

		if (VAL(DRAW_LEVEL) > 1)
		{
			// eyes and mouth
			if (VAL(MIKU_HAIR))
			{
				glPushMatrix();
				setDiffuseColor(COLOR_BLACK);
				glTranslated(0.25 * unit_block, 1.0 * unit_block, head * unit_block);
				glScaled(eye_x * unit_block, eye_y * unit_block, thick);
				drawBox(1, 1, 1);
				setDiffuseColor(COLOR_WHITE);
				glTranslated(0, -2, 0);
				glScaled(0.5, 2, 1);
				drawBox(1, 1, 1);
				setDiffuseColor(COLOR_MIKU);
				glTranslated(1, 0, 0);
				glScaled(1, 1, 1);
				drawBox(1, 1, 1);
				glPopMatrix();

				glPushMatrix();
				setDiffuseColor(COLOR_BLACK);
				glTranslated((0.25 + head / 2) * unit_block, 1.0 * unit_block, head * unit_block);
				glScaled(eye_x * unit_block, eye_y * unit_block, thick);
				drawBox(1, 1, 1);
				setDiffuseColor(COLOR_MIKU);
				glTranslated(0, -2, 0);
				glScaled(0.5, 2, 1);
				drawBox(1, 1, 1);
				setDiffuseColor(COLOR_WHITE);
				glTranslated(1, 0, 0);
				glScaled(1, 1, 1);
				drawBox(1, 1, 1);
				glPopMatrix();
			}
			else
			{
				glPushMatrix();
				setDiffuseColor(COLOR_WHITE);
				glTranslated(0.25 * unit_block, 0.75 * unit_block, head * unit_block);
				glScaled(eye_x / 2 * unit_block, eye_y * unit_block, thick);
				drawBox(1, 1, 1);
				setDiffuseColor(COLOR_GREEN);
				glTranslated(1, 0, 0);
				glScaled(1, 1, 1);
				drawBox(1, 1, 1);
				glPopMatrix();
				glPushMatrix();
				setDiffuseColor(COLOR_GREEN);
				glTranslated((0.25 + head / 2) * unit_block, 0.75 * unit_block, head * unit_block);
				glScaled(eye_x / 2 * unit_block, eye_y * unit_block, thick);
				drawBox(1, 1, 1);
				setDiffuseColor(COLOR_WHITE);
				glTranslated(1, 0, 0);
				glScaled(1, 1, 1);
				drawBox(1, 1, 1);
				glPopMatrix();

				// mouth
				glPushMatrix();
				setDiffuseColor(COLOR_PINK);
				glTranslated(0.75 * unit_block, 0.25 * unit_block, head * unit_block);
				glScaled(mouth_x * unit_block, mouth_y * unit_block, thick);
				drawBox(1, 1, 1);
				glPopMatrix();
			}
		}

		if (VAL(DRAW_LEVEL) > 2)
		{
			// hair
			glPushMatrix();
			if (VAL(MIKU_HAIR))
				setDiffuseColor(COLOR_MIKU);
			else
				setDiffuseColor(COLOR_HAIR);
			glTranslated(0, head * unit_block, head * unit_block);
			glRotated(-90, 0.0, 0.0, 1.0);
			glScaled(head / 2 * unit_block, head / 2 * unit_block, hair_thick);
			drawTriangle(1, 0, 0, 0, 1, 0, 0, 0, 1);
			glPopMatrix();
			glPushMatrix();
			glTranslated(head * unit_block, head * unit_block, head * unit_block);
			glRotated(180, 0.0, 0.0, 1.0);
			glScaled(head / 2 * unit_block, head / 2 * unit_block, hair_thick);
			drawTriangle(1, 0, 0, 0, 1, 0, 0, 0, 1);
			glPopMatrix();

			glPushMatrix();
			glTranslated(0, 0, -1 * hair_thick);
			glScaled(head * unit_block, head * unit_block, hair_thick);
			drawBox(1, 1, 1);
			glPopMatrix();

			glPushMatrix();
			glTranslated(-1 * hair_thick, head * unit_block, -1 * hair_thick);
			glScaled(head * unit_block + 2 * hair_thick, hair_thick, head * unit_block + 2 * hair_thick);
			drawBox(1, 1, 1);
			glPopMatrix();

			glPushMatrix();
			glTranslated(-1 * hair_thick, head / 2 * unit_block, -1 * hair_thick);
			if (VAL(MIKU_HAIR))
			{
				glPushMatrix();
				glTranslated(-0.2 * unit_block, 0.25 * unit_block, 0.1 * unit_block);
				glPushMatrix();
				glTranslated(-0.3 * unit_block, -1 * unit_block, 0.3 * unit_block);
				glScaled(0.3 * unit_block, 1 * unit_block, 0.3 * unit_block);
				drawBox(1, 1, 1);
				glTranslated(-0.33, -2.6, -1);
				glScaled(1.5, 3, 1.5);
				drawBox(1, 1, 1);
				glPopMatrix();
				setDiffuseColor(COLOR_RIBBON);
				glRotated(45, 1, 0, 0);
				glScaled(0.2 * unit_block, 0.7 * unit_block, 0.7 * unit_block);
				drawBox(1, 1, 1);
				glPopMatrix();
				setDiffuseColor(COLOR_MIKU);
			}
			glScaled(hair_thick, head / 2 * unit_block, head * unit_block + 2 * hair_thick);
			drawBox(1, 1, 1);
			glTranslated(0, -1, 0);
			glScaled(1, 1, 0.6);
			drawBox(1, 1, 1);
			glPopMatrix();

			glPushMatrix();
			glTranslated(head * unit_block, head / 2 * unit_block, -1 * hair_thick);
			if (VAL(MIKU_HAIR))
			{
				glPushMatrix();
				glTranslated(hair_thick, 0.25 * unit_block, 0.1 * unit_block);
				glPushMatrix();
				glTranslated(0.2 * unit_block, -1 * unit_block, 0.3 * unit_block);
				glScaled(0.3 * unit_block, 1 * unit_block, 0.3 * unit_block);
				drawBox(1, 1, 1);
				glTranslated(0, -2.6, -1);
				glScaled(1.5, 3, 1.5);
				drawBox(1, 1, 1);
				glPopMatrix();
				setDiffuseColor(COLOR_RIBBON);
				glRotated(45, 1, 0, 0);
				glScaled(0.2 * unit_block, 0.7 * unit_block, 0.7 * unit_block);
				drawBox(1, 1, 1);
				glPopMatrix();
				setDiffuseColor(COLOR_MIKU);
			}
			glScaled(hair_thick, head / 2 * unit_block, head * unit_block + 2 * hair_thick);
			drawBox(1, 1, 1);
			glTranslated(0, -1, 0);
			glScaled(1, 1, 0.6);
			drawBox(1, 1, 1);
			glPopMatrix();
		}
		// back to head
		setDiffuseColor(COLOR_SKIN);

		glScaled(head * unit_block, head * unit_block, head * unit_block);
		drawBox(1, 1, 1);

		glPopMatrix();

		// draw left hand
		glPushMatrix();
		//glTranslated(-1, 0.8, 0.75);
		//glRotated(-90, 0.0, 1.0, 0.0);
		//drawCylinder(0.4, 0.2, 0.2);

		//glRotated(90, 0.0, 1.0, 0.0);
		glTranslated(-1 * arm_x * unit_block, body_y * unit_block - arm_y * unit_block, 0);

		// WHOLE ARM ROTATION IMPLEMENTATION
		glTranslated(arm_x * unit_block, arm_y * unit_block, arm_z / 2 * unit_block);
		glRotated(VAL(LEFT_UPPER_ARM_ROTATION_X) * (VAL(HAPPINESS) + 2) / 2, 1.0, 0.0, 0.0);
		glRotated(VAL(LEFT_UPPER_ARM_ROTATION_Y) * (VAL(HAPPINESS) + 2) / 2, 0.0, 1.0, 0.0);
		glRotated(VAL(LEFT_UPPER_ARM_ROTATION_Z) * (VAL(HAPPINESS) + 2) / 2, 0.0, 0.0, 1.0);
		glTranslated(-1 * arm_x * unit_block, -1 * arm_y * unit_block, -1 * arm_z / 2 * unit_block);

		if (VAL(DRAW_LEVEL) > 1)
		{
			// sleeve
			if (VAL(MIKU_CLOTHES))
				setDiffuseColor(COLOR_MIKU);
			else
				setDiffuseColor(COLOR_DEC);
			glPushMatrix();
			glTranslated(0, 0, -1 * thick);
			glScaled(arm_x * unit_block, sleeve_y * unit_block, 1);
			drawBox(1, 1, thick);
			glTranslated(0, 0, arm_z * unit_block + thick);
			glScaled(1, 1, 1);
			drawBox(1, 1, thick);
			glPopMatrix();
			glPushMatrix();
			glTranslated(-1 * thick, 0, -1 * thick);
			glScaled(1, sleeve_y * unit_block, arm_z * unit_block);
			drawBox(thick, 1, 1);
			glTranslated(arm_x * unit_block + thick, 0, 0);
			glScaled(1, 1, 1);
			drawBox(thick, 1, 1);
			glPopMatrix();
		}

		// lower left arm
		glPushMatrix();

		glTranslated(0, -1 * arm_y * unit_block, 0);
		// lower arm rotation implementation
		glTranslated(arm_x / 2 * unit_block, arm_y * unit_block, arm_z / 2 * unit_block);
		glRotated(VAL(LEFT_LOWER_ARM_ROTATION_X) * (VAL(HAPPINESS) + 2) / 2, 1.0, 0.0, 0.0);
		glTranslated(-1 * arm_x / 2 * unit_block, -1 * arm_y * unit_block, -1 * arm_z / 2 * unit_block);

		// lower arm sleeve
		if (VAL(MIKU_CLOTHES))
		{
			if (VAL(DRAW_LEVEL) > 1)
			{
				setDiffuseColor(COLOR_MIKU);
				glPushMatrix();
				glTranslated(0, 0, -1 * thick);
				glScaled(arm_x * unit_block, sleeve_y * unit_block, 1);
				drawBox(1, 1, thick);
				glTranslated(0, 0, arm_z * unit_block + thick);
				glScaled(1, 1, 1);
				drawBox(1, 1, thick);
				glPopMatrix();
				glPushMatrix();
				glTranslated(-1 * thick, 0, -1 * thick);
				glScaled(1, sleeve_y * unit_block, arm_z * unit_block);
				drawBox(thick, 1, 1);
				glTranslated(arm_x * unit_block + thick, 0, 0);
				glScaled(1, 1, 1);
				drawBox(thick, 1, 1);
				glPopMatrix();
			}
		}

		if (VAL(MIKU_CLOTHES))
			setDiffuseColor(COLOR_BLACK);
		else
			setDiffuseColor(COLOR_SKIN);
		glScaled(arm_x * unit_block, arm_y * unit_block, arm_z * unit_block);

		// draw left lower arm
		drawBox(1, 1, 1);
		glPopMatrix();

		// back to upper left arm
		if (VAL(MIKU_CLOTHES))
			setDiffuseColor(COLOR_SKIN);
		else
			setDiffuseColor(COLOR_CYAN);
		glScaled(arm_x * unit_block, arm_y * unit_block, arm_z * unit_block);
		drawBox(1, 1, 1);
		glPopMatrix();


		// draw right hand
		glPushMatrix();
		//glTranslated(1.0, 0.8, 0.75);
		//glRotated(90, 0.0, 1.0, 0.0);
		//drawCylinder(0.4, 0.2, 0.2);

		//glRotated(-90, 0.0, 1.0, 0.0);

		glTranslated(body_x * unit_block, body_y * unit_block - arm_y * unit_block, 0);

		// WHOLE ARM ROTATION IMPLEMENTATION
		glTranslated(0, arm_y * unit_block, arm_z / 2 * unit_block);
		glRotated(VAL(RIGHT_UPPER_ARM_ROTATION_X) * (VAL(HAPPINESS) + 2) / 2, 1.0, 0.0, 0.0);
		glRotated(VAL(RIGHT_UPPER_ARM_ROTATION_Y) * (VAL(HAPPINESS) + 2) / 2, 0.0, 1.0, 0.0);
		glRotated(VAL(RIGHT_UPPER_ARM_ROTATION_Z) * (VAL(HAPPINESS) + 2) / 2, 0.0, 0.0, 1.0);
		glTranslated(0, -1 * arm_y * unit_block, -1 * arm_z / 2 * unit_block);


		if (VAL(DRAW_LEVEL) > 1)
		{
			// SLEEVE
			if (VAL(MIKU_CLOTHES))
				setDiffuseColor(COLOR_MIKU);
			else
				setDiffuseColor(COLOR_DEC);
			glPushMatrix();
			glTranslated(0, 0, -1 * thick);
			glScaled(arm_x * unit_block, sleeve_y * unit_block, 1);
			drawBox(1, 1, thick);
			glTranslated(0, 0, arm_z * unit_block + thick);
			glScaled(1, 1, 1);
			drawBox(1, 1, thick);
			glPopMatrix();
			glPushMatrix();
			glTranslated(-1 * thick, 0, -1 * thick);
			glScaled(1, sleeve_y * unit_block, arm_z * unit_block);
			drawBox(thick, 1, 1);
			glTranslated(arm_x * unit_block + thick, 0, 0);
			glScaled(1, 1, 1);
			drawBox(thick, 1, 1);
			glPopMatrix();
		}

		// lower right arm
		glPushMatrix();

		glTranslated(0, -1 * arm_y * unit_block, 0);
		// lower arm rotation implementation
		glTranslated(arm_x / 2 * unit_block, arm_y * unit_block, arm_z / 2 * unit_block);
		glRotated(VAL(RIGHT_LOWER_ARM_ROTATION_X) * (VAL(HAPPINESS) + 2) / 2, 1.0, 0.0, 0.0);
		glTranslated(-1 * arm_x / 2 * unit_block, -1 * arm_y * unit_block, -1 * arm_z / 2 * unit_block);

		// lower arm sleeve
		if (VAL(MIKU_CLOTHES))
		{
			if (VAL(DRAW_LEVEL) > 1)
			{
				setDiffuseColor(COLOR_MIKU);
				glPushMatrix();
				glTranslated(0, 0, -1 * thick);
				glScaled(arm_x * unit_block, sleeve_y * unit_block, 1);
				drawBox(1, 1, thick);
				glTranslated(0, 0, arm_z * unit_block + thick);
				glScaled(1, 1, 1);
				drawBox(1, 1, thick);
				glPopMatrix();
				glPushMatrix();
				glTranslated(-1 * thick, 0, -1 * thick);
				glScaled(1, sleeve_y * unit_block, arm_z * unit_block);
				drawBox(thick, 1, 1);
				glTranslated(arm_x * unit_block + thick, 0, 0);
				glScaled(1, 1, 1);
				drawBox(thick, 1, 1);
				glPopMatrix();
			}
		}

		if (VAL(MIKU_CLOTHES))
			setDiffuseColor(COLOR_BLACK);
		else
			setDiffuseColor(COLOR_SKIN);
		glScaled(arm_x * unit_block, arm_y * unit_block, arm_z * unit_block);
		// draw right lower arm
		drawBox(1, 1, 1);
		glPopMatrix();

		// BACK TO ARM
		if (VAL(MIKU_CLOTHES))
			setDiffuseColor(COLOR_SKIN);
		else
			setDiffuseColor(COLOR_CYAN);
		glScaled(arm_x * unit_block, arm_y * unit_block, arm_z * unit_block);
		drawBox(1, 1, 1);
		glPopMatrix();

		// draw left leg
		glPushMatrix();
		glTranslated((body_x / 2 - arm_x) / 2 * unit_block, -1 * arm_y * unit_block, 0);

		// WHOLE LEG ROTATION IMPLEMENTATION
		glTranslated(arm_x * unit_block, arm_y * unit_block, arm_z / 2 * unit_block);
		glRotated(VAL(LEFT_UPPER_LEG_ROTATION_X) * (VAL(HAPPINESS) + 2) / 2, 1.0, 0.0, 0.0);
		glRotated(VAL(LEFT_UPPER_LEG_ROTATION_Y) * (VAL(HAPPINESS) + 2) / 2, 0.0, 1.0, 0.0);
		glRotated(VAL(LEFT_UPPER_LEG_ROTATION_Z) * (VAL(HAPPINESS) + 2) / 2, 0.0, 0.0, 1.0);
		glTranslated(-1 * arm_x * unit_block, -1 * arm_y * unit_block, -1 * arm_z / 2 * unit_block);

		// sleeve
		if (VAL(DRAW_LEVEL) > 1)
		{
			if (VAL(MIKU_SHOES))
				setDiffuseColor(COLOR_MIKU);
			else
				setDiffuseColor(COLOR_DARK);
			glPushMatrix();
			glTranslated(0, 0, -1 * thick);
			glScaled(arm_x * unit_block, sleeve_y * unit_block, 1);
			drawBox(1, 1, thick);
			glTranslated(0, 0, arm_z * unit_block + thick);
			glScaled(1, 1, 1);
			drawBox(1, 1, thick);
			glPopMatrix();
			glPushMatrix();
			glTranslated(-1 * thick, 0, -1 * thick);
			glScaled(1, sleeve_y * unit_block, arm_z * unit_block);
			drawBox(thick, 1, 1);
			glTranslated(arm_x * unit_block + thick, 0, 0);
			glScaled(1, 1, 1);
			drawBox(thick, 1, 1);
			glPopMatrix();
		}

		if (VAL(MIKU_SHOES))
		{
			setDiffuseColor(COLOR_MIKU);
			glPushMatrix();
			glTranslated(0, (arm_y - sleeve_y * 2) * unit_block, -1 * thick);
			glScaled(arm_x * unit_block, sleeve_y * unit_block * 2, 1);
			drawBox(1, 1, thick);
			glTranslated(0, 0, arm_z * unit_block + thick);
			glScaled(1, 1, 1);
			drawBox(1, 1, thick);
			glPopMatrix();
			glPushMatrix();
			glTranslated(-1 * thick, (arm_y - sleeve_y * 2) * unit_block, -1 * thick);
			glScaled(1, sleeve_y * unit_block * 2, arm_z * unit_block);
			drawBox(thick, 1, 1);
			glTranslated(arm_x * unit_block + thick, 0, 0);
			glScaled(1, 1, 1);
			drawBox(thick, 1, 1);
			glPopMatrix();
		}

		// left lower leg
		glPushMatrix();
		glTranslated(0, -1 * arm_y * unit_block, 0);
		// lower leg rotation implementation
		glTranslated(arm_x / 2 * unit_block, arm_y * unit_block, arm_z * unit_block);
		glRotated(VAL(LEFT_LOWER_LEG_ROTATION_X) * (VAL(HAPPINESS) + 2) / 2, 1.0, 0.0, 0.0);
		glTranslated(-1 * arm_x / 2 * unit_block, -1 * arm_y * unit_block, -1 * arm_z * unit_block);

		// left foot
		glPushMatrix();
		if (VAL(MIKU_SHOES))
			setDiffuseColor(COLOR_MIKU);
		else
			setDiffuseColor(COLOR_GRAY);
		glTranslated((arm_x - foot_x) / 2 * unit_block, -1 * foot_y * unit_block, (arm_z - foot_z) / 2 * unit_block);
		// foot rotation implementation
		glTranslated(foot_x / 2 * unit_block, foot_y * unit_block, foot_z / 2 * unit_block);
		glRotated(VAL(LEFT_FOOT_ROTATION_X) * (VAL(HAPPINESS) + 2) / 2, 1.0, 0.0, 0.0);
		glTranslated(-1 * foot_x / 2 * unit_block, -1 * foot_y * unit_block, -1 * foot_z / 2 * unit_block);
		glScaled(foot_x * unit_block, foot_y * unit_block, foot_z * unit_block);
		drawBox(1, 1, 1);
		glPopMatrix();

		if (VAL(MIKU_SHOES))
			setDiffuseColor(COLOR_BLACK);
		else
			setDiffuseColor(COLOR_SKIN);
		glScaled(arm_x * unit_block, arm_y * unit_block, arm_z * unit_block);
		drawBox(1, 1, 1);
		glPopMatrix();

		// back to leg
		if (VAL(MIKU_SHOES))
			setDiffuseColor(COLOR_SKIN);
		else
			setDiffuseColor(COLOR_BROWN);
		glScaled(arm_x * unit_block, arm_y * unit_block, arm_z * unit_block);
		drawBox(1, 1, 1);
		glPopMatrix();


		// draw right leg
		glPushMatrix();
		glTranslated(body_x / 2 * unit_block + (body_x / 2 - arm_x) / 2 * unit_block, -1 * arm_y * unit_block, 0);

		// WHOLE LEG ROTATION IMPLEMENTATION
		glTranslated(arm_x * unit_block, arm_y * unit_block, arm_z / 2 * unit_block);
		glRotated(VAL(RIGHT_UPPER_LEG_ROTATION_X) * (VAL(HAPPINESS) + 2) / 2, 1.0, 0.0, 0.0);
		glRotated(VAL(RIGHT_UPPER_LEG_ROTATION_Y) * (VAL(HAPPINESS) + 2) / 2, 0.0, 1.0, 0.0);
		glRotated(VAL(RIGHT_UPPER_LEG_ROTATION_Z) * (VAL(HAPPINESS) + 2) / 2, 0.0, 0.0, 1.0);
		glTranslated(-1 * arm_x * unit_block, -1 * arm_y * unit_block, -1 * arm_z / 2 * unit_block);

		// sleeve
		if (VAL(DRAW_LEVEL) > 1)
		{
			if (VAL(MIKU_SHOES))
				setDiffuseColor(COLOR_MIKU);
			else
				setDiffuseColor(COLOR_DARK);
			glPushMatrix();
			glTranslated(0, 0, -1 * thick);
			glScaled(arm_x * unit_block, sleeve_y * unit_block, 1);
			drawBox(1, 1, thick);
			glTranslated(0, 0, arm_z * unit_block + thick);
			glScaled(1, 1, 1);
			drawBox(1, 1, thick);
			glPopMatrix();
			glPushMatrix();
			glTranslated(-1 * thick, 0, -1 * thick);
			glScaled(1, sleeve_y * unit_block, arm_z * unit_block);
			drawBox(thick, 1, 1);
			glTranslated(arm_x * unit_block + thick, 0, 0);
			glScaled(1, 1, 1);
			drawBox(thick, 1, 1);
			glPopMatrix();
		}

		if (VAL(MIKU_SHOES))
		{
			setDiffuseColor(COLOR_MIKU);
			glPushMatrix();
			glTranslated(0, (arm_y - sleeve_y * 2) * unit_block, -1 * thick);
			glScaled(arm_x * unit_block, sleeve_y * unit_block * 2, 1);
			drawBox(1, 1, thick);
			glTranslated(0, 0, arm_z * unit_block + thick);
			glScaled(1, 1, 1);
			drawBox(1, 1, thick);
			glPopMatrix();
			glPushMatrix();
			glTranslated(-1 * thick, (arm_y - sleeve_y * 2) * unit_block, -1 * thick);
			glScaled(1, sleeve_y * unit_block * 2, arm_z * unit_block);
			drawBox(thick, 1, 1);
			glTranslated(arm_x * unit_block + thick, 0, 0);
			glScaled(1, 1, 1);
			drawBox(thick, 1, 1);
			glPopMatrix();
		}

		glPushMatrix();
		glTranslated(0, -1 * arm_y * unit_block, 0);
		// lower leg rotation implementation
		glTranslated(arm_x / 2 * unit_block, arm_y * unit_block, arm_z * unit_block);
		glRotated(VAL(RIGHT_LOWER_LEG_ROTATION_X) * (VAL(HAPPINESS) + 2) / 2, 1.0, 0.0, 0.0);
		glTranslated(-1 * arm_x / 2 * unit_block, -1 * arm_y * unit_block, -1 * arm_z * unit_block);

		// right foot
		glPushMatrix();
		if (VAL(MIKU_SHOES))
			setDiffuseColor(COLOR_MIKU);
		else
			setDiffuseColor(COLOR_GRAY);
		glTranslated((arm_x - foot_x) / 2 * unit_block, -1 * foot_y * unit_block, (arm_z - foot_z) / 2 * unit_block);
		// foot rotation implementation
		glTranslated(foot_x / 2 * unit_block, foot_y * unit_block, foot_z / 2 * unit_block);
		glRotated(VAL(RIGHT_FOOT_ROTATION_X) * (VAL(HAPPINESS) + 2) / 2, 1.0, 0.0, 0.0);
		glTranslated(-1 * foot_x / 2 * unit_block, -1 * foot_y * unit_block, -1 * foot_z / 2 * unit_block);
		glScaled(foot_x * unit_block, foot_y * unit_block, foot_z * unit_block);
		drawBox(1, 1, 1);
		glPopMatrix();

		if (VAL(MIKU_SHOES))
			setDiffuseColor(COLOR_BLACK);
		else
			setDiffuseColor(COLOR_SKIN);
		glScaled(arm_x * unit_block, arm_y * unit_block, arm_z * unit_block);
		drawBox(1, 1, 1);
		glPopMatrix();

		// back to leg
		if (VAL(MIKU_SHOES))
			setDiffuseColor(COLOR_SKIN);
		else
			setDiffuseColor(COLOR_BROWN);
		glScaled(arm_x * unit_block, arm_y * unit_block, arm_z * unit_block);
		drawBox(1, 1, 1);
		glPopMatrix();


		// back body
		if (MIKU_CLOTHES)
		{
			setDiffuseColor(COLOR_GRAY);
		}
		else
		{
			setDiffuseColor(COLOR_CYAN);
		}
		glScaled(body_x * unit_block, body_y * unit_block, body_z * unit_block);
		WorldMatrix = CameraMatrix.inverse() * getModelViewMatrix();
		drawBox(1, 1, 1);
		glPopMatrix();
		/*
		glPushMatrix();
		glRotated(1, 0.0, 1.0, 0.0);
		glRotated(-90, 1.0, 0.0, 0.0);
		drawCylinder(2, 0.1, 0.1);

		glTranslated(0.0, 0.0, 1);
		drawCylinder(1, 1.0, 0.9);

		glTranslated(0.0, 0.0, 1);
		glRotated(90, 1.0, 0.0, 0.0);
		drawCylinder(1, 0.1, 0.2);
		glPopMatrix();
		*/
		glPopMatrix();

		// drawParticles(t, isMirror);
		drawFire(t, isMirror, texName);
	}
	glPopMatrix();
	if (isMirror == 1)
	{
		glDisable(GL_STENCIL_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			setAmbientColor(.1f, .1f, .1f);
			setDiffuseColor(COLOR_RED, 0.4);
			glPushMatrix();
			glTranslated(-5, -2, -5);
			drawBox(10, 0.01f, 10);
			glPopMatrix();
		glDisable(GL_BLEND);

		isMirror = 0;
		glPushMatrix();
		goto drawModel;
	}
		
	//*** DON'T FORGET TO PUT THIS IN YOUR OWN CODE **/
	endDraw();
}


int main()
{
    ModelerControl controls[NUMCONTROLS ];
	/*
	controls[BASE_ROTATION] = ModelerControl("base rotation (theta)", -180.0, 180.0, 0.1, 0.0 );
    controls[LOWER_TILT] = ModelerControl("lower arm tilt (phi)", 15.0, 95.0, 0.1, 55.0 );
    controls[UPPER_TILT] = ModelerControl("upper arm tilt (psi)", 0.0, 135.0, 0.1, 30.0 );
	controls[CLAW_ROTATION] = ModelerControl("claw rotation (cr)", -30.0, 180.0, 0.1, 0.0 );
    controls[BASE_LENGTH] = ModelerControl("base height (h1)", 0.5, 10.0, 0.1, 0.8 );
    controls[LOWER_LENGTH] = ModelerControl("lower arm length (h2)", 1, 10.0, 0.1, 3.0 );
    controls[UPPER_LENGTH] = ModelerControl("upper arm length (h3)", 1, 10.0, 0.1, 2.5 );
    controls[PARTICLE_COUNT] = ModelerControl("particle count (pc)", 0.0, 5.0, 0.1, 5.0 );
    */

	controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
	controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
	controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);

	controls[LIGHT0_X] = ModelerControl("Light0 X", -10, 10, 0.1f, 4);
	controls[LIGHT0_Y] = ModelerControl("Light0 Y", -10, 10, 0.1f, 2);
	controls[LIGHT0_Z] = ModelerControl("Light0 Z", -10, 10, 0.1f, -4);
	controls[LIGHT0_DIFFUSE] = ModelerControl("Light0 Diffuse", 0, 5, 0.01f, 1);

	controls[LIGHT1_X] = ModelerControl("Light1 X", -10, 10, 0.1f, -2);
	controls[LIGHT1_Y] = ModelerControl("Light1 Y", -10, 10, 0.1f, 1);
	controls[LIGHT1_Z] = ModelerControl("Light1 Z", -10, 10, 0.1f, 5);
	controls[LIGHT1_DIFFUSE] = ModelerControl("Light1 Diffuse", 0, 5, 0.01f, 1);
	controls[ENABLE_LIGHT] = ModelerControl("Enable lights", 0, 1, 1, 0);

	controls[HEAD_ROTATION_X] = ModelerControl("Head rotation X", -30, 30, 0.01f, 0);
	controls[HEAD_ROTATION_Y] = ModelerControl("Head rotation Y", -60, 60, 0.01f, 0);
	controls[HEAD_ROTATION_Z] = ModelerControl("Head rotation Z", -30, 30, 0.01f, 0);
	controls[HEAD_WIDTH] = ModelerControl("Head width", 0.0, 2.0, 0.01f, 1.0);
	controls[HEAD_HEIGHT] = ModelerControl("Head height", 0.0, 2.0, 0.01f, 1.0);
	controls[HEAD_DEPTH] = ModelerControl("Head depth", 0.0, 2.0, 0.01f, 1.0);

	controls[LEFT_UPPER_ARM_ROTATION_X] = ModelerControl("Right arm rotation X", -180, 50, 0.01f, 0);
	controls[LEFT_UPPER_ARM_ROTATION_Y] = ModelerControl("Right arm rotation Y", -90, 90, 0.01f, 0);
	controls[LEFT_UPPER_ARM_ROTATION_Z] = ModelerControl("Right arm rotation Z", -180, 30, 0.01f, 0);
	controls[LEFT_LOWER_ARM_ROTATION_X] = ModelerControl("Right lower arm rotation X", -180, 0, 0.01f, 0);
	controls[RIGHT_UPPER_ARM_ROTATION_X] = ModelerControl("Left arm rotation X", -180, 50, 0.01f, 0);
	controls[RIGHT_UPPER_ARM_ROTATION_Y] = ModelerControl("Left arm rotation Y", -90, 90, 0.01f, 0);
	controls[RIGHT_UPPER_ARM_ROTATION_Z] = ModelerControl("Left arm rotation Z", -30, 180, 0.01f, 0);
	controls[RIGHT_LOWER_ARM_ROTATION_X] = ModelerControl("Left lower arm rotation X", -180, 0, 0.01f, 0);

	controls[LEFT_UPPER_LEG_ROTATION_X] = ModelerControl("Right leg rotation X", -120, 50, 0.01f, 0);
	controls[LEFT_UPPER_LEG_ROTATION_Y] = ModelerControl("Right leg rotation Y", -90, 90, 0.01f, 0);
	controls[LEFT_UPPER_LEG_ROTATION_Z] = ModelerControl("Right leg rotation Z", -180, 180, 0.01f, 0);
	controls[LEFT_LOWER_LEG_ROTATION_X] = ModelerControl("Right lower leg rotation X", 0, 120, 0.01f, 0);
	controls[LEFT_FOOT_ROTATION_X] = ModelerControl("Right foot rotation X", -30, 30, 0.01f, 0);
	controls[RIGHT_UPPER_LEG_ROTATION_X] = ModelerControl("Left leg rotation X", -120, 50, 0.01f, 0);
	controls[RIGHT_UPPER_LEG_ROTATION_Y] = ModelerControl("Left leg rotation Y", -90, 90, 0.01f, 0);
	controls[RIGHT_UPPER_LEG_ROTATION_Z] = ModelerControl("Left leg rotation Z", -180, 180, 0.01f, 0);
	controls[RIGHT_LOWER_LEG_ROTATION_X] = ModelerControl("Left lower leg rotation X", 0, 120, 0.01f, 0);
	controls[RIGHT_FOOT_ROTATION_X] = ModelerControl("Left foot rotation X", -30, 30, 0.01f, 0);

	controls[WAIST_ROTATION_X] = ModelerControl("Waist rotation X", -90, 90, 0.01f, 0);
	controls[WAIST_ROTATION_Y] = ModelerControl("Waist rotation Y", -90, 90, 0.01f, 0);
	controls[WAIST_ROTATION_Z] = ModelerControl("Waist rotation Z", -30, 30, 0.01f, 0);

	controls[DRAW_LEVEL] = ModelerControl("Level of details", 1, 3, 1, 3);
	controls[DRAW_TEXTURE] = ModelerControl("Draw texture", 0, 1, 1, 0);

	controls[DISPLAY_LSYSTEM] = ModelerControl("Display LSystem", 0, 1, 1, 0);
//	controls[LSYSTEM_TYPE] = ModelerControl("LSystem type", 1, LSystemVec->size(), 1, 1);
//	controls[LSYSTEM_ITER] = ModelerControl("LSystem Iterations", 0, 5, 1, 1);
//	controls[LS_LENGTH] = ModelerControl("LSystem Unit Length", 0, 5, 0.01f, 1);

	controls[HAVE_HAT] = ModelerControl("Have Hat", 0, 1, 1, 0);
	controls[MIKU_CLOTHES] = ModelerControl("Miku Clothes", 0, 1, 1, 1);
	controls[MIKU_SHOES] = ModelerControl("Miku Shoes", 0, 1, 1, 1);
	controls[MIKU_HAIR] = ModelerControl("Miku Hair", 0, 1, 1, 1);
	controls[HAPPINESS] = ModelerControl("Happiness", -2, 2, 1, 0);


	// You should create a ParticleSystem object ps here and then
	// call ModelerApplication::Instance()->SetParticleSystem(ps)
	// to hook it up to the animator interface.

	ParticleSystem *ps = new ParticleSystem(BALL, 20, Vec3f(0.05, 0.05, 0.05), 30.0, 5);
	ParticleSystem *fire = new ParticleSystem(BALL, 20, Vec3f(0.05, 0.05, 0.05), 30.0, 5, 1);

	ModelerApplication::Instance()->SetParticleSystem(ps);
	ModelerApplication::Instance()->SetFire(fire);
	ModelerApplication::Instance()->Init(&createRobotArm, controls, NUMCONTROLS);

    return ModelerApplication::Instance()->Run();
}
