function [v, w] = inverse_kinematics (x, y, theta, xt, yt, v_old, w_old)

vdot_max = 0.288; % max acceleration
wdot_max = 5.579; % max angular acceleration
dt = 0.1; % sampling rate (s)

dist = pdist([x,y;xt,yt],'euclidean'); % distance between position and goal

theta_t = atan2((yt-y),(xt-x)); % desired heading
dtheta = theta_t - theta; % discrepancy between desired and current heading
if dtheta > pi % compansating for pi to -pi transition of atan2
    dtheta = dtheta - 2*pi;
elseif dtheta < -pi
    dtheta = dtheta + 2*pi;
end

k_d = 0.1; % proportional gain for forward velocity
k_t = 1; % proportional gain for angular velocity

v = dist*k_d; % velocity (m/s)
w = dtheta*k_t; % angular velocity (rad/s)

if dtheta > 15/180*pi % no forward velocity if robot isn't facing right way
    v = 0;
end

if (v - v_old)/dt > vdot_max % limiting accleration
    v = v_old + vdot_max*dt;
elseif (v_old - v)/dt > vdot_max
    v = v_old - vdot_max*dt;
end
    
if (w - w_old)/dt > wdot_max % limiting angular acceleration
    w = w_old + wdot_max*dt;
elseif (w_old - w)/dt > vdot_max
    w = w_old - wdot_max*dt;
end

end