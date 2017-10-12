function [occ] = grid_init (obs)

% creates a matrix corresponding to x-y coordinate grid indicating whether
% cells are occupied or not. Cells are referred to by their bottom left
% coordinate.

occ = zeros(12,7); % binary occupation grid (1m x 1m cells) (x = -2:5(m) y = -6:6(m))
obs_pos = floor(obs); % aligning with bottom left corner of grid cells
obs_pos(:,1) = obs_pos(:,1) + 3; % shifting positions to align with grid indices
obs_pos(:,2) = obs_pos(:,2) + 7;

for ii = 1:size(obs_pos,1) % marking occupancies in grid
    occ(13 - obs_pos(ii,2), obs_pos(ii,1)) = 1;
end

end