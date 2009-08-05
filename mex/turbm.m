%
% Turbulence Database sample Matlab client code
%

authkey = 'edu.jhu.pha.turbulence.testing-200907';
dataset = 'isotropic1024coarse';

% ---- Temporal Interpolation Options ----
NoTInt   = 0; % No temporal interpolation
PCHIPInt = 1; % Piecewise cubic Hermit interpolation in time

% ---- Spatial Interpolation Flags for getVelocity &amp; getVelocityAndPressure ----
NoSInt = 0; % No spatial interpolation
Lag4   = 4; % 4th order Lagrangian interpolation in space
Lag6   = 6; % 6th order Lagrangian interpolation in space
Lag8   = 8; % 8th order Lagrangian interpolation in space

% ---- Spatial Differentiation &amp; Interpolation Flags for getVelocityGradient &amp; getPressureGradient ----
FD4NoInt = 40; % 4th order finite differential scheme for grid values, no spatial interpolation
FD6NoInt = 60; % 6th order finite differential scheme for grid values, no spatial interpolation
FD8NoInt = 80; % 8th order finite differential scheme for grid values, no spatial interpolation
FD4Lag4  = 44; % 4th order finite differential scheme for grid values, 4th order Lagrangian interpolation in space

timestep = 182;
time = 0.002 * timestep;

npoints = 10;

% For filter functions 
nLayers = 2;
dx = 2.0 * (3.14) / 1024;
filterLength = dx * 2.0^4.0;


points = zeros(3,npoints);
result1 = zeros(npoints);
result3 = zeros(3,npoints);
result4 = zeros(4,npoints);
result6 = zeros(6,npoints);
result9 = zeros(9,npoints);
result18 = zeros(18,npoints);

for p = 1:npoints
  points(1,p) = 0.20 * (p-1+1);
  points(2,p) = 0.50 * (p-1+1);
  points(3,p) = 0.15 * (p-1+1);
end

fprintf('\nRequesting velocity at 10 points...\n',npoints);
result3 =  getVelocity (authkey, dataset, time, Lag6, NoTInt, npoints, points);
for p = 1:npoints
  fprintf(1,'%i: %f, %f, %f\n', p, result3(1,p),  result3(2,p),  result3(3,p));
end

fprintf('\nRequesting forcing at %i points...\n',npoints);
result3 =  getForce (authkey, dataset, time, Lag6, NoTInt, npoints, points);
for p = 1:npoints
  fprintf(1,'%i: %f, %f, %f\n', p, result3(1,p),  result3(2,p),  result3(3,p));
end

fprintf('\nRequesting velocity and pressure at %i points...\n',npoints);
result4 = getVelocityAndPressure (authkey, dataset, time, Lag6, NoTInt, npoints, points);
for p = 1:npoints
  fprintf(1,'%i: %f, %f, %f, %f\n', p, result4(1,p),  result4(2,p),  result4(3,p), result4(4,p));
end


fprintf('\nRequesting velocity gradient at %i points...\n',npoints);
result9 = getVelocityGradient (authkey, dataset, time, FD4Lag4, NoTInt, npoints, points);
for p = 1:npoints
  fprintf(1,'%i: duxdx=%f, duxdy=%f, duxdz=%f, duydx=%f, duydy=%f, duydz=%f, duzdx=%f, duzdy=%f, duzdz=%f\n', p, ...
    result9(1,p), result9(2,p), result9(3,p), ...
    result9(4,p), result9(5,p), result9(6,p), ...
    result9(7,p), result9(8,p), result9(9,p));
end

