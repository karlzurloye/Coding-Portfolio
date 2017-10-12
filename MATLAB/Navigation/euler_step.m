function [x, y, theta] = euler_step (x_old, y_old, theta_old, v, w, dt)

dtheta = w * dt;
theta = theta_old + dtheta; % updating heading
    
dx = v * cos((theta+theta_old)/2) * dt; % calculating change in position
dy = v * sin((theta+theta_old)/2) * dt;
x = x_old + dx; % updating x and y coordinates
y = y_old + dy;

theta = theta + dtheta; % updating theta

end