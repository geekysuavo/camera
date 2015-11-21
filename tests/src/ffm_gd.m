% ffm_gd: barebones 1d gradient descent reconstruction function.
function [x, fvals] = ffm_gd (b, sched, def, M)
  % store the input vector length.
  N = length(b);

  % build a sampling vector.
  K = zeros(2 * N, 1);
  K(sched) = ones(size(sched));

  % initialize the objective value vector.
  fvals = [];

  % initialize the reconstruction vectors.
  b = [b; zeros(N, 1)];
  x = b;

  % compute the Lipschitz constant.
  L = 0.5 / def;

  % loop over the inner indices.
  for m = 1 : M
    % compute the current spectral estimate.
    X = fft(x);

    % compute the functional values.
    [fX, dfdX] = camera_functional(X, def);

    % compute and store the time-domain gradient.
    dfdx = (2 * N) .* ifft(dfdX);
    dfdx = (1 / L) .* dfdx .* (1 - K);
    fvals = [fvals; fX];

    % compute an x-update.
    x -= dfdx;
  end

  % truncate the reconstructed vector.
  x(N + 1 : end) = [];
end
