! file: histograms-moments.f90
! author: Bozhin Karaivanov
! date: 31.10.2013
! last modified: 5.11.2013

program binning_over_calculated_histograms
! This program performs binning over yet calculated histograms
! of a sandpile model.
! The required format of the input data file is
! <value of avalanche parameter> <number of occurences of this value> <normalized histogram>
   implicit none
   integer :: max_value, length
   character (len=256) :: ifile, ofile, arg
   integer :: ierr
   integer, allocatable, dimension(:) :: t, nt
   integer, parameter :: nbins = 70 ! Number of bins
   real, dimension(nbins+1) :: bins ! boundaries of the bins
   integer, dimension(nbins) :: hist_bins ! histogram due to the binning
   double precision, dimension(nbins) :: histogram ! normalized histogram
   double precision :: normal_factor ! normalization factor
   integer, parameter :: nq = 30 ! number of different values of q
   double precision, dimension(nq) :: q ! values of the exponent q
   double precision, parameter :: q_min = 0.1d0, q_max=5.d0
   double precision, dimension(nq) :: q_moments ! values of the q-moments
   double precision, parameter :: tau = 1.75d0
   integer :: tt, ntt
   double precision :: pt, h
   integer :: i, j
   character(len=28) :: frmt
   
   if (iargc().lt.3) then
      call print_help
   elseif (iargc().eq.3) then
      call getarg(1,arg)
      read (unit=arg,fmt=*,iostat=ierr) max_value
      if (ierr.ne.0) call invalid_argument(ierr)
      length = max_value
      call getarg(2,ifile)
      call getarg(3,ofile)
   else
      call getarg(1,arg)
      read (unit=arg,fmt=*,iostat=ierr) max_value
      if (ierr.ne.0) call invalid_argument(ierr)
      call getarg(2,arg)
      read (unit=arg,fmt=*,iostat=ierr) length
      if (ierr.ne.0) call invalid_argument(ierr)
      if (length.gt.max_value) call invalid_argument(-1)
      call getarg(3,ifile)
      call getarg(4,ofile)
   end if
   
   open (unit=10, file=ifile, status='old', action='read', iostat=ierr)
   if (ierr.ne.0) call io_error (0, ierr)
   open (unit=11, file=ofile, status='unknown', action='write', position='append', iostat=ierr)
   if (ierr.ne.0) call io_error (1, ierr)

   allocate (t(length), nt(length))
   ! Initialization
   do i = 1, length
      t(i) = 0
      nt(i) = 0
   end do

   ! Read input data
   do i = 1, length
      read (unit=10, fmt=*, iostat=ierr, end=10) tt, ntt, pt
      if (ierr.ne.0) call io_error(2,ierr)
      t(i) = tt
      nt(i) = ntt
   end do
 10 continue

   call binning (1, max_value, nbins, bins)
   do j = 1, nbins
      hist_bins(j) = 0
   enddo
   do i = 1, length
      do j = 1, nbins
         if (bins(j).le.t(i).and.bins(j+1).gt.t(i)) then
            hist_bins(j) = hist_bins(j) + nt(i)
            exit
         endif
      enddo
   enddo
   
   normal_factor = dfloat(sum(hist_bins))
   histogram = dfloat(hist_bins) / normal_factor
   
   ! Calculation of the q-moments
   h = (q_max - q_min) / dfloat(nq)
   q(1) = q_min
   do i = 2, nq
      q(i) = q(i-1) + h
   enddo
   call q_moments_binned (nbins, dble(bins), histogram, tau, nq, q, q_moments)

   ! Write output data
   write (unit=frmt, fmt="('(i6,1x,',i3,'(f5.2,1x,e18.12))')") nq
   write (unit=11, fmt=frmt) max_value, (q(i),q_moments(i),i=1,nq)
   deallocate (t, nt)
   close (unit=11)
   close (unit=10)
end program binning_over_calculated_histograms

