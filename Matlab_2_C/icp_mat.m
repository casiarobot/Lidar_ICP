function [Tt, Tr, ER, t] = icp_mat(q,p)

% 输入变量，因为原版的是通过inputParser，这个在Matlab coder中不识别，所以这边略去这个部分，直接给输入变量赋值
arg.Boundary = [];
arg.EdgeRejection = false;
arg.Extrapolation = true;		% false
arg.iter = 15;					% 迭代次数，默认10次
arg.Matching = 'bruteForce';	% 控制点匹配手法，注意这里只有暴力匹配能被Matlab Coder使用，其他手段都不能用
arg.Minimize = 'point';	
arg.Normals = [];
arg.p = p;
arg.q = q;
arg.ReturnAll = 0;
arg.Triangulation = [];
arg.Verbose = 0;
arg.Weight = @(x)ones(1,length(x));
arg.WorstRejection = 0;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Actual implementation 实际应用

% Allocate vector for RMS of errors in every iteration.
% 这个t是结果用来统计均方根误差用的，这里完全可以不要，因为在Matlab Coder内不识别
% t = zeros(arg.iter+1,1);
t = 0;							% 只好这里放弃，在C里面再增加值进行测试

% Start timer
% tic;

Np = size(p,2);

% Transformed data point cloud
pt = p;

% Allocate vector for RMS of errors in every iteration.
ER = zeros(arg.iter+1,1); 

% Initialize temporary transform vector and matrix.
T = zeros(3,1);
R = eye(3,3);

% Initialize total transform vector(s) and rotation matric(es).
TT = zeros(3,1, arg.iter+1);
TR = repmat(eye(3,3), [1,1, arg.iter+1]);
Tt = zeros(3, 1);			% 这两个变量是用于返回的，上面两个是过程变量
Tr = zeros(3, 3);			% 之所以这么做是因为Matlab在返回的时候会报Mismatch


% If edge vertices should be rejected, find edge vertices
if arg.EdgeRejection
    if isempty(arg.Boundary)
        bdr = find_bound(q, arg.Triangulation);
    else
        bdr = arg.Boundary;
    end
end

if arg.Extrapolation
    % Initialize total transform vector (quaternion ; translation vec.)
    qq = [ones(1,arg.iter+1);zeros(6,arg.iter+1)];   
    % Allocate vector for direction change and change angle.
    dq = zeros(7,arg.iter+1);
    theta = zeros(1,arg.iter+1);
end

%t(1) = toc;

% Go into main iteration loop
for k=1:arg.iter
       
    % Do matching
    switch arg.Matching
        case 'bruteForce'
            [match mindist] = match_bruteForce(q,pt);
%        case 'Delaunay'
%            [match mindist] = match_Delaunay(q,pt,DT);
%        case 'kDtree'
%            [match mindist] = match_kDtree(q,pt,kdOBJ);
	end
	
	% If matches to edge vertices should be rejected
    if arg.EdgeRejection
        p_idx = not(ismember(match, bdr));
        q_idx = match(p_idx);
        mindist = mindist(p_idx);
    else
        p_idx = true(1, Np);
        q_idx = match;
    end
    
    % If worst matches should be rejected
    if arg.WorstRejection
        edge = round((1-arg.WorstRejection)*sum(p_idx));
        pairs = find(p_idx);
        [~, idx] = sort(mindist);
        p_idx(pairs(idx(edge:end))) = false;
        q_idx = match(p_idx);
        mindist = mindist(p_idx);
	end
    
	if k == 1
        ER(k) = sqrt(sum(mindist.^2)/length(mindist));
	end
	
	switch arg.Minimize
        case 'point'
            % Determine weight vector
            weights = arg.Weight(match);
            [R,T] = eq_point(q(:,q_idx),pt(:,p_idx), weights(p_idx));
