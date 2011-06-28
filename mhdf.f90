program TurbTest
  implicit none

  ! ---- Temporal Interpolation Options ----
  integer, parameter :: NoTInt = 0   ! No temporal interpolation
  integer, parameter :: PCHIPInt = 1 ! Piecewise cubic Hermit interpolation in time

  ! ---- Spatial Interpolation Flags for GetVelocity & GetVelocityAndPressure ----
  integer, parameter :: NoSInt = 0 ! No spatial interpolation
  integer, parameter :: Lag4 = 4   ! 4th order Lagrangian interpolation in space
  integer, parameter :: Lag6 = 6   ! 6th order Lagrangian interpolation in space
  integer, parameter :: Lag8 = 8   ! 8th order Lagrangian interpolation in space

  !
  ! Choose which dataset to use in this query
  !
  character*100 :: dataset = 'mhd1024coarse' // CHAR(0)

  !
  ! Specify your access key here.
  ! If you need one, please visit http://turbulence.pha.jhu.edu/
  ! (We just want to know a bit about our users!)
  !
  character*100 :: authkey = 'edu.jhu.pha.turbulence.testing-201104' // CHAR(0)

  integer, parameter :: timestep = 199
  real :: time = 0.002 * timestep
  real :: startTime = 0.002 * timestep
  real :: endTime = 0.002 * (timestep + 5)
  real :: dt = 0.0004   ! timestep to use for getposition method
  real points(3, 10)    ! input
  real dataout3(3, 10)  ! x,y,z
  real dataout1(10)  ! x,y,z


  ! Declare the return type of the mhdlib functions as integer.
  ! This is required for custom error handling (see the README).
  integer getvelocity, getmagneticfield, getvectorpotential, getpressure
  integer getposition

  ! return code
  integer rc

  ! loop iterator
  integer i

  !
  ! Intialize the gSOAP runtime.
  ! This is required before any WebService routines are called.
  !
  CALL soapinit()

  ! Enable exit on error.  See README for details.
  CALL turblibSetExitOnError(1)

  do i = 1, 10, 1
    points(1, i) = 0.56 * i
    points(2, i) = 0.15 * i
    points(3, i) = 0.60 * i 
  end do

  write(*,*) 'Advancing 10 particles through time'
  rc = getposition(authkey, dataset, startTime, endTime, dt, Lag6, 10, points, dataout3)
  do i = 1, 10, 1 
    write(*,*) i, ': (', dataout3(1,i), ', ', dataout3(2,i), ', ', dataout3(3,i), ')'
  end do

  write(*,*) 'Velocity at 10 particle locations'
  rc = getvelocity(authkey, dataset,  time, Lag6, NoTInt, 10, points, dataout3)
  do i = 1, 10, 1 
    write(*,*) i, ': (', dataout3(1,i), ', ', dataout3(2,i), ', ', dataout3(3,i), ')'
  end do

  write(*,*) 'Magnetic field at 10 particle locations'
  rc = getmagneticfield(authkey, dataset,  time, Lag6, NoTInt, 10, points, dataout3)
  do i = 1, 10, 1 
    write(*,*) i, ': (', dataout3(1,i), ', ', dataout3(2,i), ', ', dataout3(3,i), ')'
  end do

  write(*,*) 'Vector potential at 10 particle locations'
  rc = getvectorpotential(authkey, dataset,  time, Lag6, NoTInt, 10, points, dataout3)
  do i = 1, 10, 1 
    write(*,*) i, ': (', dataout3(1,i), ', ', dataout3(2,i), ', ', dataout3(3,i), ')'
  end do

  write(*,*) 'Pressure at 10 particle locations'
  rc = getpressure(authkey, dataset,  time, Lag6, NoTInt, 10, points, dataout1)
  do i = 1, 10, 1 
    write(*,*) i, ': (', dataout1(i), ')'
  end do

  !
  ! Destroy the gSOAP runtime.
  ! No more WebService routines may be called.
  !
  CALL soapdestroy()

end program TurbTest

