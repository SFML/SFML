module dflsample;

import dsfml.system.all;
import dsfml.window.all;

// DFL and Derelict must be present.
import dfl.all;

import Derelict.opengl.gl;
import Derelict.opengl.glu;


// An enum for each controls methods
enum ControlMethod
{
	MOUSE,
	KEYBOARD
}

void main()
{
	DerelictGL.load();
	DerelictGLU.load();
	//Start the message loop
	Application.run(new MyForm());
}

//A simple form with a groupbox to choose input method and the openGL control
class MyForm : Form
{
	GLControl m_gl;
	GroupBox m_gbx;
	RadioButton m_rb1;
	RadioButton m_rb2;
	
	this()
	{
		m_gbx = new GroupBox();
		m_gbx.dock = DockStyle.TOP;
		m_gbx.height = 40;
		m_gbx.text = "Choose your input";
		this.controls.add(m_gbx);
		
		m_rb1 = new RadioButton();
		m_rb1.text = "Mouse";
		m_rb1.location = Point(10, 15);
		m_rb1.checked = true;
		m_rb1.click ~= &radioButtonClick;
		m_gbx.controls.add(m_rb1);
		
		m_rb2 = new RadioButton();
		m_rb2.text = "Keyboard";
		m_rb2.location = Point(m_rb1.width + 10, 15);
		m_rb2.click ~= &radioButtonClick;
		m_gbx.controls.add(m_rb2);
		
		m_gl = new GLControl();
		m_gl.dock = DockStyle.FILL;
		m_gl.controlMethod = ControlMethod.MOUSE;
		this.controls.add(m_gl);
		
		this.text = "DFL Opengl Integration Sample";
	}
	
	private void radioButtonClick(Control c, EventArgs ea)
	{
		m_gl.controlMethod(c.text == "Mouse" ? ControlMethod.MOUSE : ControlMethod.KEYBOARD);
		m_gl.focus();
	}
 
}

//Our OpenGL control
class GLControl : Control
{
	Window m_win;
	Input i;
	Timer m_timer;
	GLfloat rotx = 0.f, roty = 0.f;
	ControlMethod m_method = ControlMethod.MOUSE;
	
	this ()
	{
		super();
		this.setStyle(ControlStyles.SELECTABLE | ControlStyles.ALL_PAINTING_IN_WM_PAINT | ControlStyles.WANT_ALL_KEYS, true);
		
		//We set a timer to ensure periodic refresh of the window
		m_timer = new Timer();
		m_timer.interval(10);
		m_timer.tick ~= &this.onTick;
			
	}
	
	public void controlMethod(ControlMethod m)
	{
		m_method = m;
	}
	
	//Override of the onHandleCreated method of Control
	//integration of DSFML window in a control need a valid handle 
	protected void onHandleCreated(EventArgs ea)
	{
		super.onHandleCreated(ea);
		
		//Construction of the window
		m_win = new Window(cast(WindowHandle)this.handle);
		
		//Get the input for further use
		i = m_win.getInput();
		
		//Now that the Window is instanciated, we can start the timer.
		m_timer.start();
		
		//Some opengl initializations functions
		glClearDepth(1.f);
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90.f, 1.f, 1.f, 500.f);
	}

	//We handle Mouse leaving and entering the control to hide or show the cursor
	protected void onMouseEnter(MouseEventArgs mea)
	{
		super.onMouseEnter(mea);
		Cursor.current.hide();
	}
	
	protected void onMouseLeave(MouseEventArgs mea)
	{
		super.onMouseLeave(mea);
		Cursor.current.show();
	}

	//If the window is resize, we need to modify openGL view
	protected void onResize(EventArgs ea)
	{
		super.onResize(ea);
		glViewport(0, 0, this.width, this.height);
	}

	protected void onTick(Timer t, EventArgs ea)
	{
		draw();
	}

	protected void onPaint(PaintEventArgs pea)
	{
		super.onPaint(pea);
		draw();
	}

	private void handleKeys()
	{
		if (i.isKeyDown(KeyCode.UP))
		 rotx += 1.f;
		if (i.isKeyDown(KeyCode.DOWN))
		 rotx += -1.f;
		if (i.isKeyDown(KeyCode.LEFT))
		 roty += -1.f;
		if (i.isKeyDown(KeyCode.RIGHT))
		 roty += 1.f;

	}
	private void handleMousePos()
	{
		rotx = i.getMouseY() - (this.height / 2.0);
		roty = i.getMouseX() - (this.width / 2.0);
	}
	
	private void draw()
	{
		if (m_method == ControlMethod.KEYBOARD)
			handleKeys();
		else
			handleMousePos();

		m_win.setActive(true);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.f, 0.f, -200.f);
		glRotatef(rotx, 1.f, 0.f, 0.f);
		glRotatef(roty, 0.f, 1.f, 0.f);

		glBegin(GL_QUADS);
			glColor3f(1.f, 0.f, 0.f);
			glVertex3f(-50.f, -50.f, -50.f);
			glVertex3f(-50.f,  50.f, -50.f);
			glVertex3f( 50.f,  50.f, -50.f);
			glVertex3f( 50.f, -50.f, -50.f);
			
			glColor3f(0.f, 1.f, 0.f);
			glVertex3f(-50.f, -50.f, 50.f);
			glVertex3f(-50.f,  50.f, 50.f);
			glVertex3f( 50.f,  50.f, 50.f);
			glVertex3f( 50.f, -50.f, 50.f);
		
			glColor3f(0.f, 0.f, 1.f);
			glVertex3f(-50.f, -50.f, -50.f);
			glVertex3f(-50.f,  50.f, -50.f);
			glVertex3f(-50.f,  50.f,  50.f);
			glVertex3f(-50.f, -50.f,  50.f);
		
			glColor3f(1.f, 1.f, 0.f);
			glVertex3f(50.f, -50.f, -50.f);
			glVertex3f(50.f,  50.f, -50.f);
			glVertex3f(50.f,  50.f,  50.f);
			glVertex3f(50.f, -50.f,  50.f);
		
			glColor3f(1.f, 0.f, 1.f);
			glVertex3f(-50.f, -50.f,  50.f);
			glVertex3f(-50.f, -50.f, -50.f);
			glVertex3f( 50.f, -50.f, -50.f);
			glVertex3f( 50.f, -50.f,  50.f);
		
			glColor3f(0.f, 1.f, 1.f);
			glVertex3f(-50.f, 50.f,  50.f);
			glVertex3f(-50.f, 50.f, -50.f);
			glVertex3f( 50.f, 50.f, -50.f);
			glVertex3f( 50.f, 50.f,  50.f);		
		glEnd();
				
		m_win.display();
	}
}
