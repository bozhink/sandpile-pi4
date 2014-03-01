! file: binning-uniform-module.f90
! author: Bozhin Karaivanov
! date created: 15.02.2014
! last modified: 24.02.2014

module binning_uniform_module

   implicit none

   double precision :: bin_width

   public binning_uniform, bin_width

contains

   subroutine binning_uniform (x_min, x_max, N, bin_boundaries)
      ! This subroutine calculates the boundaries of the bins
      ! and populates the bins array
      implicit none
      double precision, intent(in) :: x_min ! minimal value of the variable x
      double precision, intent(in) :: x_max ! maximal value of the variable x
      integer, intent(in) :: N ! number of bins
      double precision, dimension(0:N), intent(out) :: bin_boundaries ! Array of bin's boundaries
      double precision :: h
      integer :: i

      h = (x_max - x_min) / dfloat(N)

      bin_width = h

      do i = 0, N
         bin_boundaries(i) = x_min + dfloat(i) * h
      enddo
   end subroutine binning_uniform

end module binning_uniform_module
