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

% compute the nudft spectrum.
ph = exp(i .* linspace(-1, 1, 2 * length(b))' .* 0.4);
B = real(fftshift(fft([b; zeros(size(b))])) .* ph);
results = [[1 : length(B)]', B];

% compute the reconstructions.
for k = 1 : length(def)
  [x, f] = camera(b, sched, aim, lambda, def(k), 1, iters);
  X = real(fftshift(fft([x; zeros(size(x))])) .* ph);
  results = [results, X];
end

% save the convergence results.
save('-ascii', outfile, 'results');
