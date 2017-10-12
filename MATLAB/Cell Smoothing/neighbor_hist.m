function [] = neighbor_hist (neighbor_count_grid)

hist_data = reshape(neighbor_count_grid,[120*70,1]);
hist_data = hist_data(hist_data~=0); % removing multitude of zeros from empty space

figure;
histogram(hist_data);

xlabel('Number of Neighbors within Given Radius');
title('Neighbor Count Histogram');

