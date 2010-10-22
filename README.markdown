pdlib [one red dog]
===================

This is a fork of byransum/pdlib (http://github.com/bryansum/pdlib) that I have optimised for
the Apple iPad. These optimisations consist of the following:

1. Added ARM NEON vector optimisation into the Pd source code
2. Replaced the socket based message passing in liblo with queues
3. Added a means by which we can export the audio buffer into a WAV file

The NEON code is dependent upon math-neon which is available here: http://code.google.com/p/math-neon/

Pd NEON
--------------------

Inside the Pd DSP code there are many "perf8" functions which act on vectors of data, in Pd's case
these are C arrays. To gain more performance on low power devices like the iPad's ARM these
functions are ripe for vectorisation. In desktop versionsof Pd over time people have converted
the generic C code into PowerPC AltiVec and Intel SSE. Adding some NEON code here made the Pd a 
lot more performant and meant I could run multiple synthesizer voices and effects in my iPad app.

liblo
--------------------

The main app GUI and pdlib run in separate threads, so the easist way to communicate between these
two threads is to use OSC. The original pdlib chose to use liblo for this which opens localhost socket 
connections to send the OSC messages. Nothing wrong with that, except when high volumes of messages
are sent, e.g. the user manipulates a dial, the socket buffers can be flooded and messages dropped. 
A feature of BSD UDP sockets. Therefore, I added two queues to directly send between the threads
without the overhead of the socket interface.

WAV Export
--------------------

My iPad application has the feature of being able to export audio to a WAV file. I added extra
code to write the CoreAudio render buffer to a file.

Extras
--------------------

I had a requirement for more extras, some of them from the full version of Pd. So I added the 
relevant code.

Building
--------------------

All my building is handled in Xcode, I don't build any of these libraries externally so makefiles are
probably broken.

Thanks to Bryan Summersett for doing the original port.

