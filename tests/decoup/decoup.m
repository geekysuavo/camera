% get the output filename.
args = argv();
outfile = args{1};

% add the extra source path.
addpath('../src');

% load the input data.
load('input.dat.gz');

% set up the global parameters.
def = 1;
aim = 7e5;
lambda = [];
iters = 500;
J = [1 : 5 : 51];

% compute the nudft spectrum.
B = real(fftshift(fft(i .* [b; zeros(size(b))])));
results = [[1 : length(B)]', B];

% compute the reconstructions.
for j = 1 : length(J)
  % define the convolution function.
  wfn = @(idx) cos(pi .* J(j) .* idx ./ 10571);

  % compute the reconstruction.
  [x, f] = camera(b, sched, aim, lambda, def, 1, iters, 1, wfn);
  X = real(fftshift(fft(i .* [x; zeros(size(x))])));
  results = [results, X];
end

% save the convergence results.
save('-ascii', outfile, 'results');
