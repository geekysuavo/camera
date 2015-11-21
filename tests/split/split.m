% get the output filename.
args = argv();
outfile = args{1};

% add the extra source path.
addpath('../src');

% set up the global parameters.
def = 1e-4;
aim = [1e-6, 0.8];
lambda = [];
iters = 500;
mu = 0.98;

% initialize the results matrix.
results = [];

% loop over the two input datasets.
kv = [100, 300];
for k = 1 : 2
  % load the input data.
  load(['input', num2str(kv(k)), '.dat.gz']);

  % set up the results key column.
  if (isempty(results))
    results = [1 : 2 * length(b)]';
  end

  % compute the nudft spectrum.
  B = real(fftshift(fft([b; zeros(size(b))])));
  results = [results, B];

  % compute the ist reconstruction.
  [x, f] = ists(b, sched, mu, iters);
  X = real(fftshift(fft([x; zeros(size(x))])));
  results = [results, X];

  % compute the maxent reconstructions.
  for i = 1 : length(aim)
    [x, f] = camera(b, sched, aim(i), lambda, def, 1, iters);
    X = real(fftshift(fft([x; zeros(size(x))])));
    results = [results, X];
  end
end

% save the results.
save('-ascii', outfile, 'results');
