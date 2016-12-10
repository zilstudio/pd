    // Boot the server
    s.boot;

b = Buffer.readChannel(s, "/Users/zstudio/Documents/pd/lessons/grains/mill~/guitar_1.aif", 0, 44100 * 30, 1);

b.play


// NORMAL
(
{
    var trate, dur, rate;
    // X - grain pos
    // Y - grain size
    trate = MouseY.kr(2,200,1);
    dur = 4 / trate;
    rate = Dseq([10, 1, 1, 0.5, 0.5, 0.2, 0.1], inf);
    TGrains.ar(2, Impulse.ar(trate), b, rate, MouseX.kr(0,BufDur.kr(b)), dur, Dseq([-1, 1], inf), 0.1, 2) * 20;
}.scope(zoom: 4);
)

// TOY
(
{
    var trate, dur, clk, pos, pan;
    trate = MouseY.kr(1,400,1);
    dur = 8 / trate;
    clk = Impulse.kr(trate);
    pos = MouseX.kr(0,BufDur.kr(b));
    pan = WhiteNoise.kr(0.8);
    TGrains.ar(2, clk, b, 0.12 ** WhiteNoise.kr(2), pos, dur, pan, 0.1) * 10;
}.scope(zoom: 4);
)

(
{
    var trate, dur;
    trate = MouseY.kr(2,120,1);
    dur = 1.2 / trate;
    TGrains.ar(2, Impulse.ar(trate), b, (1.2 ** WhiteNoise.kr(3).round(1)), MouseX.kr(0,BufDur.kr(b)), dur, WhiteNoise.kr(0.6), 0.1);
}.scope(zoom: 4);
)

(
{
    var trate, dur, clk, pos, pan;
    trate = MouseY.kr(8,120,1);
    dur = 4 / trate;
    clk = Dust.kr(trate);
    pos = MouseX.kr(0,BufDur.kr(b)) + TRand.kr(0, 0.01, clk);
    pan = WhiteNoise.kr(0.6);
    TGrains.ar(2, clk, b, 1, pos, dur, pan, 0.1);
}.scope(zoom: 4);
)

(
{
    var trate, dur, clk, pos, pan;
    trate = LinExp.kr(LFTri.kr(MouseY.kr(0.1,2,1)),-1,1,8,120);
    dur = 12 / trate;
    clk = Impulse.ar(trate);
    pos = MouseX.kr(0,BufDur.kr(b));
    pan = WhiteNoise.kr(0.6);
    TGrains.ar(2, clk, b, 1, pos, dur, pan, 0.1);
}.scope(zoom: 4);
)

(
{
    var trate, dur, clk, pos, pan;
    trate = 12;
    dur = MouseY.kr(0.2,24,1) / trate;
    clk = Impulse.kr(trate);
    pos = MouseX.kr(0,BufDur.kr(b)) + TRand.kr(0, 0.01, clk);
    pan = WhiteNoise.kr(0.6);
    TGrains.ar(2, clk, b, 1, pos, dur, pan, 0.1);
}.scope(zoom: 4);
)

