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
  character*100 :: authkey = 'jhu.edu.pha.turbulence.testing-200802' // CHAR(0)

  real :: time = 0.08
  real points(3, 10)
  real dataout3(3, 10)  ! x,y,z
  real dataout4(4, 10)  ! x,y,z,p
  real dataout6(6, 10)  ! x,y,z
  real dataout9(9, 10)  ! x,y,z
  integer i,rc

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

  write(*, *) 'Velocity at 10 particle locations'
  CALL getvelocity(authkey, dataset,  time, Lag6, NoTInt, 10, points, dataout3)
  do i = 1, 10, 1 
    write(*,*) i, ': (', dataout3(1,i), ', ', dataout3(2,i), ',', dataout3(3,i), ')'
  end do

  write(*, *)
  write(*, *) 'Velocity and pressure at 10 particle locations'
  CALL getvelocityandpressure(authkey, dataset,  time, Lag6, NoTInt, 10, points, dataout4)
  do i = 1, 10, 1 
    write(*,*) i, ': (', dataout4(1,i), ', ', dataout4(2,i), ',', dataout4(3,i), ',', dataout4(4,i), ')'
  end do

  !write(*, *)
  !write(*, *) 'Pressure hessian at 10 particle locations'
  !CALL getpressurehessian(authkey, dataset,  time, Lag6, NoTInt, 10, points, dataout6)
  !do i = 1, 10, 1 
  !  write(*,*) i, ': (d2pdxdx=', dataout6(1,i), ', d2pdxdy=', dataout6(2,i), &
  !     ', d2pdxdz=', dataout6(3,i), ', d2pdydy=', dataout6(4,i),  &
  !     ', d2pdydz=', dataout6(5,i), ', d2pdzdz', dataout6(6,i), ')'
  !end do

  write(*, *)
  write(*, *) 'Velocity gradient at 10 particle locations'
  CALL getvelocitygradient(authkey, dataset,  time, FD4Lag4, NoTInt, 10, points, dataout9)
  do i = 1, 10, 1 
    write(*,*) i, ': (duxdx=', dataout9(1,i), ', duxdy=', dataout9(2,i), &
       ', duxdz=', dataout9(3,i), ', duydx=', dataout9(4,i),  &
       ', duydy=', dataout9(5,i), ', duydz=', dataout9(6,i),  &
       ', duzdx=', dataout9(7,i), ', duzdy=', dataout9(8,i),  &
       ', duzdz=', dataout9(9,i), ')'
  end do

  write(*, *)
  write(*, *) 'Pressure gradient at 10 particle locations'
  CALL getpressuregradient(authkey, dataset,  time, FD4Lag4, NoTInt, 10, points, dataout3)
  do i = 1, 10, 1 
    write(*,*) i, ': (dpdx=', dataout3(1,i), ', dpdy=', dataout3(2,i), ', dpdz=', dataout3(3,i), ')'
  end do

  !
  ! Destroy the gSOAP runtime.
  ! No more WebService routines may be called.
  !
  CALL soapdestroy()

end program TurbTest

