function [] = plotting_path (path_A, path_B, path_C, occ, ref_A, ref_B, ref_C)

figure;
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

p1 = plot(path_A(:,2),path_A(:,3)); % driving paths
p1.LineWidth = 5;
p2 = plot(path_B(:,2),path_B(:,3));
p2.LineWidth = 5;
p3 = plot(path_C(:,2),path_C(:,3));
p3.LineWidth = 5;
plot(ref_A(:,1),ref_A(:,2),'black'); % planned paths
plot(ref_B(:,1),ref_B(:,2),'black');
plot(ref_C(:,1),ref_C(:,2),'black');

xlabel('x (m)');
ylabel('y (m)');
title('Robot Paths');
legend('Path A','Path B','Path C','Planned Path');

figure; % plot with headings
hold on;
for ii = -2:4 % drawing grid cells
    for jj = -6:5
        pos = [ii jj 1 1];
        if is_occupied([ii jj],occ)
            rectangle('Position',pos,'FaceColor',[1 0 0],'LineWidth',3);
        end
    end
end

p1 = plot(path_A(:,2),path_A(:,3)); % driving paths
p1.LineWidth = 5;
p2 = plot(path_B(:,2),path_B(:,3));
p2.LineWidth = 5;
p3 = plot(path_C(:,2),path_C(:,3));
p3.LineWidth = 5;
plot(ref_A(:,1),ref_A(:,2),'black'); % planned paths
plot(ref_B(:,1),ref_B(:,2),'Color',[0 0.5 0]);

xlabel('x (m)');
ylabel('y (m)');
title('Robot Paths and Headings');
legend('Path A','Path B','Path C','Planned Path','Heading');

resolution = 1;
for ii = 1:size(path_A,1) % drawing heading lines
    if ii == round(ii/resolution)*resolution
        x = path_A(ii,2);
        y = path_A(ii,3);
        theta = path_A(ii,4);
        line([x,x+0.5*cos(theta)],[y,y+0.5*sin(theta)],'Color',[0 0.5 0]);
    end
end
for ii = 1:size(path_B,1) 
    if ii == round(ii/resolution)*resolution
        x = path_B(ii,2);
        y = path_B(ii,3);
        theta = path_B(ii,4);
        line([x,x+0.5*cos(theta)],[y,y+0.5*sin(theta)],'Color',[0 0.5 0]);
    end
end
for ii = 1:size(path_C,1)
    if ii == round(ii/resolution)*resolution
        x = path_C(ii,2);
        y = path_C(ii,3);
        theta = path_C(ii,4);
        line([x,x+0.5*cos(theta)],[y,y+0.5*sin(theta)],'Color',[0 0.5 0]);
    end
end

plot(ref_A(:,1),ref_A(:,2),'black'); % planned paths
plot(ref_B(:,1),ref_B(:,2),'black');
plot(ref_C(:,1),ref_C(:,2),'black');

end
