% camera: barebones 1d camera reconstruction function.
function [x, fvals, nops] = ...
camera (b, sched, sigma, lambda, defs, def0, Mout, Min, nzf, Wfn)
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

  % check for a default line search argument.
  if (nargin < 6 || isempty(def0))
    % none supplied, use a default value.
    def0 = 1;
  end

  % check for an outer loop count argument.
  if (nargin < 7 || isempty(Mout))
    % none supplied, use a default value.
    Mout = 1;
  end

  % check for an inner loop count argument.
  if (nargin < 8 || isempty(Min))
    % none supplied, use a default value.
    Min = 500;
  end

  % check for a zero-fill count argument.
  if (nargin < 9 || isempty(nzf))
    % none supplied, use a default value.
    nzf = 1;
  end

  % check for a weight argument.
  if (nargin < 10 || isempty(Wfn))
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

  % initialize the operation count vector.
  nops = 0;

  % initialize the reconstruction vectors.
  b = [b; zeros(N - N0, 1)];
  x = b;
  y = b;

  % check that the defs vector is the correct size.
  if (length(defs) != Mout)
    defs = repmat(defs(1), Mout, 1);
  end

  % loop over the outer indices.
  for mo = 1 : Mout
    % get the def value and compute the Lipschitz constant.
    def = defs(mo);
    Lf = 0.5 / def;
    L = 0.5 / (def * def0);

    % compute the initial spectral estimate.
    X = fft(x);
    nops(end)++;

    % loop over the inner indices.
    for mi = 1 : Min
      % compute the functional values.
      [fX, dfdX] = camera_functional(X, def);
      fvals = [fvals; fX];

      % compute and store the time-domain gradient.
      dfdx = N .* ifft(dfdX);
      nops(end)++;

      % compute the velocity factor.
      beta = (mi - 1) / (mi + 2);

      % enter the line-search loop.
      do
        % compute a projected gradient update.
        if (isempty(lambda))
          lz = max(0, (L/vareps) .* norm(b - A .* (x - dfdx ./ L)) - L);
        else
          lz = lambda;
        end
        z = (1 ./ (1 + (lz / L) .* AtA)) .* ...
            (x + (lz / L) .* At .* b - dfdx ./ L);

        % compute a potential x-update.
        z = (1 + beta) .* z - beta .* y;
        X = fft(z);
        nops(end)++;

        % check for a primary termination criterion.
        ok = 1;
        if (L >= Lf)
          break;
        end

        % check for a secondary termination criterion.
        [fnew, junk] = camera_functional(X, def);
        if (fnew > fX)
          % backtrack by a fixed factor of two.
          L = min(Lf, 2 * L);
          ok = 0;
        end
      until (ok == 1);

      % store the accepted new values.
      y = (z + beta .* y) ./ (1 + beta);
      x = z;
    end

    % update the operation count vector.
    nops = [nops; 0];
  end

  % truncate the reconstructed vector.
  x(N / 2 + 1 : end) = [];

  % truncate the operation count vector.
  nops(end) = [];
end
