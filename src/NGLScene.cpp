#include "NGLScene.h"
#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <QColorDialog>


//----------------------------------------------------------------------------------------------------------------------
NGLScene::NGLScene( QWidget *_parent ) : QOpenGLWidget( _parent )
{

  // set this widget to have the initial keyboard focus
  setFocus();
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize(_parent->size());
	m_wireframe=false;
	m_rotation=0.0;
	m_scale=1.0;
	m_position=0.0;

	m_selectedObject=0;
}

// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining display lists, etc.
constexpr auto shaderProgram="PBR";
//----------------------------------------------------------------------------------------------------------------------
void NGLScene::initializeGL()
{

  ngl::NGLInit::initialize();
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  /// create our camera
  ngl::Vec3 eye(0.0f,2.0f,2.0f);
  ngl::Vec3 look(0,0,0);
  ngl::Vec3 up(0,1,0);

  m_view=ngl::lookAt(eye,look,up);
  m_project=ngl::perspective(45,float(1024/720),0.1f,300.0f);
  // we are creating a shader called PBR to save typos
  // in the code create some constexpr
  constexpr auto vertexShader  = "PBRVertex";
  constexpr auto fragShader    = "PBRFragment";
  // create the shader program
  ngl::ShaderLib::createShaderProgram( shaderProgram );
  // now we are going to create empty shaders for Frag and Vert
  ngl::ShaderLib::attachShader( vertexShader, ngl::ShaderType::VERTEX );
  ngl::ShaderLib::attachShader( fragShader, ngl::ShaderType::FRAGMENT );
  // attach the source
  ngl::ShaderLib::loadShaderSource( vertexShader, "shaders/PBRVertex.glsl" );
  ngl::ShaderLib::loadShaderSource( fragShader, "shaders/PBRFragment.glsl" );
  // compile the shaders
  ngl::ShaderLib::compileShader( vertexShader );
  ngl::ShaderLib::compileShader( fragShader );
  // add them to the program
  ngl::ShaderLib::attachShaderToProgram( shaderProgram, vertexShader );
  ngl::ShaderLib::attachShaderToProgram( shaderProgram, fragShader );
  // now we have associated that data we can link the shader
  ngl::ShaderLib::linkProgramObject( shaderProgram );
  // and make it active ready to load values
  ngl::ShaderLib::use( shaderProgram );
  ngl::ShaderLib::setUniform( "camPos", eye );
  // now a light
  // setup the default shader material and light porerties
  // these are "uniform" so will retain their values
  ngl::ShaderLib::setUniform("lightPosition",0.0, 2.0f, 2.0f );
  ngl::ShaderLib::setUniform("lightColor",400.0f,400.0f,400.0f);
  ngl::ShaderLib::setUniform("exposure",2.2f);
  ngl::ShaderLib::setUniform("albedo",0.950f, 0.71f, 0.29f);
  ngl::ShaderLib::setUniform("metallic",1.02f);
  ngl::ShaderLib::setUniform("roughness",0.38f);
  ngl::ShaderLib::setUniform("ao",0.2f);
  ngl::VAOPrimitives::createSphere("sphere",1.0,40);
  m_text=std::make_unique<ngl::Text>("fonts/Arial.ttf",18);
  m_text->setScreenSize(this->size().width(),this->size().height());
  m_text->setColour(1.0,1.0,0.0);
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
void NGLScene::resizeGL( int _w, int _h )
{
  m_project=ngl::perspective( 45.0f, static_cast<float>( _w ) / _h, 0.05f, 350.0f );
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
}


void NGLScene::loadMatricesToShader()
{
   ngl::ShaderLib::use("PBR");
   struct transform
   {
     ngl::Mat4 MVP;
     ngl::Mat4 normalMatrix;
     ngl::Mat4 M;
   };

    transform t;
    t.M=m_transform.getMatrix();

    t.MVP=m_project*m_view*t.M;
    t.normalMatrix=t.M;
    t.normalMatrix.inverse().transpose();
    ngl::ShaderLib::setUniformBuffer("TransformUBO",sizeof(transform),&t.MVP.m_00);
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
void NGLScene::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);
  if(m_wireframe == true)
  {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  }
  else
  {
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }

	m_transform.setPosition(m_position);
	m_transform.setScale(m_scale);
	m_transform.setRotation(m_rotation);
	loadMatricesToShader();
	switch(m_selectedObject)
	{
		case 0 : ngl::VAOPrimitives::draw("teapot"); break;
		case 1 : ngl::VAOPrimitives::draw("sphere"); break;
		case 2 : ngl::VAOPrimitives::draw("cube"); break;
	}
	m_text->renderText(10,580,"Qt Gui Demo");
}








NGLScene::~NGLScene()
{
}

void NGLScene::toggleWireframe(bool _mode	 )
{
	m_wireframe=_mode;
	update();
}

void NGLScene::setXRotation( double _x		)
{
	m_rotation.m_x=_x;
	update();
}

void NGLScene::setYRotation( double _y	)
{
	m_rotation.m_y=_y;
	update();
}
void NGLScene::setZRotation( double _z )
{
	m_rotation.m_z=_z;
	update();
}

void NGLScene::setXScale( double _x	)
{
	m_scale.m_x=_x;
	update();
}

void NGLScene::setYScale(	 double _y)
{
	m_scale.m_y=_y;
	update();
}
void NGLScene::setZScale( double _z )
{
	m_scale.m_z=_z;
	update();
}

void NGLScene::setXPosition( double _x	)
{
	m_position.m_x=_x;
	update();
}

void NGLScene::setYPosition( double _y	)
{
	m_position.m_y=_y;
	update();
}
void NGLScene::setZPosition( double _z	 )
{
	m_position.m_z=_z;
	update();
}

void NGLScene::setObjectMode(	int _i)
{
	m_selectedObject=_i;
	update();
}
void NGLScene::setColour()
{
	QColor colour = QColorDialog::getColor();
	if( colour.isValid())
	{
    
    ngl::ShaderLib::use(shaderProgram);
    ngl::ShaderLib::setUniform("albedo",static_cast<float>(colour.redF()),static_cast<float>(colour.greenF()),static_cast<float>(colour.blueF()));
    update();
	}
}
