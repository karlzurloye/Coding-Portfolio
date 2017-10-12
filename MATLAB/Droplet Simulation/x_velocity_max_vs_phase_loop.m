%% Loading default parameters

clear variables
close all

Default_Input;

%% Alter any parameters here
% eg: global f; f = 22;

global f; f = 22;
global run_time; run_time = 1;

%% Loop and/or plotting Core results

global phase_angle;
global k;

k_loop = 1:0.5:5;

for jj = 1:length(k_loop)
    
k = k_loop(jj);
    
phase_angle_loop = linspace(0,360,10);
max_x_vel = zeros(1,length(phase_angle_loop));

    for ii = 1:length(phase_angle_loop)
        phase_angle = phase_angle_loop(ii);
        data = Bouncing_Droplet_Simulator_Core;
        max_x_vel(ii) = data.horizontal_velocity(end);
    end

p = plot(phase_angle_loop,100*max_x_vel);
p(1).LineWidth = 2;
hold on

end

set(gca,'fontsize',20)
xlabel('phase (degrees)')
ylabel('horizontal velocity asymptote (cm/s)')

%axis([0, 360, -0.5, 0.5])
legendCell = cellstr(num2str(k_loop','%2.1f'));
legend(legendCell,'Location','eastoutside')
% print('file_name','-dpdf')
