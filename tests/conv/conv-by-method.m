% get the output filename.
args = argv();
outfile = args{1};

% add the extra source path.
addpath('../src');

% load the input data.
load('input.dat.gz');

% set up the global parameters.
def = 1;
aim = 1e3;
lambda = [];
iters = 10000;

% compute the reconstructions.
[xGD, fGD] = ffm_gd(b, sched, def, iters);
[xCG, fCG] = ffm_cg(b, sched, def, iters);
[xCA, fCA] = camera(b, sched, aim, lambda, def, 1, 1, iters);
[xLS, fLS] = camera(b, sched, aim, lambda, def, 500, 1, iters);

% save the convergence results.
results = [[1 : iters]', fGD, fCG, fCA, fLS];
save('-ascii', outfile, 'results');
