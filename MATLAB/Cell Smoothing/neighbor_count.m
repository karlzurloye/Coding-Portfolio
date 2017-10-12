function [count_grid] = neighbor_count(visitation_grid, radius)

% radius = cell radius for counting

count_grid = zeros(size(visitation_grid));

% counting neighbors for each cell
for ii = 1:size(visitation_grid,1)
    for jj = 1:size(visitation_grid,2)
        cell = [ii,jj];
        for xx = -radius:radius
            for yy = -radius:radius
                % checking that cell is in grid, within R, and was visited
                if ((cell(1) + xx >= 1) && (cell(1) + xx <= 120) ...
                   && (cell(2) + yy >= 1) && (cell(2) + yy <= 70) ...
                   && (norm([xx yy]) <= radius) ... 
                   && (visitation_grid(cell(1)+xx, cell(2)+yy) == 1))
                        count_grid(ii,jj) = count_grid(ii,jj) + 1;
                end
            end
        end
    end    
end 



