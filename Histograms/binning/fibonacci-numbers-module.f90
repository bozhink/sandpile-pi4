! file: fibonacci-numbers-module.f90
! author: Bozhin Karaivanov
! date created: 15.02.2014
! last modified: 15.02.2014

module fibonacci_numbers_module
   implicit none
   public fibonacci_numbers, fibonacci_numbers_double

contains

   subroutine fibonacci_numbers (N, F)
      ! This subroutine calculates the first N Fibonacci numbers and
      ! put them in the output array F
      implicit none
      integer, intent(in) :: N ! number of required Fibonacci numbers
      integer, dimension(N), intent(out) :: F
      integer :: i
      if (N.le.0) stop 'fibonacci_numbers'
      F(1) = 1
      if (N.eq.1) return
      F(2) = 1
      if (N.eq.2) return
      do i = 3, N
         F(i) = F(i-1) + F(i-2)
      enddo
   end subroutine fibonacci_numbers
   
   subroutine fibonacci_numbers_double (N, F)
      ! This subroutine calculates the first N Fibonacci numbers and
      ! put them in the output array F
      implicit none
      integer, intent(in) :: N ! number of required Fibonacci numbers
      integer*8, dimension(N), intent(out) :: F
      integer :: i
      if (N.le.0) stop 'fibonacci_numbers'
      F(1) = 1
      if (N.eq.1) return
      F(2) = 1
      if (N.eq.2) return
      do i = 3, N
         F(i) = F(i-1) + F(i-2)
      enddo
   end subroutine fibonacci_numbers_double

end module fibonacci_numbers_module
