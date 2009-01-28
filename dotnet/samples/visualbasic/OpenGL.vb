Imports SFML
Imports SFML.Window
Imports SFML.Graphics
Imports Tao.OpenGl
Imports Tao.FreeGlut


Module OpenGL

    Dim WithEvents App As RenderWindow

    ''' <summary>
    ''' Entry point of application
    ''' </summary>
    Sub Main()

        ' Create main window
        App = New RenderWindow(New VideoMode(800, 600), "SFML.Net OpenGL (Visual Basic)")
        App.PreserveOpenGLStates(True)

        ' Create a sprite for the background
        Dim BackgroundImage = New Image("datas/opengl/background.jpg")
        Dim Background = New Sprite(BackgroundImage)

        ' Create a text to display
        Dim Text = New String2D("This is a rotating cube")
        Text.Position = New Vector2(250.0F, 300.0F)
        Text.Color = New Color(128, 0, 128)

        ' Load an OpenGL texture.
        ' We could directly use a sf::Image as an OpenGL texture (with its Bind() member function),
        ' but here we want more control on it (generate mipmaps, ...) so we create a new one
        Dim Texture = 0
        Using TempImage = New Image("datas/opengl/texture.jpg")
            Gl.glGenTextures(1, Texture)
            Gl.glBindTexture(Gl.GL_TEXTURE_2D, Texture)
            Glu.gluBuild2DMipmaps(Gl.GL_TEXTURE_2D, Gl.GL_RGBA, TempImage.Width, TempImage.Height, Gl.GL_RGBA, Gl.GL_UNSIGNED_BYTE, TempImage.Pixels)
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
        Gl.glBindTexture(Gl.GL_TEXTURE_2D, Texture)
        Gl.glColor4f(1.0F, 1.0F, 1.0F, 1.0F)

        Dim Time = 0.0F

        ' Start game loop
        While (App.IsOpened())

            ' Process events
            App.DispatchEvents()

            ' Draw background
            App.Draw(Background)

            ' Clear depth buffer
            Gl.glClear(Gl.GL_DEPTH_BUFFER_BIT)

            ' Apply some transformations
            Time += App.GetFrameTime()
            Gl.glMatrixMode(Gl.GL_MODELVIEW)
            Gl.glLoadIdentity()
            Gl.glTranslatef(0.0F, 0.0F, -200.0F)
            Gl.glRotatef(Time * 50, 1.0F, 0.0F, 0.0F)
            Gl.glRotatef(Time * 30, 0.0F, 1.0F, 0.0F)
            Gl.glRotatef(Time * 90, 0.0F, 0.0F, 1.0F)

            ' Draw a cube
            Gl.glBegin(Gl.GL_QUADS)

            Gl.glTexCoord2f(0, 0)
            Gl.glVertex3f(-50.0F, -50.0F, -50.0F)
            Gl.glTexCoord2f(0, 1)
            Gl.glVertex3f(-50.0F, 50.0F, -50.0F)
            Gl.glTexCoord2f(1, 1)
            Gl.glVertex3f(50.0F, 50.0F, -50.0F)
            Gl.glTexCoord2f(1, 0)
            Gl.glVertex3f(50.0F, -50.0F, -50.0F)

            Gl.glTexCoord2f(0, 0)
            Gl.glVertex3f(-50.0F, -50.0F, 50.0F)
            Gl.glTexCoord2f(0, 1)
            Gl.glVertex3f(-50.0F, 50.0F, 50.0F)
            Gl.glTexCoord2f(1, 1)
            Gl.glVertex3f(50.0F, 50.0F, 50.0F)
            Gl.glTexCoord2f(1, 0)
            Gl.glVertex3f(50.0F, -50.0F, 50.0F)

            Gl.glTexCoord2f(0, 0)
            Gl.glVertex3f(-50.0F, -50.0F, -50.0F)
            Gl.glTexCoord2f(0, 1)
            Gl.glVertex3f(-50.0F, 50.0F, -50.0F)
            Gl.glTexCoord2f(1, 1)
            Gl.glVertex3f(-50.0F, 50.0F, 50.0F)
            Gl.glTexCoord2f(1, 0)
            Gl.glVertex3f(-50.0F, -50.0F, 50.0F)

            Gl.glTexCoord2f(0, 0)
            Gl.glVertex3f(50.0F, -50.0F, -50.0F)
            Gl.glTexCoord2f(0, 1)
            Gl.glVertex3f(50.0F, 50.0F, -50.0F)
            Gl.glTexCoord2f(1, 1)
            Gl.glVertex3f(50.0F, 50.0F, 50.0F)
            Gl.glTexCoord2f(1, 0)
            Gl.glVertex3f(50.0F, -50.0F, 50.0F)

            Gl.glTexCoord2f(0, 1)
            Gl.glVertex3f(-50.0F, -50.0F, 50.0F)
            Gl.glTexCoord2f(0, 0)
            Gl.glVertex3f(-50.0F, -50.0F, -50.0F)
            Gl.glTexCoord2f(1, 0)
            Gl.glVertex3f(50.0F, -50.0F, -50.0F)
            Gl.glTexCoord2f(1, 1)
            Gl.glVertex3f(50.0F, -50.0F, 50.0F)

            Gl.glTexCoord2f(0, 1)
            Gl.glVertex3f(-50.0F, 50.0F, 50.0F)
            Gl.glTexCoord2f(0, 0)
            Gl.glVertex3f(-50.0F, 50.0F, -50.0F)
            Gl.glTexCoord2f(1, 0)
            Gl.glVertex3f(50.0F, 50.0F, -50.0F)
            Gl.glTexCoord2f(1, 1)
            Gl.glVertex3f(50.0F, 50.0F, 50.0F)

            Gl.glEnd()

            ' Draw some text on top of our OpenGL object
            App.Draw(Text)

            ' Finally, display the rendered frame on screen
            App.Display()

        End While

        ' Don't forget to destroy our texture
        Gl.glDeleteTextures(1, Texture)

    End Sub

    ''' <summary>
    ''' Function called when the window is closed
    ''' </summary>
    Sub App_Closed(ByVal sender As Object, ByVal e As EventArgs) Handles App.Closed
        Dim window = CType(sender, RenderWindow)
        window.Close()
    End Sub

    ''' <summary>
    ''' Function called when a key is pressed
    ''' </summary>
    Sub App_KeyPressed(ByVal sender As Object, ByVal e As KeyEventArgs) Handles App.KeyPressed
        Dim window = CType(sender, RenderWindow)
        If e.Code = KeyCode.Escape Then
            window.Close()
        End If
    End Sub

    ''' <summary>
    ''' Function called when the window is resized
    ''' </summary>
    Sub App_Resized(ByVal sender As Object, ByVal e As SizeEventArgs) Handles App.Resized
        Gl.glViewport(0, 0, e.Width, e.Height)
    End Sub

End Module
