function [] = safety_map(grid)

figure;
hold on;
for ii = -2:0.1:4.9 % drawing grid cells
    for jj = -6:0.1:5.9
        safety_factor = cell_value([ii jj],grid);
        if safety_factor == inf % missing values visually represented as 0 safety factor
            safety_factor = 0; 
        end
        color = [1-safety_factor, 1-safety_factor, 0.5*(1-safety_factor)+0.5];
        pos = [ii jj 0.1 0.1];
        rectangle('Position',pos,'FaceColor',color,'LineWidth',0.1);
    end
end

xlabel('X Position (m)');
ylabel('Y Position (m)');