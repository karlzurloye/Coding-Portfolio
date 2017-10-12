function [bool] = is_occupied (cell, occ)

% checks if cell is occupied
bool = occ((6-cell(2)), (cell(1)+3));

end