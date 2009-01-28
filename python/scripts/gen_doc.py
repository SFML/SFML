#!/usr/bin/python
# coding=UTF-8

from PySFML import sf

# Amélioration à faire : trouver les méthodes héritées, et de quelle classe

def function_str(function, class_name=None):
	string = ''
	name = function.__name__
	doc = function.__doc__
	if not doc.startswith(name+'('):
		string += name+"(...)"+'\n'
	string += doc+'\n'
	strings = string.split('\n')
	strings[0] = '<div class="attr_name">'+strings[0]+'</div>\n<div class="desc">'
	string = strings[0]
	for s in strings[1:-1]:
		string += s + '<br />'
	string += strings[-1]
	inherited = ''
	if class_name != None:
		try:
			base_class_name = function.__objclass__.__name__
			if base_class_name != class_name:
				inherited = '<div class="inherited">Inherited</div>\n'
		except AttributeError:
			pass
	return string.replace('\t', '&nbsp;&nbsp;&nbsp;&nbsp;')+'</div>\n'+inherited

class FilesManager:
	def __init__(self):
		self.files = {}
		f = open("header.htm")
		self.header = f.read()
		f.close()
		f = open("footer.htm")
		self.footer = f.read()
		f.close()

	def add(self, filename, string):
		if not self.files.has_key(filename):
			self.files[filename] = open('../doc/' + filename + '.html', 'w')
			self.files[filename].write(self.header.replace('TITLE', filename))
		self.files[filename].write(string)

	def __del__(self):
		for filename in self.files:
			self.files[filename].write(self.footer)
			self.files[filename].close()


def Main():

	fm = FilesManager()

	fm.add('index', '<h1>PySFML index</h1>\n')

	fm.add('index', '<h2>Classes</h2>\n')

	module_methods = ""
	module_constants = ""

	for m in dir(sf):
		if not m.startswith('__'):
			if m == 'Event':
				attr = sf.Event()
			else:
				attr = getattr(sf, m)
			if type(attr) == str:
				module_constants += '<div class="attr_name">'+m+'</div>\n<div class="desc">"'+attr+'"</div>\n'
			elif str(type(attr)) == "<type 'builtin_function_or_method'>" or str(type(attr)) == "<type 'method_descriptor'>":
				module_methods += function_str(attr)
			else:
				fm.add('index', '<a href="'+m+'.html">'+m+'</a><br />\n')
				info = {'Attributes':'', 'Constants':'', 'Methods':'', 'Static methods':''}
				members = ""
				constants = ""
				static_methods = ""
				methods = ""
				for n in dir(attr):
					if not n.startswith('__'):
						attr2 = getattr(attr, n)
						if str(type(attr2)) == "<type 'member_descriptor'>": # member
							info['Attributes'] += '<div class="attr_name">'+n+'</div>\n<div class="desc">'+attr2.__doc__+'</div>\n'
						elif type(attr2) == long:
							info['Attributes'] += '<div class="attr_name">'+n+'</div>\n'
						elif type(attr2) == int or type(attr2) == sf.Color: # int or color (constant)
							info['Constants'] += '<div class="attr_name">'+n+'</div>\n'
						elif str(type(attr2)) == "<type 'builtin_function_or_method'>": # static method
							info['Static methods'] += function_str(attr2, m)
						elif str(type(attr2)) == "<type 'method_descriptor'>": # method
							info['Methods'] += function_str(attr2, m)
						elif str(type(attr2)).startswith("<type 'Event."):
							info['Attributes'] += '<div class="attr_name">'+n+'</div>\n<div class="desc">' + attr2.__doc__+'</div>\n'
							for o in dir(attr2):
								if not o.startswith('__'):
									attr3 = getattr(attr2, o)
									info['Attributes'] += '<div class="event_member"> > ' + o + '</div>\n'
						else:
							print "Warning : unknown type for " + n + " " + str(type(attr2))
				fm.add(m, '<h1>sf.'+m+' Class Reference</h1>\n')
				fm.add(m, '<div class="class_desc">'+attr.__doc__.replace('\n', '<br />\n').replace('\t', '&nbsp;&nbsp;&nbsp;&nbsp;')+'</div>\n')
				if m != 'Event':
					base = attr.__base__.__name__
					if base != 'object':
						fm.add(m, '<div class="base_class">Base class: <a href="'+base+'.html">sf.'+base+'</a>.</div>\n')
				for t in info:
					if info[t] != '':
						fm.add(m, '<h2>'+t+'</h2>\n'+info[t]+'<br />\n')
				fm.add(m, '<br />\n<br />\n')

	fm.add('index', '<h2>Module methods</h2>\n' + module_methods)

	fm.add('index', '<h2>Module constants</h2>\n' + module_constants)

Main()

