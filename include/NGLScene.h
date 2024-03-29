#ifndef NGLSCENE_H_
#define NGLSCENE_H_

#include <ngl/Transformation.h>
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/Text.h>
#include "WindowParams.h"
#include <QEvent>
#include <QResizeEvent>
#include <QOpenGLWidget>
#include <memory>


/// @file NGLScene.h
/// @brief a basic Qt GL window class for ngl demos
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/10/10
/// Revision History :
/// Initial Version 10/10/10 (Binary day ;-0 )
/// @class GLWindow
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
class NGLScene : public QOpenGLWidget
{
Q_OBJECT        // must include this if you use Qt signals/slots
public :
  /// @brief Constructor for GLWindow
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor for GLWindow
  /// @param [in] _parent the parent window to create the GL context in
  //----------------------------------------------------------------------------------------------------------------------
  NGLScene(QWidget *_parent );

		/// @brief dtor
  ~NGLScene() override;
 public slots :
	/// @brief a slot to toggle wireframe mode
	/// @param[in] _mode the mode passed from the toggle
	/// button
	void toggleWireframe( bool _mode	 );
	/// @brief set the X rotation value
	/// @parm[in] _x the value to set
	void setXRotation( double _x	);
	/// @brief set the Y rotation value
	/// @parm[in] _y the value to set
	void setYRotation( double _y	);
	/// @brief set the Z rotation value
	/// @parm[in] _z the value to set
	void setZRotation( double _z	);
	/// @brief set the X scale value
	/// @parm[in] _x the value to set
	void setXScale( double _x	);
	/// @brief set the Y scale value
	/// @parm[in] _y the value to set
	void setYScale( double _y	);
	/// @brief set the Z scale value
	/// @parm[in] _z the value to set
	void setZScale( double _z	);

	/// @brief set the X position value
	/// @parm[in] _x the value to set
	void setXPosition( double _x	);
	/// @brief set the Y position value
	/// @parm[in] _y the value to set
	void setYPosition(double _y);
	/// @brief set the Z position value
	/// @parm[in] _z the value to set
	void setZPosition(double _z	);
	/// @brief set the draw object
	/// @param[in] _i the index of the object
	void setObjectMode(int _i);
	/// @brief a slot to set the colour
	void setColour();
private :
	/// @brief m_wireframe mode
	bool m_wireframe=false;
	/// @brief rotation data
  ngl::Vec3 m_rotation={0.0f,0.0f,0.0f};
	/// @brief scale data
  ngl::Vec3 m_scale={1.0f,1.0f,1.0f};
	/// @brief position data
  ngl::Vec3 m_position={0.0f,0.0f,0.0f};
	/// @brief our object to draw
	int m_selectedObject=0;
	//----------------------------------------------------------------------------------------------------------------------
	// text for rendering
	//----------------------------------------------------------------------------------------------------------------------
	std::unique_ptr<ngl::Text> m_text;

protected:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the windows params such as mouse and rotations etc
  //----------------------------------------------------------------------------------------------------------------------
  WinParams m_win;
  /// @brief  The following methods must be implimented in the sub class
  /// this is called when the window is created
  void initializeGL() override;

  /// @brief this is called whenever the window is re-sized
  /// @param[in] _w the width of the resized window
  /// @param[in] _h the height of the resized window
  void resizeGL(int _w , int _h) override;
  /// @brief this is the main gl drawing routine which is called whenever the window needs to
  // be re-drawn
  void paintGL() override;

  /// @brief our model position
  ngl::Vec3 m_modelPos;
  /// @brief our camera
  ngl::Mat4 m_view;
  ngl::Mat4 m_project;
	/// @brief our transform for objects
	ngl::Transformation m_transform;
private :
  /// @brief this method is called every time a mouse is moved
  /// @param _event the Qt Event structure

  void mouseMoveEvent (QMouseEvent * _event   ) override;
  /// @brief this method is called everytime the mouse button is pressed
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure

  void mousePressEvent ( QMouseEvent *_event  ) override;

  /// @brief this method is called everytime the mouse button is released
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  void mouseReleaseEvent (QMouseEvent *_event ) override;
  void wheelEvent( QWheelEvent* _event ) override;

  void loadMatricesToShader( );

};

#endif
