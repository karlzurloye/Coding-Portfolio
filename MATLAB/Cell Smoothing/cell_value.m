function [value] = cell_value(coord, grid)

% returns value of cell at (x,y) coordinate
value = grid(round((120-((coord(2)+6)*10))), round((10*(coord(1)+2))+1));

end