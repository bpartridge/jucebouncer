# Jucebouncer

Jucebouncer is a simple web service that provides fast offline (non-realtime) 
bouncing/rendering of simple musical content through a specified VST
or other plugin. Parameter settings can be specified through JSON post data.
Audio plugin interfacing is provided by 
[JUCE](http://www.rawmaterialsoftware.com/juce.php).

## API

`GET /index.html` or `GET /` for an interactive demo using Javascript.

`GET /list.json` to list the names and current values of all 
plugin parameters that can be modified. Each value is normalized to a 
floating-point value between 0 and 1.

`GET /render.wav` to render and download a WAV sound file corresponding
to the *entire* query string decoded and parsed as JSON. For instance, `GET /render.wav?{%22foo%22:%22bar%22}` will be parsed as `{"foo": "bar"}`. 
See AudioRequestParameters in [main.cpp](src/main.cpp) for more details
about what parameters are accepted.
Note that you can force this endpoint to emit JSON as above by setting
`{'listParameters':true}`. Additionally, note that this endpoint is
useful when using 
[SoundManager2](http://www.schillmania.com/projects/soundmanager2/)
because you can simply load sounds with 
`soundManager.createSound({..., url: "/render.wav?" + JSON.stringify(data)})`.

`POST /render.wav` to render and download a WAV sound file corresponding
to JSON content provided in the POST data, with the same semantics as the
GET method.

The following CURL commands demonstrate the functionality, assuming the
service is running on port 8080:

- If the audio player SoX is installed and in your PATH, run 
`curl localhost:8080/render.wav -s -d '{"midiPitch":80}' | play -`
to hear the sound. 

- Otherwise, if on OS X, run: 
`curl localhost:8080/render.wav -d '{"midiPitch":80}' > tmp/render2.wav && echo "Playing" && afplay tmp/render2.wav`

- `curl localhost:8080/list.json` should emit a list of parameter names that can be manipulated.

## Compiling and running

Clone this repository and run `git submodule init` and 
`git submodule update` to fetch the correct version of JUCE.

Before compiling, you must also make an account and download the 
VST 2.3 & 2.4 development kit 
[here](http://www.steinberg.net/en/company/developer.html),
since licensees are not allowed to distribute copies.
Place the vstsdk2.4 directory in the lib directory such that you have
files like `lib/vstsdk2.4/pluginterfaces/vst2.x/aeffect.h`.

At the moment, Mac OS X is the only supported platform. Installation requires 
[CMake](http://www.cmake.org/cmake/resources/software.html).

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

Feel free to send a pull request for any reason. 
See the [Issues](issues) page for known outstanding problems.

## License

This software is released under the GNU General Public License and is
(c) Brenton Partridge 2013.
