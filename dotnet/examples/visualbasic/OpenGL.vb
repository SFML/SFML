Imports SFML
Imports SFML.Window
Imports SFML.Graphics
Imports Tao.OpenGl
Imports Tao.FreeGlut


Module OpenGL

    Dim WithEvents window As RenderWindow

    ''' <summary>
    ''' Entry point of application
    ''' </summary>
    Sub Main()

        ' Create main window
        window = New RenderWindow(New VideoMode(800, 600), "SFML.Net OpenGL (Visual Basic)")

        ' Create a sprite for the background
        Dim backgroundImage = New Image("resources/background.jpg")
        Dim background = New Sprite(backgroundImage)

        ' Create a text to display
        Dim text = New Text("SFML / OpenGL demo")
        text.Position = New Vector2(250.0F, 450.0F)
        text.Color = New Color(255, 255, 255, 170)

        ' Load an OpenGL texture.
        ' We could directly use a SFML.Graphics.Image as an OpenGL texture (with its Bind() member function),
        ' but here we want more control on it (generate mipmaps, ...) so we create a new one
        Dim texture = 0
        Using tempImage = New Image("resources/texture.jpg")
            Gl.glGenTextures(1, texture)
            Gl.glBindTexture(Gl.GL_TEXTURE_2D, texture)
            Glu.gluBuild2DMipmaps(Gl.GL_TEXTURE_2D, Gl.GL_RGBA, tempImage.Width, tempImage.Height, Gl.GL_RGBA, Gl.GL_UNSIGNED_BYTE, tempImage.Pixels)
            Gl.glTexParameteri(Gl.GL_TEXTURE_2D, Gl.GL_TEXTURE_MAG_FILTER, Gl.GL_LINEAR)
            Gl.glTexParameteri(Gl.GL_TEXTURE_2D, Gl.GL_TEXTURE_MIN_FILTER, Gl.GL_LINEAR_MIPMAP_LINEAR)
        End Using

        ' Enable Z-buffer read and write
        Gl.glEnable(Gl.GL_DEPTH_TEST)
        Gl.glDepthMask(Gl.GL_TRUE)
        Gl.glClearDepth(1.0F)

        ' Setup a perspective projection
        Gl.glMatrixMode(Gl.GL_PROJECTION)
        Gl.glLoadIdentity()
        Glu.gluPerspective(90.0F, 1.0F, 1.0F, 500.0F)

        ' Bind our texture
        Gl.glEnable(Gl.GL_TEXTURE_2D)
        Gl.glBindTexture(Gl.GL_TEXTURE_2D, texture)
        Gl.glColor4f(1.0F, 1.0F, 1.0F, 1.0F)

        Dim time = 0.0F

        ' Start game loop
        While (window.IsOpened())

            ' Process events
            window.DispatchEvents()

            ' Draw background
            window.SaveGLStates()
            window.Draw(background)
            window.RestoreGLStates()

            ' Clear depth buffer
            Gl.glClear(Gl.GL_DEPTH_BUFFER_BIT)

            ' We get the position of the mouse cursor, so that we can move the box accordingly
            Dim x = window.Input.GetMouseX() * 200.0F / window.Width - 100.0F
            Dim y = -window.Input.GetMouseY() * 200.0F / window.Height + 100.0F

            ' Apply some transformations
            time += window.GetFrameTime()
            Gl.glMatrixMode(Gl.GL_MODELVIEW)
            Gl.glLoadIdentity()
            Gl.glTranslatef(x, y, -100.0F)
            Gl.glRotatef(time * 50, 1.0F, 0.0F, 0.0F)
            Gl.glRotatef(time * 30, 0.0F, 1.0F, 0.0F)
            Gl.glRotatef(time * 90, 0.0F, 0.0F, 1.0F)

            ' Draw a cube
            Dim size = 20.0F
            Gl.glBegin(Gl.GL_QUADS)

            Gl.glTexCoord2f(0, 0) : Gl.glVertex3f(-size, -size, -size)
            Gl.glTexCoord2f(0, 1) : Gl.glVertex3f(-size, size, -size)
            Gl.glTexCoord2f(1, 1) : Gl.glVertex3f(size, size, -size)
            Gl.glTexCoord2f(1, 0) : Gl.glVertex3f(size, -size, -size)

            Gl.glTexCoord2f(0, 0) : Gl.glVertex3f(-size, -size, size)
            Gl.glTexCoord2f(0, 1) : Gl.glVertex3f(-size, size, size)
            Gl.glTexCoord2f(1, 1) : Gl.glVertex3f(size, size, size)
            Gl.glTexCoord2f(1, 0) : Gl.glVertex3f(size, -size, size)

            Gl.glTexCoord2f(0, 0) : Gl.glVertex3f(-size, -size, -size)
            Gl.glTexCoord2f(0, 1) : Gl.glVertex3f(-size, size, -size)
            Gl.glTexCoord2f(1, 1) : Gl.glVertex3f(-size, size, size)
            Gl.glTexCoord2f(1, 0) : Gl.glVertex3f(-size, -size, size)

            Gl.glTexCoord2f(0, 0) : Gl.glVertex3f(size, -size, -size)
            Gl.glTexCoord2f(0, 1) : Gl.glVertex3f(size, size, -size)
            Gl.glTexCoord2f(1, 1) : Gl.glVertex3f(size, size, size)
            Gl.glTexCoord2f(1, 0) : Gl.glVertex3f(size, -size, size)

            Gl.glTexCoord2f(0, 1) : Gl.glVertex3f(-size, -size, size)
            Gl.glTexCoord2f(0, 0) : Gl.glVertex3f(-size, -size, -size)
            Gl.glTexCoord2f(1, 0) : Gl.glVertex3f(size, -size, -size)
            Gl.glTexCoord2f(1, 1) : Gl.glVertex3f(size, -size, size)

            Gl.glTexCoord2f(0, 1) : Gl.glVertex3f(-size, size, size)
            Gl.glTexCoord2f(0, 0) : Gl.glVertex3f(-size, size, -size)
            Gl.glTexCoord2f(1, 0) : Gl.glVertex3f(size, size, -size)
            Gl.glTexCoord2f(1, 1) : Gl.glVertex3f(size, size, size)

            Gl.glEnd()

            ' Draw some text on top of our OpenGL object
            window.SaveGLStates()
            window.Draw(text)
            window.RestoreGLStates()

            ' Finally, display the rendered frame on screen
            window.Display()

        End While

        ' Don't forget to destroy our texture
        Gl.glDeleteTextures(1, texture)

    End Sub

    ''' <summary>
    ''' Function called when the window is closed
    ''' </summary>
    Sub App_Closed(ByVal sender As Object, ByVal e As EventArgs) Handles window.Closed
        Dim window = CType(sender, RenderWindow)
        window.Close()
    End Sub

    ''' <summary>
    ''' Function called when a key is pressed
    ''' </summary>
    Sub App_KeyPressed(ByVal sender As Object, ByVal e As KeyEventArgs) Handles window.KeyPressed
        Dim window = CType(sender, RenderWindow)
        If e.Code = KeyCode.Escape Then
            window.Close()
        End If
    End Sub

    ''' <summary>
    ''' Function called when the window is resized
    ''' </summary>
    Sub App_Resized(ByVal sender As Object, ByVal e As SizeEventArgs) Handles window.Resized
        Gl.glViewport(0, 0, e.Width, e.Height)
    End Sub

End Module
