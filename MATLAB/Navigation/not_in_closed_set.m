function [bool] = not_in_closed_set (cell, closed_set)

% checks if cell is listed in closed set
bool = 1; 
for ii = 1:size(closed_set,1)
    if  (round(closed_set(ii,1)*10)/10 == round(cell(1)*10)/10) && ...
            (round(closed_set(ii,2)*10)/10 == round(cell(2)*10)/10)
        bool = 0;
    end
end

end