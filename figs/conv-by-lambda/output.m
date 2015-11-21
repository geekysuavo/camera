args = argv();
x = load(args{1});
y = [x(:,1), x(:,2:end) ./ max(vec(x(:,2:end)))];
save('-ascii', args{2}, 'y');
