function [grid, remove_coords] = training(grid, percent)

% percent = percent of full grid kept

% number of cells to remove from full grid
remove_num = round((100-percent)/100*size(grid,1)*size(grid,2));

% vector of all coordinates in grid
coords = zeros(size(grid,1)*size(grid,2),2);
index = 1;
for ii = 1:120
    for jj = 1:70
        coords(index,:) = [ii jj];
        index = index + 1;
    end 
end

% row and column indices to be removed
remove_coords = datasample(coords,remove_num,1,'Replace',false);

for kk = 1:remove_num
    % inf used as 'removed' placeholder
    grid(remove_coords(kk,1),remove_coords(kk,2)) = inf; 
end

