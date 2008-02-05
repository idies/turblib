program TurbTest
  implicit none

  ! ---- Spatial Interpolation Flags ----
  integer, parameter :: NoSpatialInterpolation = 0
  integer, parameter :: Lagrangian4thOrder = 4
  integer, parameter :: Lagrangian6thOrder = 6
  integer, parameter :: Lagrangian8thOrder = 8

  ! ---- Temporal Interpolation Options ----
  integer, parameter :: NoTemporalInterpolation = 0
  integer, parameter :: PCHIPInterpolation = 1
  
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
  character*100 :: authkey = 'jhu.edu.pha.turbulence.testing-200711' // CHAR(0)

  real :: time = 0.06
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
  CALL getvelocity(authkey, dataset,  time, Lagrangian6thOrder , NoTemporalInterpolation, 10, points, dataout3)
  do i = 1, 10, 1 
    write(*,*) i, ': (', dataout3(1,i), ', ', dataout3(2,i), ',', dataout3(3,i), ')'
  end do

  write(*, *)
  write(*, *) 'Velocity and pressure at 10 particle locations'
  CALL getvelocityandpressure(authkey, dataset,  time, Lagrangian6thOrder, NoTemporalInterpolation, 10, points, dataout4)
  do i = 1, 10, 1 
    write(*,*) i, ': (', dataout4(1,i), ', ', dataout4(2,i), ',', dataout4(3,i), ',', dataout4(4,i), ')'
  end do

  write(*, *)
  write(*, *) 'Pressure hessian at 10 particle locations'
  CALL getpressurehessian(authkey, dataset,  time, Lagrangian6thOrder, NoTemporalInterpolation, 10, points, dataout6)
  do i = 1, 10, 1 
    write(*,*) i, ': (d2pdxdx=', dataout6(1,i), ', d2pdxdy=', dataout6(2,i), &
       ', d2pdxdz=', dataout6(3,i), ', d2pdydy=', dataout6(4,i),  &
       ', d2pdydz=', dataout6(5,i), ', d2pdzdz', dataout6(6,i), ')'
  end do

  write(*, *)
  write(*, *) 'Velocity gradient at 10 particle locations'
  CALL getvelocitygradient(authkey, dataset,  time, Lagrangian6thOrder, NoTemporalInterpolation, 10, points, dataout9)
  do i = 1, 10, 1 
    write(*,*) i, ': (duxdx=', dataout9(1,i), ', duxdy=', dataout9(2,i), &
       ', duxdz=', dataout9(3,i), ', duydx=', dataout9(4,i),  &
       ', duydy=', dataout9(5,i), ', duydz=', dataout9(6,i),  &
       ', duzdx=', dataout9(7,i), ', duzdy=', dataout9(8,i),  &
       ', duzdz=', dataout9(9,i), ')'
  end do

  !
  ! Destroy the gSOAP runtime.
  ! No more WebService routines may be called.
  !
  CALL soapdestroy()

end program TurbTest

