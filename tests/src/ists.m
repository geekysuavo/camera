% ists: barebones 1d stern ist reconstruction function.
function [x, fvals] = ists (b, sched, mu, M)
  % store the input vector length.
  N = length(b);

  % build a sampling vector.
  K = zeros(2 * N, 1);
  K(sched) = ones(size(sched));

  % build an initial threshold value.
  th = max(abs(fft(b)));

  % initialize the objective value vector.
  fvals = [];

  % initialize the reconstruction vectors.
  b = [b; zeros(N, 1)];
  X = zeros(2 * N, 1);
  x = zeros(2 * N, 1);

  % loop over the outer indices.
  for m = 1 : M
    % compute the current spectral estimate.
    dx = K .* (b - x);
    X += fft(dx);

    % compute and store the functional values.
    [fX, X] = ists_functional(X, th);
    fvals = [fvals; fX];

    % update the estimate and the threshold.
    x = ifft(X);
    th *= mu;
  end

  % truncate the reconstructed vector.
  x(N + 1 : end) = [];
end
