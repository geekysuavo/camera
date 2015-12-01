# camera/tests

All [GNU Octave] (https://www.gnu.org/software/octave/) scripts used to
test and demonstrate the basic features of **camera**, such as convergence,
are stored here.

## Running the scripts

You can build everything in all subdirectories by running **make** from
this location, or you can selectively build any of the subdirectories
here by changing into that directory and running **make**, _i.e._:

```bash
cd conv
make
```

## Subdirectories

### conv

This contains several analyses of the convergence behavior of **camera**.
A comparison of convergence with gradient descent and conjugate gradient
fast forward MaxEnt (FFM), as well as convergence analyses at several
values of _lambda_ and _delta_ are stored here.

### decoup

This subdirectory contains a demonstration of using **camera** to achieve
_J_-coupling deconvolution, similar to that performed here:

> Shimba, N. et al., _Elimination of Ca-13 Splitting in Protein NMR Spectra
> by Deconvolution with Maximum Entropy Reconstruction_, JACS, 2003,
> 125: 2382-2383.

### narrow

This subdirectory contains a similar demonstration of deconvolution, but
using an exponential decay for a convolution kernel.

### nonlin

This subdirectory contains an analysis of the nonlinearity of **camera**
reconstructions, similar to that performed in this work:

> Schmieder, P. et al., _Quantification of Maximum-Entropy Spectrum
> Reconstructions_, Journal of Magnetic Resonance, 1997, 125: 332-339.

### split

This subdirectory contains an analysis of the effect of constraint strength
(_i.e._ sigma value) on reconstructions, similar to that performed in
this published work:

> Stern, A. S. et al., _A New Approach to Compressed Sensing for NMR_,
> Magnetic Resonance in Chemistry, 2015, 53(11): 908-912.

### src

Implementations of **camera**, Iterative Soft Thresholding (IST-S), and
Fast Forward MaxEnt (FFM) are stored here. The IST-S implementation is
based the description here:

> Stern, A. S. et al., _NMR Data Processing using Iterative Soft Thresholding
> and Minimum l1-norm Reconstruction_, Journal of Magnetic Resonance, 2007,
> 188: 295-300.

The FFM implementation is based on descriptions here:

> Balsgart, N. M. et al., _Fast Forward Maximum Entropy Reconstruction of
> Sparsely Sampled Data_, Journal of Magnetic Resonance, 2012, 223: 164-169.

Both gradient descent and conjugate gradient (Polak-Ribiere) implementations
of FFM are stored here.

