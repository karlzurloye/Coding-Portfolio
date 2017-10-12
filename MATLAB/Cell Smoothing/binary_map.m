function [] = binary_map(grid)

figure;
hold on;

for ii = -2:0.1:4.9 % drawing grid cells
    for jj = -6:0.1:5.9
        pos = [ii jj 0.1 0.1];
        if cell_value([ii jj],grid) == 1
            rectangle('Position',pos,'FaceColor',[0 0 0.65],'LineWidth',0.1);
        else
            rectangle('Position',pos,'LineWidth',0.1);
        end
    end
end

title('Visited and Unvisited Cells');
xlabel('X Position (m)');
ylabel('Y Position (m)');


