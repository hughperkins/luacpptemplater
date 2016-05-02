# cpptemplater
Write Jinja2-style templates in C++.  Uses lua as a scripting language (very lightweight)

## How I use it

I use cpptemplater for templating OpenCL kernels, eg https://github.com/hughperkins/cltorch/blob/master/src/lib/THClScatter.cl. Enabling/disabling chunks of code:
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

## How to use cppluatemplater

### To build:
```
git clone https://github.com/hughperkins/cppluatemplater
cd cppluatemplater
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

You can run `../dist/bin/cppluatemplater_unittests` to convince yourself everything is working ok

### To link to your own project

* link with libcppluatemplater.so
* there is a .cmake file you can import from cmakelists, at `../dist/lib/cppluatemplater/cppluatemplater-targets.cmake`

## Related projects

* initially, I dabbled with writing a templating engine in pure C++, ie [Jinja2CppLight](https://github.com/hughperkins/Jinja2CppLight)
* however, it quickly became apparent that I was gradually reinventing my own scripting language from scratch...
* so I switched to lua.  It works great.  Never looked back :-)

## License

BSD v2

