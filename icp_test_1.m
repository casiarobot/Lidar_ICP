% http://blog.csdn.net/u013517182/article/details/53995993
% pcregrigid

clear; clc;

load('data_test_simple_rho.mat');

[tform, movingReg] = icp_simple(data, data_last);
