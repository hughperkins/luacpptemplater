# cpptemplater
Write Jinja2-style templates in C++.  Uses lua as a scripting language (very lightweight)

## How I use it

I use luacpptemplater for templating OpenCL kernels, eg https://github.com/hughperkins/cltorch/blob/master/src/lib/THClScatter.cl. Enabling/disabling chunks of code:
```
{% if scatterFill then %}
kernel void THClTensor_kernel_scatterFill(
...
{% end %}
```
Unrolling loops:
```
int linearId = _linearId; // copy it, since we'll modify it
int curDimIndex;
{% for d=dims-1,0,-1 do %}
  curDimIndex = linearId % idx_info->sizes[{{d}}];
  idxOffset += curDimIndex * idx_info->strides[{{d}}];
  if( {{d}} != dim ) { // this only matters for the source, the others are 
                   // unaffected by which dimension we are on. I think.
    dstOffset += curDimIndex * dst_info->strides[{{d}}];
  }
  linearId /= idx_info->sizes[{{d}}];
{% end %}
```

## How it works

* contains a lua scripting engine, which is *very* lightweight
* can use the entire power of lua scripting inside your templates
* mostly I just use loops and `if` blocks, but the sky is the limit.  The entire power of lua scripting is at
our disposal

## How to use luacpptemplater

### To build:
```
git clone https://github.com/hughperkins/luacpptemplater
cd luacpptemplater
mkdir build
cd build
ccmake ..
# press 'c'
# select the options you want
# press 'c'
# press 'g'
make -j 4 install
```
=> the unit test (if selected) will install into `../dist/bin`, and the library will be built into `../dist/lib` (assuming
linux)

You can run `../dist/bin/luacpptemplater_unittests` to convince yourself everything is working ok

### To link to your own project

* link with libluacpptemplater.so
* there is a .cmake file you can import from cmakelists, at `../dist/lib/luacpptemplater/luacpptemplater-targets.cmake`

## How does it work?

- it basically uses the code from https://john.nachtimwald.com/2014/08/06/using-lua-as-a-templating-engine/  I think I did make some tweaks to it, and provided c++ wrapping around it, but basically it's John Nachtiwald's idea and core implementation.  His idea is very cool :-)

## Related projects

* initially, I dabbled with writing a templating engine in pure C++, ie [Jinja2CppLight](https://github.com/hughperkins/Jinja2CppLight)
* however, it quickly became apparent that I was gradually reinventing my own scripting language from scratch...
* So I hunted around for a Lua solution, which I found at https://john.nachtimwald.com/2014/08/06/using-lua-as-a-templating-engine/
* This worked great for me, and I've never looked back :-)

## License

BSD v2

