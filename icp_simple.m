function [tform, movingReg] = icp_simple(data_in, data_in_last)


% 转进来的数据先从极坐标换算到x, y
k = (64.0 - 255.0) / (6000.0 - 0);              % 灰度梯度，这里作点的时候不用固定值，距离越远的点噪声越大，用的灰度值就越小
b = 255;										% 用一次函数y = kx + b来拟合这个梯度

% 本次数据
[size_x, ~] = size(data_in);
data = zeros(size_x, 2);
gray = zeros(size_x, 1);
for i = 1 : size_x
   
   % 极坐标换算笛卡尔坐标
   theta = i * pi / 180;
   rho   = data_in(i);
   data(i, 1) = rho  * sin(theta);
   data(i, 2) = -rho * cos(theta);
   
   % 根据距离计算灰度值，距离越远灰度值越低，这样能够有效的降低图像的噪声
   if (rho ~= 0)                                % 不等于
       gray(i) = k * rho + b;
   else
       gray(i) = 0;
   end
    
end
% 上次数据
[size_x, ~] = size(data_in_last);
data_last = zeros(size_x, 2);
gray_last = zeros(size_x, 1);
for i = 1 : size_x
    
   theta = i * pi / 180;
   rho   = data_in_last(i);
   data_last(i, 1) = rho  * sin(theta);
   data_last(i, 2) = -rho * cos(theta);
   
   if (rho ~= 0)                                % 不等于
       gray_last(i) = k * rho + b;
   else
       gray_last(i) = 0;
   end
    
end
clear data_in, clear data_in_last;
clear i; clear size_x; clear rho;

% 作图，检验结果
% figure; scatter(data(:, 1), data(:, 2)); hold on; scatter(data_last(:, 1), data_last(:, 2));    % 别用plot

%  构造点云类
[size_x, ~] = size(data);
z_temp  = zeros(size_x, 1);
cloudNow  = pointCloud([data(:, 1), data(:, 2), z_temp]);
cloudNow.Color = [ uint8(gray), uint8(gray), uint8(gray) ];

[size_x, ~] = size(data_last);
z_temp  = zeros(size_x, 1);
cloudLast = pointCloud([data_last(:, 1), data_last(:, 2), z_temp]);
cloudLast.Color = [ uint8(gray_last), uint8(gray_last), uint8(gray_last) ];
clear size_x; clear size_y; clear z_temp;

% 进行ICP匹配
[tform, movingReg] = my_pcregrigid(cloudLast, cloudNow, 'Extrapolate', true);

% 作图，画出结果 
% figure; pcshow(cloudNow); hold on; pcshow(cloudLast); 
% pcsThowpair(cloudNow, movingReg);

return;