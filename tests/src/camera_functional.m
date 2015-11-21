% camera_functional: compute the scalar sum and vector gradient
% of the hoch-hore entropy functional.
function [f, df] = camera_functional (X, d)
  % compute the scalar sum.
  f = sum(abs(X) .* ...
          log(abs(X)./(2*d) + sqrt(1 + (abs(X)./(2*d)).^2)) - ...
          sqrt(4*d*d + abs(X).^2));

  % compute the vector gradient.
  df = X .* log(abs(X)./(2*d) + sqrt(1 + (abs(X)./(2*d)).^2)) ./ abs(X);
end
