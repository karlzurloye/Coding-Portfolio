function [pos] = path_gen_follow (start, goal, occ)

% variable and matrix initialization
current_cell = floor(start);
goal_cell = floor(goal);
closed_set = current_cell;
open_set = []; % [x(m) y(m) h(n)]
pos = zeros(1000,4); % [t(s) x(m) y(m) theta(rad)]
v = 0; % (m/s) initial velocity
w = 0; % (rad/s) initial angular velocity
theta = -pi/2; % (rad) initial heading
x = start(1); % (m) initial x coordinate
y = start(2); % (m) initial y coordinate
t = 0; % (s) time
pos(1,:) = [0,x,y,theta];
index = 2; % pos index

% main loop
while ~((current_cell(1) == goal_cell(1)) && (current_cell(2) == goal_cell(2)))
            
    for ii = -1:1
        for jj = -1:1
            cell = current_cell + [ii jj];
            % checking if adjascent cells are valid for the open set
            if (in_grid(cell) && not_in_closed_set(cell,closed_set)...
                    && not_in_open_set(cell,open_set))
                % heuristic based on straight-line distance
                h = 1+1/sqrt(2)*sqrt((goal_cell(2)-cell(2))^2+(goal_cell(1)-cell(1))^2);
                if is_occupied(cell,occ) % true cost = 1000 for occupied cells
                    h = h + 999;
                end
                open_set = [open_set; [cell h]];
            end   
        end
    end
    
    % moving to cell with lowest heuristic
    new_index = find(open_set(:,3) == min(open_set(:,3)),1);
    current_cell = open_set(new_index,1:2);
    open_set(new_index,:) = []; % removing new cell from open_set
      
    % blocking future expansion of current cell
    closed_set = [closed_set; round(current_cell*10)/10];
         
    % moving to new cell
    [cell_change_path, v, w] = disconnected_path_follow...
        (x,y,theta,current_cell(1)+0.5,current_cell(2)+0.5,v,w,t);
    t = cell_change_path(end,1);
    x = cell_change_path(end,2);
    y = cell_change_path(end,3);
    theta = cell_change_path(end,4);
    L = size(cell_change_path,1);
    pos(index:index+L-1,:) = cell_change_path; % adding path to overall path
    index = index + L;
        
end

pos(~any(pos,2),:) = []; % removing zero rows

end





