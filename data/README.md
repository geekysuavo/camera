# camera/data

All datasets used to test and characterize **camera** are stored within
subdirectories here. Each data directory contains _acqus_ files generated
from Bruker Topspin, and a _scripts_ subdirectory that holds all necessary
conversion, reconstruction and processing steps for that dataset.

## Running the scripts

To build (_i.e._ reconstruct and process) all the datasets, you can just
run **make** from this directory. To build an individual dataset, simply
run **make** from its _scripts_ subdirectory. For example, to build the
HNCO dataset, run:

> cd hnco/scripts && make

Raw serial files are not stored in this GitHub repository, for the sake
of minimizing the disk storage required by the **camera** source tree.
However, the _scripts/Makefile_ within each data subdirectory contains
a Dropbox link to its respective _ser_ file. Thus, running **make**
here or in any _scripts_ subdirectory will automatically download
all required serial files.

## Datasets

### nhsqc-gb1

The [nhsqc-gb1](nhsqc-gb1/) directory contains a 1H-15N HSQC originally
provided by the Wagner group to demonstrate
the [hmsIST] (http://gwagner.med.harvard.edu/intranet/hmsIST/) program.
It was collected using Poisson-gap sampling at 10% sparsity on a 512-point
hypercomplex grid.

### nhsqc-ubq-sigma

The [nhsqc-ubq-sigma](nhsqc-ubq-sigma/) directory holds a 1H-15N HSQC of
ubiquitin, originally collected to compare the performance of Poisson-gap,
sine-gap and sine-burst sampling in one dimension:

> Worley, B., Powers, R., _Deterministic Multidimensional Nonuniform
> Gap Sampling_, Journal of Magnetic Resonance, 2015, 261: 19-26.

The data was collected using a sine-gap schedule at 5% sparsity on a 1k-point
hypercomplex grid. This folder contains scripts to reconstruct the data in
constant-aim (_i.e._ constant-sigma) mode.

### nhsqc-ubq-lambda

The [nhsqc-ubq-lambda](nhsqc-ubq-lambda/) directory has the same dataset of
ubiquitin, but with scripts to reconstruct the data in constant-lambda mode.
The value of _lambda_ used in these scripts was determined by examining the
resulting Lagrange multipliers from the constant-aim dataset in the
[nhsqc-ubq-sigma](nhsqc-ubq-sigma/) directory.

### chsqc-ca

The [chsqc-ca](chsqc-ca/) directory houses a semi-selective 1H-13C HSQC of
ubiquitin, with evolution times set to select for alpha carbons. The data
was collected at 5% sparsity on a 2048-point grid using sine-gap sampling.

### chsqc-cacb

The [chsqc-cacb](chsqc-cacb/) directory holds the same 1H-13C HSQC as the
[chsqc-ca](chsqc-ca/) directory, but with evolution times set to select for
both alpha and beta carbons.

### hnco

The [hnco](hnco/) directory contains an HNCO of ubiquitin, acquired for
reference purposes for the
[UNL RIF] (http://chem.unl.edu/research-instrumentation). It was collected
using a sine-burst schedule at 5% sparsity on a 48x128-point hypercomplex
grid.

### hnco-ext

The [hnco-ext](hnco-ext/) directory holds the same HNCO dataset, but with
slightly different reconstruction parameters. Specifically, this is an
testbed for how **camera** behaves when asked to simultaneously perform
MaxEnt reconstruction and extrapolation. Thus, the original 48x128-point
dataset is extrapolated into a 128x256-point grid.

### noesy

The [noesy](noesy/) directory holds an HCCH-NOESY (HSQC-NOESY-HSQC) of
ubiquitin, collected specifically to implement and test **camera** on
four-dimensional data. It was collected using a subrandom exponentially
weighted (with 2x bias) schedule at 0.8% sparsity on a 64x64x64-point
hypercomplex grid.

