function [] = err_hist (repaired_grid, safety_factor_grid, removed_coords)

error = zeros(size(removed_coords,1),1);

for ii = 1:length(error) % comparing removed value to LWLR value
    row = removed_coords(ii,1);
    col = removed_coords(ii,2);
    error(ii) = 100*abs(repaired_grid(row,col) - safety_factor_grid(row,col));
end

error = error(error~=0); % removing trivial LWLR repairs in empty space
figure;
histogram(error);

xlabel('Error (%)');
title('Locally Weighted Linear Regression Error Histogram');

fprintf('\nmedian error: %5.3f%%\nmax error: %5.3f%%\n\n',median(error),max(error));