%        case 'plane'
%            weights = arg.Weight(match);
%            [R,T] = eq_plane(q(:,q_idx),pt(:,p_idx),arg.Normals(:,q_idx),weights(p_idx));
%        case 'lmaPoint'
%            [R,T] = eq_lmaPoint(q(:,q_idx),pt(:,p_idx));
	end
    % Add to the total transformation
    TR(:,:,k+1) = R*TR(:,:,k);
    TT(:,:,k+1) = R*TT(:,:,k)+T;

    % Apply last transformation
    pt = TR(:,:,k+1) * p + repmat(TT(:,:,k+1), 1, Np);
    
    % Root mean of objective function 
    ER(k+1) = rms_error(q(:,q_idx), pt(:,p_idx));
    
    % If Extrapolation, we might be able to move quicker
    if arg.Extrapolation
        qq(:,k+1) = [rmat2quat(TR(:,:,k+1));TT(:,:,k+1)];
        dq(:,k+1) = qq(:,k+1) - qq(:,k);
        theta(k+1) = (180/pi)*acos(dot(dq(:,k),dq(:,k+1))/(norm(dq(:,k))*norm(dq(:,k+1))));
        if arg.Verbose
            disp(['Direction change ' num2str(theta(k+1)) ' degree in iteration ' num2str(k)]);
        end
        if k>2 && theta(k+1) < 10 && theta(k) < 10
            d = [ER(k+1), ER(k), ER(k-1)];
            v = [0, -norm(dq(:,k+1)), -norm(dq(:,k))-norm(dq(:,k+1))];
            vmax = 25 * norm(dq(:,k+1));
            dv = extrapolate(v,d,vmax);
            if dv ~= 0
                q_mark = qq(:,k+1) + dv * dq(:,k+1)/norm(dq(:,k+1));
                q_mark(1:4) = q_mark(1:4)/norm(q_mark(1:4));
                qq(:,k+1) = q_mark;
                TR(:,:,k+1) = quat2rmat(qq(1:4,k+1));
                TT(:,:,k+1) = qq(5:7,k+1);
                % Reapply total transformation
                pt = TR(:,:,k+1) * p + repmat(TT(:,:,k+1), 1, Np);
                % Recalculate root mean of objective function
                % Note this is costly and only for fun!
                switch arg.Matching
                    case 'bruteForce'
                        [~, mindist] = match_bruteForce(q,pt);
%                    case 'Delaunay'
%                        [~, mindist] = match_Delaunay(q,pt,DT);
%                    case 'kDtree'
%                        [~, mindist] = match_kDtree(q,pt,kdOBJ);
                end
                ER(k+1) = sqrt(sum(mindist.^2)/length(mindist));
            end
        end
    end
    % t(k+1) = toc;
end

Tr = TR(:,:,end);
Tt = TT(:,:,end);
%if not(arg.ReturnAll)
%	% 原来这样在Matlab Coder里会报Mismatch，这边只好加入一个临时变量，用于返回
%   % 这段也弃用
%    TR = TR(:,:,end);
%    TT = TT(:,:,end);
%end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 暴力匹配

function [match mindist] = match_bruteForce(q, p)
    m = size(p,2);
    n = size(q,2);    
    match = zeros(1,m);
    mindist = zeros(1,m);
    for ki=1:m
        d=zeros(1,n);
        for ti=1:3
            d=d+(q(ti,:)-p(ti,ki)).^2;
        end
        [mindist(ki),match(ki)]=min(d);
    end
    
    mindist = sqrt(mindist);
	

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [R,T] = eq_point(q,p,weights)

m = size(p,2);
n = size(q,2);

% normalize weights
weights = weights ./ sum(weights);

% find data centroid and deviations from centroid
q_bar = q * transpose(weights);
q_mark = q - repmat(q_bar, 1, n);
% Apply weights
q_mark = q_mark .* repmat(weights, 3, 1);

% find data centroid and deviations from centroid
p_bar = p * transpose(weights);
p_mark = p - repmat(p_bar, 1, m);
% Apply weights
%p_mark = p_mark .* repmat(weights, 3, 1);

