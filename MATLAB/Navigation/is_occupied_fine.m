function [bool] = is_occupied_fine (cell, occ)

% checks if cell is occupied
bool = occ(round((120-((cell(2)+6)*10))), round((10*(cell(1)+2))+1));

end