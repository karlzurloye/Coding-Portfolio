function [] = plotting (path_A, path_B, path_C, occ)

hold on;
for ii = -2:4 % drawing grid cells
    for jj = -6:5
        pos = [ii jj 1 1];
        if is_occupied([ii jj],occ)
            rectangle('Position',pos,'FaceColor',[1 0 0],'LineWidth',3);
        else
            rectangle('Position',pos,'LineWidth',3);
        end
    end
end

p1 = plot(path_A(:,1),path_A(:,2));
p1.LineWidth = 5;
p2 = plot(path_B(:,1),path_B(:,2));
p2.LineWidth = 5;
p3 = plot(path_C(:,1),path_C(:,2));
p3.LineWidth = 5;

xlabel('x (m)');
ylabel('y (m)');
title('Robot Paths');
legend('Path A','Path B','Path C');

end
