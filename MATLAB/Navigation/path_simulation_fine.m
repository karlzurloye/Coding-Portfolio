function [path] = path_simulation_fine (start, goal, occ)

% variable and matrix initialization
current_cell = floor(start*10)/10;
goal_cell = floor(goal*10)/10;
closed_set = current_cell;
path = zeros(250,2); % tracking position for plotting
path_index = 1;
open_set = []; % [x(m) y(m) h(n)]

% main loop
while ~((round(current_cell(1)*10)/10 == round(goal_cell(1)*10)/10) && ...
        (round(current_cell(2)*10)/10 == round(goal_cell(2)*10)/10))
            
    for ii = -0.1:0.1:0.1
        for jj = -0.1:0.1:0.1
            cell = current_cell + [ii jj];
            % checking if adjascent cells are valid for the open set
            if (in_grid(cell) && not_in_closed_set(cell,closed_set))
                % heuristic based on straight-line distance
                h = 1+10/sqrt(2)*sqrt((goal_cell(2)-cell(2))^2+(goal_cell(1)-cell(1))^2);
                if is_occupied_fine(cell,occ) % true cost = 1000 for occupied cells
                    h = h + 999;
                end
                open_set = [open_set; [cell h]];
            end   
        end
    end

    % tracking position for plotting
    path(path_index,:) = (current_cell + 0.05);
    path_index = path_index + 1;
    
    % moving to cell with lowest heuristic
    new_index = find(open_set(:,3) == min(open_set(:,3)),1);
    current_cell = open_set(new_index,1:2);
    open_set(new_index,:) = []; % removing new cell from open_set
    
    % blocking future expansion of current cell
    closed_set = [closed_set; round(current_cell*10)/10];
         
end

path(path_index,:) = (current_cell + 0.05);
path(path_index+1:end,:) = []; % removing extra preallocated rows

end