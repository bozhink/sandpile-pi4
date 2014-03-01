! file: q-moments-binned.f90
! author: Bozhin Karaivanov
! date: 31.10.2013
! last modified: 05.11.2013

subroutine q_moments_binned (nbins, bin_bound, prob, tau, nq, q, q_moments)
   implicit none
   integer, intent(in) :: nbins ! number of bins
   double precision, dimension(nbins+1), intent(in) :: bin_bound ! boundaries of the bins
   double precision, dimension(nbins), intent(in) :: prob ! probabilities in every bin
   integer, intent(in) :: nq ! the number of values of the q for q-moments
   double precision, intent(in) :: tau ! value of the critical exponent
   double precision, dimension(nq), intent(in) :: q ! values of q for q-moments
   double precision, dimension(nq), intent(out) :: q_moments ! the resultant q-moments
   integer :: iq, i
   double precision :: factor, exp1, exp0, dnumer, denom
   exp0 = 1.d0 - tau
   do iq = 1, nq
      q_moments(iq) = 0.d0
      exp1 = q(iq) + exp0
      factor = exp0/exp1
      do i = 1, nbins
         dnumer = bin_bound(i+1)**exp1-bin_bound(i)**exp1
         denom  = bin_bound(i+1)**exp0-bin_bound(i)**exp0
         !print "(e10.3,2(1x,f5.2,1x,e10.3),2(1x,e10.3))", prob(iq), exp0, dnumer, exp1, denom, &
         !       bin_bound(i), bin_bound(i+1)
         q_moments(iq) = q_moments(iq) + prob(iq)*dnumer/denom
      end do
      q_moments(iq) = factor*q_moments(iq)
   end do
end subroutine q_moments_binned
