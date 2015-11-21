% get the output filename.
args = argv();
outfile = args{1};

% add the extra source path.
addpath('../src');

% load the input data.
load('input.dat.gz');

% set up the global parameters.
def = [1, 10, 25, 50, 70, 100];
aim = 1e5;
lambda = [];
iters = 500;

% compute the reconstructions.
results = [1 : iters]';
for k = 1 : length(def)
  [x, f] = camera(b, sched, aim, lambda, def(k), 1, iters);
  results = [results, f];
end

% save the convergence results.
save('-ascii', outfile, 'results');