N = p_mark*transpose(q_mark); % taking points of q in matched order

[U,~,V] = svd(N); % singular value decomposition

R = V*diag([1 1 det(U*V')])*transpose(U);

T = q_bar - R*p_bar;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Extrapolation in quaternion space. Details are found in:
%
% Besl, P., & McKay, N. (1992). A method for registration of 3-D shapes. 
% IEEE Transactions on pattern analysis and machine intelligence, 239?256.

function [dv] = extrapolate(v,d,vmax)

p1 = polyfit(v,d,1); % linear fit
p2 = polyfit(v,d,2); % parabolic fit
v1 = -p1(2)/p1(1); % linear zero crossing
v2 = -p2(2)/(2*p2(1)); % polynomial top point

if issorted([0 v2 v1 vmax]) || issorted([0 v2 vmax v1])
    disp('Parabolic update!');
    dv = v2;
elseif issorted([0 v1 v2 vmax]) || issorted([0 v1 vmax v2])...
        || (v2 < 0 && issorted([0 v1 vmax]))
    disp('Line based update!');
    dv = v1;
elseif v1 > vmax && v2 > vmax
    disp('Maximum update!');
    dv = vmax;
else
    disp('No extrapolation!');
    dv = 0;
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Determine the RMS error between two point equally sized point clouds with
% point correspondance.
% ER = rms_error(p1,p2) where p1 and p2 are 3xn matrices.

function ER = rms_error(p1,p2)
dsq = sum(power(p1 - p2, 2),1);
ER = sqrt(mean(dsq));

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Converts (orthogonal) rotation matrices R to (unit) quaternion
% representations
% 
% Input: A 3x3xn matrix of rotation matrices
% Output: A 4xn matrix of n corresponding quaternions
%
% http://en.wikipedia.org/wiki/Rotation_matrix#Quaternion

function quaternion = rmat2quat(R)

Qxx = R(1,1,:);
Qxy = R(1,2,:);
Qxz = R(1,3,:);
Qyx = R(2,1,:);
Qyy = R(2,2,:);
Qyz = R(2,3,:);
Qzx = R(3,1,:);
Qzy = R(3,2,:);
Qzz = R(3,3,:);

w = 0.5 * sqrt(1+Qxx+Qyy+Qzz);
x = 0.5 * sign(Qzy-Qyz) .* sqrt(1+Qxx-Qyy-Qzz);
y = 0.5 * sign(Qxz-Qzx) .* sqrt(1-Qxx+Qyy-Qzz);
z = 0.5 * sign(Qyx-Qxy) .* sqrt(1-Qxx-Qyy+Qzz);

quaternion = reshape([w;x;y;z],4,[]);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Converts (unit) quaternion representations to (orthogonal) rotation matrices R
% 
% Input: A 4xn matrix of n quaternions
% Output: A 3x3xn matrix of corresponding rotation matrices
%
% http://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#From_a_quaternion_to_an_orthogonal_matrix

function R = quat2rmat(quaternion)

% 这里增加一个初始化，这边敢这么设置是因为这个东西本身就是二维的
q0 = zeros(1, 1, 1);
qx = zeros(1, 1, 1);
qy = zeros(1, 1, 1);
qz = zeros(1, 1, 1);

q0(1,1,:) = quaternion(1,:);
qx(1,1,:) = quaternion(2,:);
qy(1,1,:) = quaternion(3,:);
qz(1,1,:) = quaternion(4,:);

R = [q0.^2+qx.^2-qy.^2-qz.^2 2*qx.*qy-2*q0.*qz 2*qx.*qz+2*q0.*qy;
     2*qx.*qy+2*q0.*qz q0.^2-qx.^2+qy.^2-qz.^2 2*qy.*qz-2*q0.*qx;
     2*qx.*qz-2*q0.*qy 2*qy.*qz+2*q0.*qx q0.^2-qx.^2-qy.^2+qz.^2];
 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


