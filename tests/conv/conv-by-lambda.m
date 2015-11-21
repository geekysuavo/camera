% get the output filename.
args = argv();
outfile = args{1};

% add the extra source path.
addpath('../src');

% load the input data.
load('input.dat.gz');

% set up the global parameters.
def = [1, 10];
aim = 1e5;
lambda = [5, 10, 100, 1000];
iters = 500;

% compute the reconstructions.
results = [1 : iters]';
for i = 1 : length(def)
  for k = 1 : length(lambda)
    [x, f] = camera(b, sched, aim, lambda(k), def(i), 1, iters);
    results = [results, f];
  end
end

% save the convergence results.
save('-ascii', outfile, 'results');
