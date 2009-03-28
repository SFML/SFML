#!/usr/bin/python

from PySFML import sf

class MyCustomStream(sf.SoundStream):

	def Open(self, Filename):
		# Load the sound data into a sound buffer
		self.SoundData = sf.SoundBuffer()
		if not self.SoundData.LoadFromFile(Filename):
			return False
		# Initialize the stream with the sound parameters
		self.Initialize(self.SoundData.GetChannelsCount(), self.SoundData.GetSampleRate())
		# Copy the audio samples into our internal array
		self.myBuffer = self.SoundData.GetSamples()
		return True

	def OnStart(self):
		self.myOffset = 0
		self.myBufferSize = 80000
		return True

	def OnGetData(self):
		# Check if there is enough data to stream
		if self.myOffset > len(self.myBuffer):
			# Returning something else than a string means that we want to stop playing the stream
			return ""
		# Data contains the string of samples we will return
		if self.myOffset + self.myBufferSize >= len(self.myBuffer):
			print "End of audio data reached"
			Data = self.myBuffer[self.myOffset:]
		else:
			Data = self.myBuffer[self.myOffset:self.myOffset+self.myBufferSize]
		# Update the offset
		self.myOffset = self.myBufferSize + self.myOffset
		return Data

def Main():
	Stream = MyCustomStream()
	Stream.Open("./data/fart.wav")
	Stream.Play()
	print "Playing 5 seconds of audio data..."
	sf.Sleep(5)
	Stream.Stop()
	print "Press enter to exit..."
	raw_input()

Main()

