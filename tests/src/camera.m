% camera: barebones 1d camera reconstruction function.
function [x, fvals] = camera (b, sched, sigma, lambda, defs, Mout, Min)
  % store the input vector length.
  n = length(sched);
  N = length(b);

  % build a sampling vector.
  K = zeros(2 * N, 1);
  K(sched) = ones(size(sched));

  % build an epsilon value.
  vareps = sqrt(2 * n) * sigma;

  % initialize the objective value vector.
  fvals = [];

  % initialize the reconstruction vectors.
  b = [b; zeros(N, 1)];
  x = b;
  y = zeros(2 * N, 1);
  z = zeros(2 * N, 1);

  % check that the defs vector is the correct size.
  if (length(defs) != Mout)
    defs = repmat(defs(1), Mout, 1);
  end

  % loop over the outer indices.
  for mo = 1 : Mout
    % get the def value and compute the Lipschitz constant.
    def = defs(mo);
    L = 0.5 / def;

    % initialize the prox-center and weighted gradient sum.
    xc = x;
    adf = zeros(2 * N, 1);

    % loop over the inner indices.
    for mi = 1 : Min
      % compute the current spectral estimate.
      X = fft(x);

      % compute the functional values.
      [fX, dfdX] = camera_functional(X, def);

      % compute and store the time-domain gradient.
      dfdx = (2 * N) .* ifft(dfdX);
      fvals = [fvals; fX];

      % compute the new coupling constants.
      alpha = 0.5 * (mi + 1);
      tau = 2 / (mi + 3);

      % update the weighted gradient sum.
      adf += alpha .* dfdx;

      % compute a y-update.
      if (isempty(lambda))
        ly = max(0, (L/vareps) .* norm(b - K .* (x - dfdx ./ L)) - L);
      else
        ly = lambda;
      end
      y = (1 - (ly / (ly + L)) .* K) .* ...
          (x + (ly / L) .* b - dfdx ./ L);

      % compute a z-update.
      if (isempty(lambda))
        lz = max(0, (L/vareps) .* norm(b - K .* (xc - adf ./ L)) - L);
      else
        lz = lambda;
      end
      z = (1 - (lz / (lz + L)) .* K) .* ...
          (xc + (lz / L) .* b - adf ./ L);

      % compute an x-update.
      x = tau .* z + (1 - tau) .* y;
    end
  end

  % truncate the reconstructed vector.
  x(N + 1 : end) = [];
end