fprintf('\nRequesting velocity hessian at %i points...\n',npoints);
result18 = getVelocityHessian (authkey, dataset, time, FD4Lag4, NoTInt, npoints, points);
for p = 1:npoints
  fprintf(1,'%i: d2uxdxdx=%f, d2uxdxdy=%f, d2uxdxdz=%f, d2uxdydy=%f, d2uxdydz=%f, d2uxdzdz=%f, d2uydxdx=%f, d2uydxdy=%f, d2uydxdz=%f, d2uydydy=%f, d2uydydz=%f, d2uydzdz=%f, d2uzdxdx=%f, d2uzdxdy=%f, d2uzdxdz=%f, d2uzdydy=%f, d2uzdydz=%f, d2uzdzdz=%f\n', p, ...
    result18(1,p), result18(2,p), result18(3,p), result18(4,p), result18(5,p), result18(6,p), ...
    result18(7,p), result18(8,p), result18(9,p), result18(10,p), result18(11,p), result18(12,p), ...
    result18(13,p), result18(14,p), result18(15,p), result18(16,p), result18(17,p), result18(18,p));
end

fprintf('\nRequesting velocity laplacian at %i points...\n',npoints);
result3 =  getVelocityLaplacian (authkey, dataset, time, FD4Lag4, NoTInt, npoints, points);
for p = 1:npoints
  fprintf(1,'%i: grad2ux=%f, grad2uy=%f, grad2uz=%f\n', p, result3(1,p),  result3(2,p),  result3(3,p));
end

fprintf('\nRequesting pressure gradient at %i points...\n',npoints);
result3 =  getPressureGradient (authkey, dataset, time, FD4Lag4, NoTInt, npoints, points);
for p = 1:npoints
  fprintf(1,'%i: dpdx=%f,dpdy=%f,dpdz=%f\n', p, result3(1,p),  result3(2,p),  result3(3,p));
end

fprintf('\nRequesting pressure hessian at %i points...\n',npoints);
result6 = getPressureHessian (authkey, dataset, time, FD4Lag4, NoTInt, npoints, points);
for p = 1:npoints
  fprintf(1,'%i: d2pdxdx=%f,d2pdxdy=%f,d2pdxdz=%f, d2pdydy=%f, d2pdydz=%f, d2pdzdz=%f\n', p, ...
    result6(1,p), result6(2,p), result6(3,p), ...
    result6(4,p), result6(5,p), result6(6,p));
end

fprintf('\nRequesting velocity box filter at 1 point...\n');
result3 = getBoxFilterVelocity(authkey, dataset, time, filterLength, nLayers, NoTInt, 1, points);
fprintf('Layer 1 (filter length = %f) :\n', filterLength);
fprintf('Location: x=%f, y=%f, z=%f\n', points(1,1), points(2,1), points(3,1));
fprintf('ux=%f,uy=%f,uz=%f\n', result3(1,1), result3(2,1), result3(3,1));
fprintf('Layer 2 (filter length = %f) :\n', filterLength/2.0); 
  for p=2:npoints-1
   if (p == 2 || p == 4 || p == 6 || p == 8) 
      xp = points(1,1) - filterLength/4.0;
   else 
     xp = points(1,1) + filterLength/4.0;
   end
   if (p == 2 || p == 3 || p == 6 || p == 7)
     yp = points(2,1) - filterLength/4.0;
   else
     yp = points(2,1) + filterLength/4.0;
   end
   if (p == 2 || p == 3 || p == 4 || p == 5)
     zp = points(3,1) - filterLength/4.0;
   else
     zp = points(3,1)+ filterLength/4.0;
   end
   fprintf('%i: Location: x=%f, y=%f, z=%f\n', p, xp, yp, zp); 
   fprintf('   ux=%f,uy=%f,uz=%f\n', result3(1,p),  result3(2,p),  result3(3,p));
  end
  
fprintf('\nRequesting pressure box filter at 1 point...\n');
result1 = getBoxFilterPressure(authkey, dataset, time, filterLength, nLayers, ...
  NoTInt, 1, points);
fprintf('Layer 1 (filter length = %f) :\n', filterLength);
fprintf('Location: x=%f, y=%f, z=%f\n', points(1,1), points(2,1), points(3,1));
fprintf('p=%f\n', result1(1));
fprintf('Layer 2 (filter length = %f) :\n', filterLength/2.0); 
  for p=2:npoints-1
   if (p == 2 || p == 4 || p == 6 || p == 8) 
      xp = points(1,1) - filterLength/4.0;
   else 
     xp = points(1,1) + filterLength/4.0;
   end
   if (p == 2 || p == 3 || p == 6 || p == 7)
     yp = points(2,1) - filterLength/4.0;
   else
     yp = points(2,1) + filterLength/4.0;
   end
   if (p == 2 || p == 3 || p == 4 || p == 5)
     zp = points(3,1) - filterLength/4.0;
   else
     zp = points(3,1)+ filterLength/4.0;
   end
   fprintf('%i: Location: x=%f, y=%f, z=%f\n', p, xp, yp, zp); 
   fprintf('   p=%f\n', result1(p));
  end
  
