function [occ] = grid_init_fine (obs)

% creates a matrix corresponding to x-y coordinate grid indicating whether
% cells are occupied or not. Cells are referred to by their bottom left
% coordinate.

% binary occupation grid (0.1m x 0.1m cells) (x = -2:5(m) y = -6:6(m))
occ = zeros(120,70); 
obs_pos = floor(obs*10)/10; % aligning with bottom left corner of grid cells
obs_pos(:,1) = obs_pos(:,1) + 2.1; % shifting positions to align with grid indices
obs_pos(:,2) = obs_pos(:,2) + 6.1;

for ii = 1:size(obs_pos,1) % marking occupancies in grid
    obs_center = [121-obs_pos(ii,2)*10, obs_pos(ii,1)*10];
    for jj = -3:3 % filling in nearby cells to compensate for robot radius
        for kk = -3:3
            if ((obs_center(1) + jj >= 1) && (obs_center(1) + jj <= 120) ...
                    && (obs_center(2) + kk >= 1) && (obs_center(2) + kk <= 70))
                occ(round(obs_center(1))+jj, round(obs_center(2)+kk)) = 1;
            end
        end
    end
end
    
end