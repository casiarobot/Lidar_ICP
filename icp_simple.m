function [tform, movingReg] = icp_simple(data_in, data_in_last)


% ת�����������ȴӼ����껻�㵽x, y
k = (64.0 - 255.0) / (6000.0 - 0);              % �Ҷ��ݶȣ����������ʱ���ù̶�ֵ������ԽԶ�ĵ�����Խ���õĻҶ�ֵ��ԽС
b = 255;										% ��һ�κ���y = kx + b���������ݶ�

% ��������
[size_x, ~] = size(data_in);
data = zeros(size_x, 2);
gray = zeros(size_x, 1);
for i = 1 : size_x
   
   % �����껻��ѿ�������
   theta = i * pi / 180;
   rho   = data_in(i);
   data(i, 1) = rho  * sin(theta);
   data(i, 2) = -rho * cos(theta);
   
   % ���ݾ������Ҷ�ֵ������ԽԶ�Ҷ�ֵԽ�ͣ������ܹ���Ч�Ľ���ͼ�������
   if (rho ~= 0)                                % ������
       gray(i) = k * rho + b;
   else
       gray(i) = 0;
   end
    
end
% �ϴ�����
[size_x, ~] = size(data_in_last);
data_last = zeros(size_x, 2);
gray_last = zeros(size_x, 1);
for i = 1 : size_x
    
   theta = i * pi / 180;
   rho   = data_in_last(i);
   data_last(i, 1) = rho  * sin(theta);
   data_last(i, 2) = -rho * cos(theta);
   
   if (rho ~= 0)                                % ������
       gray_last(i) = k * rho + b;
   else
       gray_last(i) = 0;
   end
    
end
clear data_in, clear data_in_last;
clear i; clear size_x; clear rho;

% ��ͼ��������
% figure; scatter(data(:, 1), data(:, 2)); hold on; scatter(data_last(:, 1), data_last(:, 2));    % ����plot

%  ���������
[size_x, ~] = size(data);
z_temp  = zeros(size_x, 1);
cloudNow  = pointCloud([data(:, 1), data(:, 2), z_temp]);
cloudNow.Color = [ uint8(gray), uint8(gray), uint8(gray) ];

[size_x, ~] = size(data_last);
z_temp  = zeros(size_x, 1);
cloudLast = pointCloud([data_last(:, 1), data_last(:, 2), z_temp]);
cloudLast.Color = [ uint8(gray_last), uint8(gray_last), uint8(gray_last) ];
clear size_x; clear size_y; clear z_temp;

% ����ICPƥ��
[tform, movingReg] = my_pcregrigid(cloudLast, cloudNow, 'Extrapolate', true);

% ��ͼ��������� 
% figure; pcshow(cloudNow); hold on; pcshow(cloudLast); 
% pcsThowpair(cloudNow, movingReg);

return;