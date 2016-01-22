args = argv();
A = load(args{1});
B = load(args{2});
xmin = min(rows(A), rows(B));
x = [1 : xmin]';
y = [A(:,1), (A(:,2) - A(end,2)) ./ (A(1,2) - A(end,2))];
z = [B(:,1), (B(:,2) - B(end,2)) ./ (B(1,2) - B(end,2))];
dat = [x, y(1:xmin,:), z(1:xmin,:)];
save('-ascii', args{3}, 'dat');
