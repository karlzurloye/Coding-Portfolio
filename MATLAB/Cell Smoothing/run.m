
clear;
close all;

% data loading
groundtruth = importdata('ds1_Groundtruth.dat',' ',4);
groundtruth_data = groundtruth.data;
visited = groundtruth_data(:,2:3);

% binary visitation grid initialization
visitation_grid = grid_init(visited);

% neighbor count grid initialization 
neighbor_count_grid = neighbor_count(visitation_grid,5);
safety_factor_grid = neighbor_count_grid/max(max(neighbor_count_grid));

% training set creation
[training_grid,removed_coords] = training(safety_factor_grid,70);
save('training_set.mat','training_grid');

% algorithm testing
algorithm_test();

% locally weighted linear regression
repaired_grid = lwlr(training_grid,2);

% neighbor count histogram
neighbor_hist(neighbor_count_grid);

% grid plotting 
binary_map(visitation_grid);
safety_map(safety_factor_grid);
title('Safety Factors of Cells');
safety_map(training_grid);
title('Training Dataset');
safety_map(repaired_grid);
title('Training Dataset Repaired by Locally Weighted Linear Regression');

% grid reparation error histogram
err_hist(repaired_grid,safety_factor_grid,removed_coords);







