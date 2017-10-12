function [pos, v, w] = disconnected_path_follow (x, y, theta, xt, yt, v, w, t)

a = 0.01; % distance from goal point to count as "there"
dt = 0.1;
pos = zeros(500,4); % [t(s) x(m) y(m) theta(rad)]
index = 1; % pos index

while pdist([x,y;xt,yt],'euclidean') > a
     [v,w] = inverse_kinematics(x,y,theta,xt,yt,v,w);
     [x,y,theta] = euler_step(x,y,theta,v,w,dt);
     t = t + dt;
     pos(index,:) = [t,x,y,theta];
     index = index + 1;
end

pos(~any(pos,2),:) = []; % removing zero rows

end






