% camera: barebones 1d camera reconstruction function.
function [x, fvals] = ...
camera (b, sched, sigma, lambda, defs, Mout, Min, nzf, Wfn)
  % check for a minimum number of arguments.
  if (nargin < 2)
    error('at least two arguments required');
  end

  % check that the required input arguments are vectors.
  if (!isvector(b) || !isvector(sched))
    error('"b" and "sched" must be vectors');
  end

  % store the input vector lengths.
  n = length(sched);
  N0 = length(b);

  % check for an estimated noise argument.
  if (nargin < 3 || isempty(sigma))
    % none supplied, use a default value.
    sigma = 0.001 * norm(b);
  end

  % check for a lagrange multiplier argument.
  if (nargin < 4)
    % none supplied, use a default value.
    lambda = [];
  end

  % check for a default background argument.
  if (nargin < 5 || isempty(defs))
    % none supplied, use a default value.
    defs = 0.1 * sigma;
  end

  % check for an outer loop count argument.
  if (nargin < 6 || isempty(Mout))
    % none supplied, use a default value.
    Mout = 1;
  end

  % check for an inner loop count argument.
  if (nargin < 7 || isempty(Min))
    % none supplied, use a default value.
    Min = 500;
  end

  % check for a zero-fill count argument.
  if (nargin < 8 || isempty(nzf))
    % none supplied, use a default value.
    nzf = 1;
  end

  % check for a weight argument.
  if (nargin < 9 || isempty(Wfn))
    % none supplied, use a default value.
    Wfn = @(idx) 1;
  end

  % set the output size.
  N = (2 ^ nzf) * N0;

  % build a sampling vector.
  K = zeros(N, 1);
  K(sched) = ones(size(sched));

  % build a weighting vector.
  W = Wfn([0 : N - 1]');

  % build easy-to-understand transformation variables.
  A = K .* W;
  At = W .* K;
  AtA = (W .^ 2) .* K;

  % build an epsilon value.
  vareps = sqrt(2 * n) * sigma;

  % initialize the objective value vector.
  fvals = [];

  % initialize the reconstruction vectors.
  b = [b; zeros(N - N0, 1)];
  x = b;
  y = zeros(N, 1);
  z = zeros(N, 1);

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
    adf = zeros(N, 1);

    % loop over the inner indices.
    for mi = 1 : Min
      % compute the current spectral estimate.
      X = fft(x);

      % compute the functional values.
      [fX, dfdX] = camera_functional(X, def);

      % compute and store the time-domain gradient.
      dfdx = N .* ifft(dfdX);
      fvals = [fvals; fX];

      % compute the new coupling constants.
      alpha = 0.5 * (mi + 1);
      tau = 2 / (mi + 3);

      % update the weighted gradient sum.
      adf += alpha .* dfdx;

      % compute a y-update.
      if (isempty(lambda))
        ly = max(0, (L/vareps) .* norm(b - A .* (x - dfdx ./ L)) - L);
      else
        ly = lambda;
      end
      gy = 1 ./ (1 + (ly / L) .* AtA);
      y = gy .* (x + (ly / L) .* At .* b - dfdx ./ L);

      % compute a z-update.
      if (isempty(lambda))
        lz = max(0, (L/vareps) .* norm(b - A .* (xc - adf ./ L)) - L);
      else
        lz = lambda;
      end
      gz = 1 ./ (1 + (lz / L) .* AtA);
      z = gz .* (xc + (lz / L) .* At .* b - adf ./ L);

      % compute an x-update.
      x = tau .* z + (1 - tau) .* y;
    end
  end

  % truncate the reconstructed vector.
  x(N / 2 + 1 : end) = [];
end
