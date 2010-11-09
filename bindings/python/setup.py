#!/usr/bin/env python
# coding=UTF-8

from distutils.core import setup, Extension

setup(name='PySFML',
	version='1.6',
	description='Python binding for SFML (Simple and Fast Multimedia Library)',
	author='Rémi Koenig',
	author_email='remi.k2620@gmail.com',
	url='http://www.sfml-dev.org/',
	license='zlib/png',
	ext_modules=[ Extension('PySFML.sf',
		['src/Clock.cpp', 'src/Color.cpp', 'src/Drawable.cpp',
		'src/Event.cpp', 'src/Image.cpp', 'src/Input.cpp', 'src/Key.cpp', 'src/main.cpp', 'src/Music.cpp',
		'src/Shader.cpp', 'src/Rect.cpp', 'src/RenderWindow.cpp', 'src/Sleep.cpp',
		'src/Sprite.cpp', 'src/Text.cpp', 'src/VideoMode.cpp', 'src/View.cpp', 'src/Window.cpp',
		'src/Joy.cpp', 'src/Mouse.cpp', 'src/WindowStyle.cpp', 'src/Blend.cpp', 'src/Sound.cpp',
		'src/SoundBuffer.cpp', 'src/Listener.cpp', 'src/SoundRecorder.cpp', 'src/SoundBufferRecorder.cpp',
		'src/SoundStream.cpp', 'src/Font.cpp', 'src/Glyph.cpp', 'src/Shape.cpp', 'src/ContextSettings.cpp'],
		libraries=['sfml-graphics', 'sfml-window', 'sfml-audio', 'sfml-system'],
		library_dirs=['../lib/mingw'],
		include_dirs=['../include']
		)],
	package_dir = {'PySFML':'PySFML'},
	packages=['PySFML'],
	)
