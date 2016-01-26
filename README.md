# camera

A command-line utility to reconstruct nonuniformly sampled (NUS) nuclear
magnetic resonance (NMR) data using an accelerated first-order convex
solver for the maximum entropy (MaxEnt) objective function. The
fundamental principles of the **camera** utility are published in:

> Worley, B., _Convex Accelerated Maximum Entropy Reconstruction_,
> Journal of Magnetic Resonance, 2016, Under review.

## Introduction

The **camera** utility is a purpose-built implementation of Nesterov's
accelerated first-order convex optimization algorithm applied to the
MaxEnt reconstruction problem in NUS NMR. As it turns out, the Hoch-Hore
spin-half entropy functional is perfectly suited to this accelerated
algorithm, because it is smooth, convex, and Lipschitz continuous with
easily tunable curvature.

To fully exploit the rapid first-order convergence of Nesterov's method,
**camera** was written from scratch with the goal of fast execution on
modern multi-core workstations and compute servers. Whenever supported
by the system, **camera** uses hardware-accelerated [single-instruction
multiple-data](https://en.wikipedia.org/wiki/SIMD) scalar types to store
and manipulate hypercomplex numbers, as well as the
[OpenMP API](https://en.wikipedia.org/wiki/OpenMP) to execute independent
reconstructions in parallel. Running **camera** requires only trivial
changes to standard nmrPipe processing workflows, and execution times
rival those of [RNMRTK](http://rnmrtk.uchc.edu) MaxEnt and
[NESTA-NMR](http://www.nestanmr.com).

As of 26 Jan 2016, **camera** utilizes the [FFTW](http://www.fftw.org)
library for all its fast Fourier transforms. This means:

 * Data are no longer limited to power-of-two sizes.
 * Reconstructions will be up to _3x_ faster.

### Reconstructing NMR data

The **camera** algorithm and software supports all the trimmings of MEM,
including interpolation, extrapolation, linewdith deconvolution and
_J_-coupling deconvolution. Example scripts for converting, processing
and reconstructing 2D, 3D and 4D Bruker NMR data are provided
in the [data](data/) subdirectory.

### Installing and Running

The **camera** utility is written in C99-compliant source code, and
requires a compiler with OpenMP support. Use of the
[GNU Compiler Collection] (http://gcc.gnu.org) version 4.9 or later
is recommended.

You can compile and install **camera** as follows:

```bash
git clone git://github.com/geekysuavo/camera.git
cd camera
make
sudo make install
```

When compiling **camera** from source, the [FFTW](http://www.fftw.org)
development headers and libraries must be installed. A recent processor
that supports
[SSE] (https://en.wikipedia.org/wiki/Streaming_SIMD_Extensions),
[SSE2] (https://en.wikipedia.org/wiki/SSE2), and
[SSE3] (https://en.wikipedia.org/wiki/SSE3) is required to
run **camera**, and support for
[AVX] (https://en.wikipedia.org/wiki/Advanced_Vector_Extensions) is
highly recommended for reconstructing 3D and 4D datasets. If you have a
processor that supports AVX, you will need to set USE_AVX to **y** in the
Makefile prior to running _make_ and _make install_ above. Or, you can run
this to build AVX support in:

```bash
sed -e 's,^\(USE_AVX\)=.*,\1=y,' -i Makefile
make again
sudo make install
```

By default, running _make install_ will install **camera** and its manual
page into the **/usr/local** prefix. To change this, modify the PREFIX
variable in the Makefile prior to running _make install_ above. Or:

```bash
sed -e 's,^\(PREFIX\)=.*,\1=/opt/camera,' -i Makefile
sudo make install
```

## Licensing

This project is released under the
[GNU GPL 3.0] (http://www.gnu.org/licenses/gpl-3.0.txt).

Have fun with it,

*~ Brad.*

