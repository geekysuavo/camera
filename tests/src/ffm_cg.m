% ffm_cg: barebones 1d conjugate gradient reconstruction function.
function [x, fvals] = ffm_cg (b, sched, def, M)
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

  % initialize the conjugate direction.
  s = zeros(2 * N, 1);

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

    % compute the conjugate gradient factor.
    if (m > 1)
      bf = max(0, (dfdx' * (dfdx - dfdxPrev)) / (dfdxPrev' * dfdxPrev));
    else
      bf = 0;
    end

    % compute an x-update.
    s = dfdx + bf .* s;
    x -= s;

    % store the previous gradient.
    dfdxPrev = dfdx;
  end

  % truncate the reconstructed vector.
  x(N + 1 : end) = [];
end
