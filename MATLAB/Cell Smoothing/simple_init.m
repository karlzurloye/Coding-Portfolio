function [grid] = simple_init()

% creates a matrix the same size as the safety factor grid but with a
% simple distrobution to test the LWLR algorithm

grid = zeros(120,70); 

for ii = 1:120 % mapping sine waves over grid
    for jj = 1:70
        grid(ii,jj) = (sin(ii/120*8*pi) + sin(jj/70*5*pi) + 2)/4;
    end
end

