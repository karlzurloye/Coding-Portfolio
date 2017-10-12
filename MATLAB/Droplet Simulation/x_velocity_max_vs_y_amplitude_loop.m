%% Loading default parameters

clear variables
close all

Default_Input;

%% Alter any parameters here
% eg: global f; f = 22;

global f; f = 22;
global run_time; run_time = 1;

%% Loop and/or plotting Core results

global m1_ratio;
global m2_ratio;
global Az;
m1_loop = 0.1:0.1:0.9;
m2_loop = 0.9:-0.1:0.1;

for jj = 1:length(m1_loop)
    
m1_ratio = m1_loop(jj);
m2_ratio = m2_loop(jj);

Az_loop = 1E-3*(0.5:0.1:1.6);
max_x_vel = zeros(1,length(Az_loop));

    for ii = 1:length(Az_loop)
        Az = Az_loop(ii);
        data = Bouncing_Droplet_Simulator_Core;
        max_x_vel(ii) = data.horizontal_velocity(end);
    end

p = plot(Az_loop*1000,100*max_x_vel);
p(1).LineWidth = 2;
hold on

end

set(gca,'fontsize',20)
xlabel('vertical amplitude (mm)')
ylabel('horizontal velocity asymptote (cm/s)')

axis([0.5, 1.6, -0.5, 3])
legendCell = cellstr(num2str(m1_loop','%2.1f'));
legend(legendCell,'Location','eastoutside')
% print('file_name','-dpdf')
