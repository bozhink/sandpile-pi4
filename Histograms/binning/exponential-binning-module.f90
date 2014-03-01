! file: exponential-binning-module.f90
! author: Bozhin Karaivanov
! date: 28.02.2014
! last modified: 01.03.2014

module exponential_binning_module

   implicit none

   public exponential_binning

contains

   subroutine exponential_binning (x_min, x_max, N, bin_boundaries)
      ! This subroutine calculates the boundaries of the bins
      ! and populates the bins' array
      implicit none
      integer, intent(in) :: x_min ! minimal value of the variable to be binned
      integer, intent(in) :: x_max ! maximal value of the variable to be binned
      integer, intent(in) :: N ! Number of bins
      integer, dimension(0:N), intent(out) :: bin_boundaries ! Array of bin's boundaries
      real :: z_min, z_max, h, z
      real, dimension(0:N) :: z_bins
      integer :: i, n1
      
      if (N.lt.1) stop 'You must have at least one bin'
      if (x_min.ge.x_max) stop 'Minimal value must be less than the maximal'
      if (x_min.lt.0.or.x_max.lt.0) stop 'Maximal / minimal value of the binned variable must be non-negative'
      if (x_max.le.1) stop 'Maximal value of the binned variable must be greater than 1'
      
      bin_boundaries(0) = x_min
      
      z_min = float(x_min+1)
      if (x_min.lt.1) z_min = 1.0
      z_max = float(x_max)
      n1 = int(2.0 * log(z_max-z_min) + 1.0)
      if (N.gt.n1) then
         write(unit=0,fmt="('You want too many bins. Maximal reasonable value is ',i5)") n1
         stop 'Too many bins'
      endif
      if (N.eq.1) then
         bin_boundaries(1) = x_max
         return
      endif
      
      z_min = log(z_min)
      z_max = log(z_max)
      h = (z_max - z_min) / float(N-1)
      if (h.lt.0.) stop 'Negative stride?'
      z_bins(1) = z_min
      do i = 2, N
         z_bins(i) = z_bins(i-1) + h
      enddo
      
      ! Calculate bin boundaries
      do i = 1, N
         bin_boundaries(i) = nint(exp(z_bins(i)))
      enddo

   end subroutine exponential_binning

end module exponential_binning_module

