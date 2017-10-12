function [path] = path_simulation (start, goal, occ)

% variable and matrix initialization
current_cell = floor(start);
goal_cell = floor(goal);
closed_set = current_cell;
path = zeros(250,2); % tracking position for plotting
path_index = 1;
open_set = []; % [x(m) y(m) h(n)]

% main loop
while ~((current_cell(1) == goal_cell(1)) && (current_cell(2) == goal_cell(2)))
        
    for ii = -1:1
        for jj = -1:1
            cell = current_cell + [ii jj];
            % checking if adjascent cells are valid for the open set
            if (in_grid(cell) && not_in_closed_set(cell,closed_set))
                % heuristic based on straight-line distance
                h = 1+1/sqrt(2)*sqrt((goal_cell(2)-cell(2))^2+(goal_cell(1)-cell(1))^2);
                if is_occupied(cell,occ) % true cost = 1000 for occupied cells
                    h = h + 999;
                end
                open_set = [open_set; [cell h]];
            end   
        end
    end
    
    % tracking position for plotting
    path(path_index,:) = (current_cell + 0.5);
    path_index = path_index + 1;
    
    % moving to cell with lowest heuristic
    new_index = find(open_set(:,3) == min(open_set(:,3)),1);
    current_cell = open_set(new_index,1:2);
    open_set(new_index,:) = []; % removing new cell from open_set    
    
    % blocking future expansion of current cell
    closed_set = [closed_set; current_cell];
    
end

path(path_index,:) = (current_cell + 0.5);
path(path_index+1:end,:) = []; % removing extra preallocated rows

end