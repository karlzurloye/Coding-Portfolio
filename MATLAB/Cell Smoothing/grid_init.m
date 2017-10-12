function [grid] = grid_init(visited)

% creates a matrix corresponding to x-y coordinate grid indicating whether
% cells have been visited or not. Cells are referred to by their bottom left
% coordinate.

% binary visitation grid (0.1m x 0.1m cells) (x = -2:5(m) y = -6:6(m))
grid = zeros(120,70); 
visited_coords = floor(visited*10)/10; % aligning with bottom left corner of grid cells
visited_coords(:,1) = visited_coords(:,1) + 2.1; % shifting positions to align 
visited_coords(:,2) = visited_coords(:,2) + 6.1; % with grid indices

for ii = 1:size(visited_coords,1) % marking visited cells in grid
    visited_cell = [121-visited_coords(ii,2)*10, visited_coords(ii,1)*10];
    grid(round(visited_cell(1)*10)/10, round(visited_cell(2)*10)/10) = 1;
end

end