% get the output filename.
args = argv();
outfile = args{1};

% add the extra source path.
addpath('../src');

% load the input file.
load('input.dat.gz');

% set up the global parameters.
N = 1024;
t = linspace(0, 1, N)';
A = [1 : 20]';
sigma = 0.05;
freq = 100;
decay = 5;
def = 1e-6;
lambda = [];
aim = [1e-9; 1e-6; linspace(1e-3, 5, 15)'];
iters = 500;

% build the input data structures.
B.td = cell(length(A), 1);
B.fd = cell(length(A), 1);
X.td = cell(length(A), 1);
X.fd = cell(length(A), 1);
a = [];
for j = 1 : length(A)
  % build the input vector.
  x = A(j) .* exp(-i .* freq .* t - decay .* t);
  x += complex(normrnd(0, sigma, N, 1), normrnd(0, sigma, N, 1));

  % subsample the vector.
  b = x;
  b(setdiff([1 : N]', sched)) = 0;

  % compute the nudft and store everything.
  B.td{j} = b;
  X.td{j} = x;
  B.fd{j} = real(fftshift(fft(b)));
  X.fd{j} = real(fftshift(fft(x)));

  % compute the input intensity.
  a = [a; max(X.fd{j})];
end

% initialize the results matrix.
results = [A, a];

% loop over the aims.
for i = 1 : length(aim)
  % initialize the amplitude vector.
  a = [];

  % loop over the amplitudes.
  for j = 1 : length(A)
    % compute the maxent reconstruction.
    [x, f] = camera(B.td{j}, sched, aim(i), lambda, def, 1, iters);
    xft = real(fftshift(fft(x)));
    a = [a; max(xft)];
  end

  % save the amplitude vector.
  results = [results, a];
end

% save the results.
save('-ascii', outfile, 'results');
