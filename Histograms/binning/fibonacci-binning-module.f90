! file: fibonacci-binning-module.f90
! author: Bozhin Karaivanov
! date created: 15.02.2014
! last modified: 15.02.2014

module fibonacci_binning_module

   use fibonacci_numbers_module

   implicit none

   public fibonacci_binning, fibonacci_binning_double

contains

   subroutine fibonacci_binning (s, N, F, bin_boundaries)
      ! This subroutine calculates the boundaries of the bins
      ! and populates the bins array
      implicit none
      integer, intent(in) :: s ! starting offset
      integer, intent(in) :: N ! Number of bins
      integer, dimension(N+2), intent(in) :: F ! first N+2 Fibonacci numbers
      integer, dimension(0:N), intent(out) :: bin_boundaries ! Array of bin's boundaries
      integer :: i
      do i = 0, N
         bin_boundaries(i) = s + F(i+2) - 1
      enddo
   end subroutine fibonacci_binning

   subroutine fibonacci_binning_double (s, N, F, bin_boundaries)
      ! This subroutine calculates the boundaries of the bins
      ! and populates the bins array
      implicit none
      integer, intent(in) :: s ! starting offset
      integer, intent(in) :: N ! Number of bins
      integer*8, dimension(N+2), intent(in) :: F ! first N+2 Fibonacci numbers
      integer*8, dimension(0:N), intent(out) :: bin_boundaries ! Array of bins boundaries
      integer :: i
      do i = 0, N
         bin_boundaries(i) = s + F(i+2) - 1
      enddo
   end subroutine fibonacci_binning_double

end module fibonacci_binning_module

