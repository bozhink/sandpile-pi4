! file: exponential-binning-driver.f90
! author: Bozhin Karaivanov
! date: 01.03.2014
! last modified: 01.03.2014

program exponential_binning_driver
   use exponential_binning_module
   implicit none
   
   integer :: N
   integer :: x_min, x_max
   integer, allocatable, dimension(:) :: bins
   
   integer :: i
   
   x_min = 0
   x_max = 10001
   
   N = int(2.0 * log(float(x_max-x_min)) + 1.0)
   
   print*,'Number of bins = ', N
   
   allocate ( bins(0:N) )
   
   call exponential_binning(x_min, x_max, N, bins)
   
   print*
   print*,'Bin boundaries:'
   do i = 0, N
      print "(i2,1x,i6)", i, bins(i)
   enddo
   
   deallocate (bins)

end program exponential_binning_driver


