! file: fib-driver.f90
! author: Bozhin Karaivanov
! date created: 15.02.2014
! last modified: 24.02.2014

program fib_driver
   use fibonacci_numbers_module
   use fibonacci_binning_module
   implicit none

   ! If we want N bins, then bin's bounds must be N+1: from 0 to N.
   ! To obtain this bin's bounds we need the fist N+2 Fibonacci numbers.
   ! Every bin is in the interval [bin_bounds(i-1), bin_bounds(i)-1], where
   ! i is the number of the current bin.
   ! The centroide of this bin is a real number equa to 1/2 * (bin_bounds(i-1) + (bin_bounds(i)-1)).
   ! The length of the bin is exactly the i-th Fibonacci number F(i), i.e. this is
   ! the normalization factor for the histogram.

   integer, parameter :: N = 70
   integer*8, dimension(N+2) :: F
   integer*8, dimension(0:N) :: bin_bounds
   double precision, dimension(N) :: bin_centroide
   integer :: i

   call fibonacci_numbers_double (N+2, F)

   do i = 1, N+2
      print "('Fibonacci number #',i2,' = ',i15)",i, F(i)
   end do

   print *
   print *
   print *

   call fibonacci_binning_double (0, N, F, bin_bounds)
   do i = 1, N
      bin_centroide(i) = 0.5d0*(dfloat(bin_bounds(i-1)) + dfloat(bin_bounds(i)-1))
   end do

   print "('Bin boundaries:')"
   do i = 1, N
      print *, i, bin_bounds(i-1), bin_bounds(i)-1, bin_centroide(i), bin_bounds(i)-bin_bounds(i-1), F(i)
   end do

end program fib_driver
