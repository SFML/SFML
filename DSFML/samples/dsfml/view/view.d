module view;

import dsfml.system.all;
import dsfml.window.all;
import dsfml.graphics.all;

void main()
{
	RenderWindow window = new RenderWindow(VideoMode(800, 600), "View sample");
	window.setFramerateLimit(100);
	Input input = window.getInput();
	Vector2f top;
	Rect!(float) bound;
	Shape s;
	bool mousePressed;
	
	Sprite background = new Sprite(new Image("Data/background.jpg"));
	
	Font f = new Font("Data/cheeseburger.ttf");
	Text str = new Text("Create a selection of the background with your mouse.\nPress Enter to zoom to this selection.\nPress Escape to return to the default view."c, f);
	
	while (window.isOpened())
	{
		Event evt;
		
		while (window.getEvent(evt))
		{
			if (		evt.Type == EventType.MouseButtonPressed &&
						evt.MouseButton.Button == MouseButtons.Left)
			{
				top = window.convertCoords(input.getMouseX(), input.getMouseY());
				mousePressed = true;
				
			}
			else if (	evt.Type == EventType.MouseButtonReleased &&
						evt.MouseButton.Button == MouseButtons.Left)
			{
				mousePressed = false;	
			}
			else if (	evt.Type == EventType.MouseMoved &&
						mousePressed)
			{
				Vector2f bottom = window.convertCoords(input.getMouseX(), input.getMouseY());
				bound = FloatRect(top.x, top.y, bottom.x, bottom.y);
				s = Shape.rectangle(top.x, top.y, bottom.x, bottom.y, Color(0, 0, 0, 0), 1, Color.BLACK);
			}
			else if (	evt.Type == EventType.KeyPressed &&
						evt.Key.Code == KeyCode.Return)
			{
				if (bound != FloatRect())
					window.setView(new View(bound));
				s = null;
			}
			else if (	evt.Type == EventType.KeyPressed &&
						evt.Key.Code == KeyCode.Escape)
			{
				window.setView(window.getDefaultView());	
			}
			else if (	evt.Type == EventType.Closed)
				window.close();
			
		}
		
		window.draw(background);
		window.draw(str);
		if (s !is null) window.draw(s);
		window.display();
	}
}
