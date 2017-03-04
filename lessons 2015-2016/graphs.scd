(
{
    var sig = SinOsc.ar(4000);
    var env = EnvGen.ar(Env.triangle(0.01));
    [sig, env, sig * env]
}.plot(0.01)
)


([
    Signal.welchWindow(1000),
    Signal.hanningWindow(1000),
    Signal.hammingWindow(1000),
    Env.perc()
].plot
)