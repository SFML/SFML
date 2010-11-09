module util;

const ubyte AudioData	= 1;
const ubyte EndOfStream = 2;

public import dsfml.system.all;
public import dsfml.audio.all;
public import dsfml.network.all;

version(Tango)
{
	public import tango.io.Console;
	public import tango.text.convert.Integer;
}
else
{
	public import std.stdio;
	
	//simple abstraction of Cout & Cin for phobos	
	class Cout
	{
		static Cout s_c;
		static this()
		{
			s_c = new Cout();
		}
		
		static Cout opCall(char[] str)
		{
			writefln("%s", str);
			return s_c;
		}
		void newline()
		{
			
		}
	}

	class Cin
	{
		static char[] get()
		{
			return readln();
		}
	}

	public import std.string : atoi;
	alias atoi parse;
}
