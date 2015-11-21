% get the output filename.
args = argv();
outfile = args{1};

% add the extra source path.
addpath('../src');

% load the input data.
load('input.dat.gz');

% set up the global parameters.
def = [10, 10, 10, 10; 100, 50, 25, 10];
aim = 1e5;
lambda = [];
iters = [1, 200; 4, 10];

% compute the reconstructions.
results = [1 : iters(1,1) * iters(1,2)]';
for k = 1 : rows(def)
  [x, f] = camera(b, sched, aim, lambda, def(k,:), iters(k,1), iters(k,2));
  f = [f; nan(rows(results) - length(f), 1)];
  results = [results, f];
end

% save the convergence results.
save('-ascii', outfile, 'results');
