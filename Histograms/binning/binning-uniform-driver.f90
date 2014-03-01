! file: binning-uniform-driver.f90
! author: Bozhin Karaivanov
! date created: 15.02.2014
! last modified: 15.02.2014

program binning_uniform_driver

   use binning_uniform_module
   
   implicit none
   
   integer, parameter :: N = 10
   
   double precision, dimension(0:N) :: bin_bound
   
   double precision :: x_min, x_max
   
   integer :: i
   
   x_min = 0.d0
   x_max = 1.d1
   
   call binning_uniform (x_min, x_max, N, bin_bound)
   
   do i = 1, N
      print*, i, bin_bound(i-1), bin_bound(i)
   end do

end program binning_uniform_driver

