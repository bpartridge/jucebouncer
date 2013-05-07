# Jucebouncer

Jucebouncer is a simple web service that provides fast offline (non-realtime) 
bouncing/rendering of simple musical content through a specified VST
or other plugin. Parameter settings can be specified through JSON post data.

## API

`GET /test.html` for an interactive demo using Javascript.

`GET /list.json` to list the names of all plugin parameters that can 
be modified. Each of these is normalized to a floating-point value between
0 and 1.

`POST /render.wav` to render and download a WAV sound file corresponding
to JSON content provided in the POST data. See AudioRequestParameters in
[main.cpp](src/main.cpp) for more details about what parameters are accepted.
Note that you can force this endpoint to emit JSON as above by setting .
`{'listParameters':true}`.

The following CURL commands demonstrate the functionality, assuming the
service is running on port 8080:

- If the audio player SoX is installed and in your PATH, run 
`curl localhost:8080/render.wav -s -d '{"midiPitch":80}' | play -`
to hear the sound. 

- Otherwise, if on OS X, run 
`curl localhost:8080/render.wav -d '{"midiPitch":80}' > tmp/render2.wav && echo "Playing sound..." && afplay tmp/render2.wav`.

- `curl localhost:8080/list.json` should emit a list of parameter names that can be manipulated.

## Compiling and running

Clone this repository and run `git submodule init` and 
`git submodule update` to fetch the correct version of JUCE.

Before compiling, you must also make an account and download the 
VST 2.3 & 2.4 development kit 
[here](http://www.steinberg.net/en/company/developer.html).
Place the vstsdk2.4 directory in the lib directory such that you have
files like `lib/vstsdk2.4/pluginterfaces/vst2.x/aeffect.h`.

At the moment, Mac OS X is the only supported platform. 
Installation requires CMake, which can be downloaded from 
`http://www.cmake.org/cmake/resources/software.html`.

You can compile and start the server with 
`cmake . && make && bin/jucebouncer`.

## Implementation Details

We use Mongoose as a multi-threaded web server. However, we must ensure
that each plugin instance is only used from one thread at a time.
Therefore, for each plugin of interest
(currently just LinPlug's FreeAlpha synthesizer VST), we preload a pool of
plugin instances. Each time a server thread wishes to perform an action,
it tries (until timeout) to acquire a re-entrant mutex on one of the pool
instances, then releases it once rendering completes.

## Contributing

Feel free to send a pull request for any reason. See the Issues page for
known outstanding problems.