fprintf('\nRequesting SGS stress box filter at 1 point...\n');
result6 = getBoxFilterSGSStress(authkey, dataset, time, filterLength, nLayers, NoTInt, 1, points);
fprintf('Layer 1 (filter length = %f) :\n', filterLength);
fprintf('Location: x=%f, y=%f, z=%f\n', points(1,1), points(2,1), points(3,1));
fprintf('xx=%f,yy=%f,zz=%f, xy=%f, xz=%f, yz=%f\n', ...
    result6(1,1), result6(2,1), result6(3,1), result6(4,1), result6(5,1), result6(6,1));

fprintf('Layer 2 (filter length = %f) :\n', filterLength/2.0); 
  for p=2:npoints-1
   if (p == 2 || p == 4 || p == 6 || p == 8) 
      xp = points(1,1) - filterLength/4.0;
   else 
     xp = points(1,1) + filterLength/4.0;
   end
   if (p == 2 || p == 3 || p == 6 || p == 7)
     yp = points(2,1) - filterLength/4.0;
   else
     yp = points(2,1) + filterLength/4.0;
   end
   if (p == 2 || p == 3 || p == 4 || p == 5)
     zp = points(3,1) - filterLength/4.0;
   else
     zp = points(3,1)+ filterLength/4.0;
   end
   fprintf('%i: Location: x=%f, y=%f, z=%f\n', p, xp, yp, zp); 
   fprintf('      xx=%f,yy=%f,zz=%f, xy=%f, xz=%f, yz=%f\n', ...
       result6(1,p), result6(2,p), result6(3,p), result6(4,p), result6(5,p), result6(6,p));
  end  
  
fprintf('\nRequesting velocity gradient box filter at 1 points...\n');
result9 = getBoxFilterVelocityGradient(authkey, dataset, time, filterLength, nLayers, NoTInt, 1, points);
fprintf('Layer 1 (filter length = %f) :\n', filterLength);
fprintf('Location: x=%f, y=%f, z=%f\n', points(1,1), points(2,1), points(3,1));
fprintf('duxdx=%f, duxdy=%f, duxdz=%f, duydx=%f, duydy=%f, duydz=%f, duzdx=%f, duzdy=%f, duzdz=%f\n', ...
    result9(1,1), result9(2,1), result9(3,1), ...
    result9(4,1), result9(5,1), result9(6,1), ...
    result9(7,1), result9(8,1), result9(9,1));

fprintf('Layer 2 (filter length = %f) :\n', filterLength/2.0); 
for p=2:npoints-1
 if (p == 2 || p == 4 || p == 6 || p == 8) 
    xp = points(1,1) - filterLength/4.0;
 else 
   xp = points(1,1) + filterLength/4.0;
 end
 if (p == 2 || p == 3 || p == 6 || p == 7)
   yp = points(2,1) - filterLength/4.0;
 else
   yp = points(2,1) + filterLength/4.0;
 end
 if (p == 2 || p == 3 || p == 4 || p == 5)
   zp = points(3,1) - filterLength/4.0;
 else
   zp = points(3,1)+ filterLength/4.0;
 end
 fprintf('%i: Location: x=%f, y=%f, z=%f\n', p, xp, yp, zp); 
 fprintf('      duxdx=%f, duxdy=%f, duxdz=%f, duydx=%f, duydy=%f, duydz=%f, duzdx=%f, duzdy=%f, duzdz=%f\n', ...
    result9(1,p), result9(2,p), result9(3,p), ...
    result9(4,p), result9(5,p), result9(6,p), ...
    result9(7,p), result9(8,p), result9(9,p));
end  

