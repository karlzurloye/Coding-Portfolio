function [pos] = path_follow (path)

a = 0.01; % distance from goal point to count as "there"
dt = 0.1;
v = 0; % (m/s) initial velocity
w = 0; % (rad/s) initial angular velocity
theta = -pi/2; % (rad) initial heading
x = path(1,1); % (m) initial x coordinate
y = path(1,2); % (m) initial y coordinate
t = 0; % (s) time
pos = zeros(10000,4); % [t(s) x(m) y(m) theta(rad)]
pos(1,:) = [0,x,y,theta];
index = 2; % pos index

for ii = 2:size(path,1) % path following loop
    while pdist([x,y;path(ii,1),path(ii,2)],'euclidean') > a
        [v,w] = inverse_kinematics(x,y,theta,path(ii,1),path(ii,2),v,w);
        [x,y,theta] = euler_step(x,y,theta,v,w,dt);
        t = t + dt;
        pos(index,:) = [t,x,y,theta];
        index = index + 1;
    end
end

pos(~any(pos,2),:) = []; % removing zero rows

end






