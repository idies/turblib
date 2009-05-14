program TurbTest
  implicit none

  ! ---- Temporal Interpolation Options ----
  integer, parameter :: NoTInt = 0 ! No temporal interpolation
  integer, parameter :: PCHIPInt = 1 ! Piecewise cubic Hermit interpolation in time

  ! ---- Spatial Interpolation Flags for GetVelocity & GetVelocityAndPressure ----
  integer, parameter :: NoSInt = 0 ! No spatial interpolation
  integer, parameter :: Lag4 = 4 ! 4th order Lagrangian interpolation in space
  integer, parameter :: Lag6 = 6 ! 6th order Lagrangian interpolation in space
  integer, parameter :: Lag8 = 8 ! 8th order Lagrangian interpolation in space

  ! ---- Spatial Differentiation & Interpolation Flags for GetVelocityGradient & GetPressureGradient ----
  integer, parameter :: FD4NoInt = 40 ! 4th order finite differential scheme for grid values, no spatial interpolation
  integer, parameter :: FD6NoInt = 60 ! 6th order finite differential scheme for grid values, no spatial interpolation
  integer, parameter :: FD8NoInt = 80 ! 8th order finite differential scheme for grid values, no spatial interpolation
  integer, parameter :: FD4Lag4 = 44 ! 4th order finite differential scheme for grid values, 4th order Lagrangian interpolation in space

  !
  ! Choose which dataset to use in this query
  ! Currently, only valid datasets are:
  !   'isotropic1024fine' and 'isotropic1024coarse'
  !
  character*100 :: dataset = 'isotropic1024coarse' // CHAR(0)

  !
  ! Specify your access key here.
  ! If you need one, please visit http://turbulence.pha.jhu.edu/
  ! (We just want to know a bit about our users!)
  !
  character*100 :: authkey = 'edu.jhu.pha.turbulence.testing-200905' // CHAR(0)

  integer, parameter :: timestep = 182
  real :: time = 0.002 * timestep
  real points(3, 10)    ! input
  real dataout1(10)     ! p
  real dataout3(3, 10)  ! x,y,z
  real dataout4(4, 10)  ! x,y,z,p
  real dataout6(6, 10)  ! results from Pressure Hessian
  real dataout9(9, 10)  ! results from Velocity Gradient
  real dataout18(18, 10) ! results from Velocity Hessian

  integer i,rc
  real xp,yp,zp

  integer :: nLayers = 2
  real :: dx = 2.0 * 3.14 / 1024.0;
  real :: filterLength = (2.0 * 3.14 / 1024.0) * 16.0;  ! 2.0^4.0=16.0

  !
  ! Intialize the gSOAP runtime.
  ! This is required before any WebService routines are called.
  !
  CALL soapinit()

  do i = 1, 10, 1
    points(1, i) = 0.20 * i
    points(2, i) = 0.50 * i
    points(3, i) = 0.15 * i 
  end do

  write(*,*) 'Velocity at 10 particle locations'
  CALL getvelocity(authkey, dataset,  time, Lag6, NoTInt, 10, points, dataout3)
  do i = 1, 10, 1 
    write(*,*) i, ': (', dataout3(1,i), ', ', dataout3(2,i), ', ', dataout3(3,i), ')'
  end do

  write(*,*) 'Forcing at 10 particle locations'
  CALL getforce(authkey, dataset,  time, Lag6, NoTInt, 10, points, dataout3)
  do i = 1, 10, 1 
    write(*,*) i, ': (', dataout3(1,i), ', ', dataout3(2,i), ', ', dataout3(3,i), ')'
  end do


  write(*,*)
  write(*,*) 'Velocity and pressure at 10 particle locations'
  CALL getvelocityandpressure(authkey, dataset,  time, Lag6, NoTInt, 10, points, dataout4)
  do i = 1, 10, 1 
    write(*,*) i, ': (', dataout4(1,i), ', ', dataout4(2,i), ', ', dataout4(3,i), ', ', dataout4(4,i), ')'
  end do

  write(*,*)
  write(*,*) 'Velocity gradient at 10 particle locations'
  CALL getvelocitygradient(authkey, dataset,  time, FD4Lag4, NoTInt, 10, points, dataout9)
  do i = 1, 10, 1 
    write(*,*) i, ': (duxdx=', dataout9(1,i), ', duxdy=', dataout9(2,i), &
       ', duxdz=', dataout9(3,i), ', duydx=', dataout9(4,i),  &
       ', duydy=', dataout9(5,i), ', duydz=', dataout9(6,i),  &
       ', duzdx=', dataout9(7,i), ', duzdy=', dataout9(8,i),  &
       ', duzdz=', dataout9(9,i), ')'
  end do

  write(*,*) 'Velocity laplacian at 10 particle locations'
  CALL getvelocitylaplacian(authkey, dataset,  time, FD4Lag4, NoTInt, 10, points, dataout3)
  do i = 1, 10, 1 
    write(*,*) i, ': (grad2ux=', dataout3(1,i), ', grad2uy=', dataout3(2,i), ', grad2uz=', dataout3(3,i), ')'
  end do

  write(*,*) 'Velocity hessian at 10 particle locations'
  CALL getvelocityhessian(authkey, dataset,  time, FD4Lag4, NoTInt, 10, points, dataout18)
  do i = 1, 10, 1 
    write(*,*) i, ': (d2uxdxdx=', dataout18(1,i), &
       ', d2uxdxdy=', dataout18(2,i), &
       ', d2uxdxdz=', dataout18(3,i), &
       ', d2uxdydy=', dataout18(4,i), &
       ', d2uxdydz=', dataout18(5,i), &
       ', d2uxdzdz=', dataout18(6,i), &
       ', d2uydxdx=', dataout18(7,i), &
       ', d2uydxdy=', dataout18(8,i), &
       ', d2uydxdz=', dataout18(9,i), &
       ', d2uydydy=', dataout18(10,i), &
       ', d2uydydz=', dataout18(11,i), &
       ', d2uydzdz=', dataout18(12,i), &
       ', d2uzdxdx=', dataout18(13,i), &
       ', d2uzdxdy=', dataout18(14,i), &
       ', d2uzdxdz=', dataout18(15,i), &
       ', d2uzdydy=', dataout18(16,i), &
       ', d2uzdydz=', dataout18(18,i), &
       ', d2uzdzdz=', dataout18(18,i), ')'
  end do

  write(*,*)
  write(*,*) 'Pressure gradient at 10 particle locations'
  CALL getpressuregradient(authkey, dataset,  time, FD4Lag4, NoTInt, 10, points, dataout3)
  do i = 1, 10, 1 
    write(*,*) i, ': (dpdx=', dataout3(1,i), ', dpdy=', dataout3(2,i), ', dpdz=', dataout3(3,i), ')'
  end do

  write(*,*)
  write(*,*) 'Pressure hessian at 10 particle locations'
  CALL getpressurehessian(authkey, dataset,  time, FD4Lag4, NoTInt, 10, points, dataout6)
  do i = 1, 10, 1 
    write(*,*) i, ': (d2pdxdx=', dataout6(1,i), ', d2pdxdy=', dataout6(2,i), &
       ', d2pdxdz=', dataout6(3,i), ', d2pdydy=', dataout6(4,i),  &
       ', d2pdydz=', dataout6(5,i), ', d2pdzdz', dataout6(6,i), ')'
  end do

  write(*,*)
  write(*,*) 'Velocity box filter at 1 particle location'
  CALL getboxfiltervelocity(authkey, dataset, time, filterLength, nLayers, NoTInt, 1, points, dataout3)
  write(*,*) 'Layer 1: (filter length = ', filterLength, ')'
  write(*,*) 1, ': Location: (', points(1,1), ', ', points(2,1), ', ', points(3,1), ')'
  write(*,*) '           (', dataout3(1,1), ', ', dataout3(2,1), ', ', dataout3(3,1), ')'
  write(*,*) 'Layer 2: (filter length = ', filterLength/2.0, ')'
  do i = 2, 9, 1 
    if ( i==2 .OR. i==4 .OR. i==6 .OR. i==8 ) then
      xp = points(1,1)-filterLength/4.0
    else
      xp = points(1,1)+filterLength/4.0
    endif
    if ( i==2 .OR. i==3 .OR. i==6 .OR. i==7 ) then
      yp = points(2,1)-filterLength/4.0
    else
      yp = points(2,1)+filterLength/4.0
    endif
    if ( i==2 .OR. i==3 .OR. i==4 .OR. i==5 ) then
      zp = points(3,1)-filterLength/4.0
    else
      zp = points(3,1)+filterLength/4.0
    endif
    write(*,*) i-1, ': Location: (', xp, ', ', yp, ', ', zp, ')'
    write(*,*) '           (', dataout3(1,i), ', ', dataout3(2,i), ', ', dataout3(3,i), ')'
  end do

  write(*,*)
  write(*,*) 'Pressure box filter at 1 particle location'
  CALL getboxfilterpressure(authkey, dataset, time, filterLength, nLayers, NoTInt, 1, points, dataout1)
  write(*,*) 'Layer 1: (filter length = ', filterLength, ')'
  write(*,*) 1, ': Location: (', points(1,1), ', ', points(2,1), ', ', points(3,1), ')'
  write(*,*) '           (', dataout1(1), ')'
  write(*,*) 'Layer 2: (filter length = ', filterLength/2.0, ')'
  do i = 2, 9, 1
    if ( i==2 .OR. i==4 .OR. i==6 .OR. i==8 ) then
      xp = points(1,1)-filterLength/4.0
    else
      xp = points(1,1)+filterLength/4.0
    endif
    if ( i==2 .OR. i==3 .OR. i==6 .OR. i==7 ) then
      yp = points(2,1)-filterLength/4.0
    else
      yp = points(2,1)+filterLength/4.0
    endif
    if ( i==2 .OR. i==3 .OR. i==4 .OR. i==5 ) then
      zp = points(3,1)-filterLength/4.0
    else
      zp = points(3,1)+filterLength/4.0
    endif
    write(*,*) i-1, ': Location: (', xp, ', ', yp, ', ', zp, ')'
    write(*,*) '           (', dataout1(i), ')'
  end do
  
  write(*,*)
  write(*,*) 'SGSstress box filter at 1 particle location'
  CALL getboxfiltersgsstress(authkey, dataset, time, filterLength, nLayers, NoTInt, 1, points, dataout6)
  write(*,*) 'Layer 1: (filter length = ', filterLength, ')'
  write(*,*) 1, ': Location: (', points(1,1), ', ', points(2,1), ', ', points(3,1), ')'
  write(*,*) '           (xx =', dataout6(1,1), ', yy =', dataout6(2,1), ', zz =', dataout6(3,1), &
                       ', xy =', dataout6(4,1), ', xz =', dataout6(5,1), ', yz =', dataout6(6,1), ')'
  write(*,*) 'Layer 2: (filter length = ', filterLength/2.0, ')'
  do i = 2, 9, 1
    if ( i==2 .OR. i==4 .OR. i==6 .OR. i==8 ) then
      xp = points(1,1)-filterLength/4.0
    else
      xp = points(1,1)+filterLength/4.0
    endif
    if ( i==2 .OR. i==3 .OR. i==6 .OR. i==7 ) then
      yp = points(2,1)-filterLength/4.0
    else
      yp = points(2,1)+filterLength/4.0
    endif
    if ( i==2 .OR. i==3 .OR. i==4 .OR. i==5 ) then
      zp = points(3,1)-filterLength/4.0
    else
      zp = points(3,1)+filterLength/4.0
    endif
    write(*,*) i-1, ': Location: (', xp, ', ', yp, ', ', zp, ')'
    write(*,*) '           (xx =', dataout6(1,i), ', yy =', dataout6(2,i), ', zz =', dataout6(3,i), &
                         ', xy =', dataout6(4,i), ', xz =', dataout6(5,i), ', yz =', dataout6(6,i), ')'
  end do

  write(*,*)
  write(*,*) 'Velocity gradient box filter at 1 particle location'
  CALL getboxfiltervelocitygradient(authkey, dataset, time, filterLength, nLayers, NoTInt, 1, points, dataout9)
  write(*,*) 'Layer 1: (filter length = ', filterLength, ')'
  write(*,*) 1, ': Location: (', points(1,1), ', ', points(2,1), ', ', points(3,1), ')'
  write(*,*) '           (duxdx =', dataout9(1,1), ', duxdy =', dataout9(2,1), ', duxdz =', dataout9(3,1), &
                       ', duydx =', dataout9(4,1), ', duydy =', dataout9(5,1), ', duydz =', dataout9(6,1), &
                       ', duzdx =', dataout9(7,1), ', duzdy =', dataout9(8,1), ', duzdz =', dataout9(9,1), ')'
  write(*,*) 'Layer 2: (filter length = ', filterLength/2.0, ')'
  do i = 2, 9, 1
    if ( i==2 .OR. i==4 .OR. i==6 .OR. i==8 ) then
      xp = points(1,1)-filterLength/4.0
    else
      xp = points(1,1)+filterLength/4.0
    endif
    if ( i==2 .OR. i==3 .OR. i==6 .OR. i==7 ) then
      yp = points(2,1)-filterLength/4.0
    else
      yp = points(2,1)+filterLength/4.0
    endif
    if ( i==2 .OR. i==3 .OR. i==4 .OR. i==5 ) then
      zp = points(3,1)-filterLength/4.0
    else
      zp = points(3,1)+filterLength/4.0
    endif
    write(*,*) i-1, ': Location: (', xp, ', ', yp, ', ', zp, ')'
    write(*,*) '           (duxdx =', dataout9(1,i), ', duxdy =', dataout9(2,i), ', duxdz =', dataout9(3,i), &
                         ', duydx =', dataout9(4,i), ', duydy =', dataout9(5,i), ', duydz =', dataout9(6,i), &
                         ', duzdx =', dataout9(7,i), ', duzdy =', dataout9(8,i), ', duzdz =', dataout9(9,i), ')'
  end do


  !
  ! Destroy the gSOAP runtime.
  ! No more WebService routines may be called.
  !
  CALL soapdestroy()

end program TurbTest

