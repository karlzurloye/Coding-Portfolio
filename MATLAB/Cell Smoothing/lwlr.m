function [grid] = lwlr(grid, R)

% uses locally weighted linear regression to fill in missing cell values

% R = cell radius limitation for local weighting. Cell values are weighted
% according to a Gaussian distrobution with a standard deviation equal to a
% third of the cell radius limitation, thus, cells outside of this limit
% only contribute to 0.3% of the result. Ignoring these cells has minimal
% effect on the result while greatly increasing computation speed.

for ii = 1:size(grid,1)
    for jj = 1:size(grid,2)
        if grid(ii,jj) == inf % perform LWLR
            cell = [ii,jj];
            X = zeros(R*R,2); % initializing regression variables
            Y = zeros(R*R,1);
            W = zeros(R*R,1); 
            B0 = [0; 0];
            index = 1;
            for xx = -R:R
                for yy = -R:R
                    % checking that cell is in grid, within R, and not missing
                    if ((cell(1) + xx >= 1) && (cell(1) + xx <= 120) ...
                        && (cell(2) + yy >= 1) && (cell(2) + yy <= 70) ...
                        && (norm([xx yy]) <= R) ...
                        && (grid(cell(1)+xx, cell(2)+yy) ~= inf))
                            X(index,:) = [cell(1)+xx cell(2)+yy];
                            Y(index) = grid(cell(1)+xx, cell(2)+yy);
                            W(index) = exp(-(norm([xx yy])^2)/(2*(R/3)^2));
                            index = index + 1;
                    end
                end
            end
            X(index:end,:) = []; % removing extra preallocated rows
            Y(index:end) = [];
            W(index:end) = [];
            
            fun = @(B) ((X*B-Y).^2)'*W; % minimizing error with respect to B
            B = fminsearch(fun,B0);
            
            repair = [ii,jj]*B; % approximated value to replace missing value
            if repair < 0
                repair = 0;
            elseif repair > 1
                repair = 1;
            end
            grid(ii,jj) = repair; % inserting approximated missing value
        end
    end
end


