function [bool] = in_grid (cell)

% checks if cell is within grid bounds
bool = 1;
if ((cell(1) < -2) || (cell(1) >= 5))
    bool = 0;
end
if ((cell(2) < -6) || (cell(2) >= 6))
    bool = 0;
end
    
end