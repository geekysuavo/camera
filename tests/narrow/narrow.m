% get the output filename.
args = argv();
outfile = args{1};

% add the extra source path.
addpath('../src');

% load the input data.
load('input.dat.gz');

% set up the global parameters.
def = 1;
aim = 1e5;
lambda = [];
iters = 500;
lw = [0 : 2 : 15];

% compute the nudft spectrum.
ph = exp(i .* linspace(-1, 1, 2 * length(b))' .* 0.4);
B = real(fftshift(fft([b; zeros(size(b))])) .* ph);
results = [[1 : length(B)]', B];

% compute the reconstructions.
for i = 1 : length(lw)
  % define the convolution function.
  wfn = @(idx) exp(-lw(i) .* idx ./ 2367);

  % compute the reconstruction.
  [x, f] = camera(b, sched, aim, lambda, def, 100, 1, iters, 2, wfn);
  X = real(fftshift(fft(x)) .* ph);
  results = [results, X];
end

% save the convergence results.
save('-ascii', outfile, 'results');
