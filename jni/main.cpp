#include "Window.hpp"
#include "Program.hpp"
#include "Matrix4x4.hpp"

XPG::Matrix4x4<GLfloat> projection;
GLint matrixUniform;

static const char* const VertexShader =
	"uniform mat4 uMatrix;\n"
    "attribute vec4 vPosition;\n"
	"attribute vec4 aColor;\n"
	"varying vec4 vColor;\n"
    "void main()\n"
	"{\n"
	"  vColor = aColor;\n"
    "  gl_Position = uMatrix * vec4(vPosition.xy, 0.0, 1.0);\n"
    "}\n";

static const char* const FragmentShader =
    "precision mediump float;\n"
	"varying vec4 vColor;\n"
    "void main()\n"
	"{\n"
    "  gl_FragColor = vColor;\n"
    "}\n";

static const GLfloat TriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
        0.5f, -0.5f };

static const GLfloat TriangleColors[] = { 1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

static XPG::Program<2>* program = 0;

void OnLoad(const XPG::Ammo& ammo)
{
	LOGI("OnLoad");
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

	XPG::Shader vs(GL_VERTEX_SHADER);
	XPG::Shader fs(GL_FRAGMENT_SHADER);

	vs.LoadFromBuffer(VertexShader);
	fs.LoadFromBuffer(FragmentShader);

	program = new XPG::Program<2>;
	program->AttachShader(vs.Handle());
	program->AttachShader(fs.Handle());
	program->BindAttribLocation(0, "vPosition");
	program->BindAttribLocation(1, "aColor");
	program->Link();

	matrixUniform = program->GetUniformLocation("uMatrix");

	float ratio = float(ammo.x) / float(ammo.y);
	projection.loadIdentity();
	projection.orthographic(1.0f, ratio);

	glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, projection);
}

void OnUnload(const XPG::Ammo& ammo)
{
	LOGI("OnUnload");
	delete program;
	program = 0;
}

void OnDraw(const XPG::Ammo& ammo)
{
	glClear(GL_COLOR_BUFFER_BIT);
	program->Use();
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, TriangleVertices);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, TriangleColors);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	ammo.window->SwapBuffers();
}

void OnTouch(const XPG::Ammo& ammo)
{
	glClearColor(((float)ammo.x)/ ammo.window->Width(), (float)ammo.window->Angle(),
			((float)ammo.y)/ammo.window->Height(), 1);
}

void OnAccel(const XPG::Ammo& ammo)
{

}

void OnLowMemory(const XPG::Ammo& ammo)
{

}

void OnResize(const XPG::Ammo& ammo)
{

}

void android_main(struct android_app* state)
{
	XPG::Window window(state);
	window.OnLoad().Aim(OnLoad);
	window.OnUnload().Aim(OnUnload);
	window.OnDraw().Aim(OnDraw);
	window.OnTouch().Aim(OnTouch);
	window.OnAccel().Aim(OnAccel);
	window.OnResize().Aim(OnResize);
	window.Run();
}
