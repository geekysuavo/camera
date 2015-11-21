% get the input and output filenames.
args = argv();
infile = args{1};
outfile = args{2};

% add the extra source path.
addpath('../src');

% load the input file.
x = load(infile);

% set up the global parameters.
aim = [1e-9; 1e-6; linspace(1e-3, 5, 15)'];

% initialize the correlations matrix.
corrs = [];

% loop over the columns.
for i = 3 : columns(x)
  % save the correlation coefficient.
  corrs = [corrs; corr(x(:,2), x(:,i))];
end

% save the results.
results = [aim, corrs];
save('-ascii', outfile, 'results');
