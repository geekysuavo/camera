args = argv();
fin = args{1};
fout = args{2};
leva = str2num(args{3});
levb = str2num(args{4});

cmd = ['pipe2txt.tcl ', fin];
[status, output] = system(cmd);

dat = str2num(output);
N = max(dat(:, 1 : 2));
X = reshape(dat(:, 3), N);

lev = logspace(log10(leva), log10(levb), 10);
C = contourc(X', lev);

G = [];
idx = 1;
idxmax = columns(C);

do
  n = C(2, idx);

  z = repmat(C(1, idx), n, 1);
  x = C(1, idx + 1 : idx + n)';
  y = C(2, idx + 1 : idx + n)';
  G = [G; [x, y, z]; nan(1, 3)];

  idx += n + 1;
until (idx >= idxmax)

G(end, :) = [];
save('-ascii', fout, 'G');

