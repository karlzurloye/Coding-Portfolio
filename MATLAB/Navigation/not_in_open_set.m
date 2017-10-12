function [bool] = not_in_open_set (cell, open_set)

% checks if cell is listed in open set
bool = 1; 
for ii = 1:size(open_set,1)
    if  (round(open_set(ii,1)*10)/10 == round(cell(1)*10)/10) && ...
            (round(open_set(ii,2)*10)/10 == round(cell(2)*10)/10)
        bool = 0;
    end
end

end