subroutine print_help
   write (unit=0, fmt="('Usage: histograms <maxval> [<length> ]<infile> <outfile>')")
   write (unit=0, fmt="('where:')")
   write (unit=0, fmt="('    maxval = maximal value of the avalanche''s parameter')")
   write (unit=0, fmt="('    length = number of different values of the avalanche''s parameter')")
   write (unit=0, fmt="('             lenght must be less or equal to maxval')")
   write (unit=0, fmt="('    infile = name of the input file')")
   write (unit=0, fmt="('   outfile = name of the output file')")
   stop
end subroutine print_help

subroutine invalid_argument (ierr)
   write (unit=0, fmt="('ERROR: invalid argument!',/,'Error message #',i5)") ierr
   stop 'Invalid argument'
end subroutine invalid_argument

subroutine io_error (icode, ierr)
   if (icode.eq.0) then
      write (unit=0,fmt="('ERROR: Open input file! Error message #',i5)") ierr
      stop 'open input file'
   elseif (icode.eq.1) then
      write (unit=0,fmt="('ERROR: Open output file! Error message #',i5)") ierr
      stop 'open output file'
   elseif (icode.eq.2) then
      write (unit=0,fmt="('ERROR: Read input data! Error message #',i5)") ierr
      stop 'read data'
   elseif (icode.eq.3) then
      write (unit=0,fmt="('ERROR: Write data to output file! Error message #',i5)") ierr
      stop 'write data'
   endif
end subroutine io_error

subroutine binning (minimal_value, maximal_value, nbins, bins)
   ! This subroutine calculates the boundaries of the bins
   ! and populates the bins array
   implicit none
   integer, intent(in) :: minimal_value ! Minimal value of the parameter to be binned
   integer, intent(in) :: maximal_value ! Maximal value of the parameter to be binned
   integer, intent(in) :: nbins ! Number of bins
   real, dimension(nbins+1), intent(out) :: bins ! Array of bins boundaries
   real :: h ! the bin width
   integer :: i
   
   h = float(maximal_value-minimal_value)/float(nbins)
   bins(1) = float(minimal_value)
   do i = 2, nbins
      bins(i) = bins(i-1) + h
   enddo
   bins(nbins+1) = float(maximal_value)
end subroutine binning

subroutine binning1 (minimal_value, maximal_value, nbins, bins)
   ! This subroutine calculates the boundaries of the bins
   ! and populates the bins array
   implicit none
   integer, intent(in) :: minimal_value ! Minimal value of the parameter to be binned
   integer, intent(in) :: maximal_value ! Maximal value of the parameter to be binned
   integer, intent(in) :: nbins ! Number of bins
   real, dimension(nbins+1), intent(out) :: bins ! Array of bins boundaries
   real :: h ! the bin width
   real :: bisection
   integer :: i
   
   h = float(maximal_value-minimal_value)/float(nbins)
   bins(1) = 1.0
   bins(2) = bisection(0.001, 1.0, float(maximal_value), maximal_value, nbins)
   do i = 3, nbins
      bins(i) = bins(i-1)*bins(2)
   enddo
end subroutine binning1
function f(x, L, nbins) result (fx)
  implicit none
  real, intent(in) :: x
  integer, intent(in) :: L, nbins
  real :: fx
  fx = x*(1.0-x)*L - (1.0-x**(1+nbins))
end function f
recursive function bisection(h, xmin, xmax, L, nbins) result(root)
  implicit none
  real, intent(in) :: h, xmin, xmax
  integer, intent(in) :: L, nbins
  real :: root, a, b, c, fa, fb, fc, f
  a = xmin
  b = xmax
  c = (a+b)/2.0
  if (abs(a-b).le.h) then
    root = c
  else
    fa = f(a, L, nbins)
    fb = f(b, L, nbins)
    fc = f(c, L, nbins)
    if (fa*fc.lt.0.0) then
       root = bisection(h, a, c, L, nbins)
    else if (fc*fb.lt.0.0) then
       root = bisection(h, c, b, L, nbins)
    else
       stop 'No root'
    endif
  endif
end function bisection

