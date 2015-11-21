% ists_functional: compute the scalar sum and vector gradient
% of the stern ist functional.
function [f, df] = ists_functional (X, lambda)
  % compute the scalar sum.
  f = sum(abs(X));

  % compute the vector gradient.
  df = X .* (abs(X) > lambda);
  df = df .* (1 - lambda ./ abs(X));
end
