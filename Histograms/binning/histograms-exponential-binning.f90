! file: histograms-exponential-binning.f90
! author: Bozhin Karaivanov
! date: 01.03.2014
! last modified: 01.03.2014

program binning_over_calculated_histograms
! This program performs binning over yet calculated histograms
! of a sandpile model.
! The required format of the input data file is
! <value of avalanche parameter> <number of occurences of this value> <normalized histogram>

   use exponential_binning_module
   use histograms_binning_messages_module

   implicit none

   integer :: max_value, length
   character (len=256) :: ifile, ofile, arg
   integer :: ierr
   integer, allocatable, dimension(:) :: t, nt

   integer :: nbins ! Number of bins
   
   integer, allocatable, dimension(:) :: bins ! boundaries of the bins

   integer, allocatable, dimension(:) :: hist_bins ! histogram due to the binning

   double precision, allocatable, dimension(:) :: histogram ! normalized histogram
   double precision, allocatable, dimension(:) :: centroide ! centroides of the bins

   integer :: tt, ntt, t_min, t_max
   !double precision :: pt
   integer :: i, j

   ! Read command line arguments
   do i = 1, iargc()
      call getarg(i,arg)
      print "('Input argument #',i1,': ',a)", i, trim(arg)
   enddo

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
      if (length.gt.max_value+1) call invalid_argument(-1)
      call getarg(3,ifile)
      call getarg(4,ofile)
   end if
   
   ! Open input and output files
   open (unit=10,file=ifile,status='old',action='read',iostat=ierr)
   if (ierr.ne.0) call io_error (0, ierr)
   open (unit=11,file=ofile,status='new',action='write',iostat=ierr)
   if (ierr.ne.0) call io_error (1, ierr)

   allocate (t(1:length), nt(1:length))
   do i = 1, length
      t(i) = 0
      nt(i) = 0
   end do
   ! Read input data
   do i = 1, length
      read (unit=10, fmt=*, iostat=ierr, end=10) tt, ntt
      if (ierr.ne.0) call io_error(2,ierr)
      t(i) = tt
      nt(i) = ntt
   end do
 10 continue

   !t_min = minval(t)
   !t_max = maxval(t) ! + 1
   t_min = 0
   t_max = max_value + 1
   ! Calculate optimal number of bins
   nbins = int(2.0 * log(float(t_max-t_min)) + 1.0)
   
   ! Calculate bin's boundaries
   allocate ( bins(0:nbins) )
   call exponential_binning (t_min, t_max, nbins, bins)
   
   !do i = 0, nbins
   !   print*, i, bins(i)
   !enddo

   allocate ( hist_bins(1:nbins) )
   do j = 1, nbins
      hist_bins(j) = 0
   enddo
   ! Perform binning
   do i = 1, length
      do j = 1, nbins
         if ( (bins(j-1).le.t(i)) .and. (t(i).lt.bins(j)) ) then
            hist_bins(j) = hist_bins(j) + nt(i)
            exit
         endif
      enddo
   enddo
   
   deallocate (t, nt)

   allocate ( histogram(1:nbins), centroide(1:nbins) )
   do i = 1, nbins
      histogram(i) = dfloat(hist_bins(i)) / dfloat(bins(i)-bins(i-1))
      centroide(i) = 0.5d0*(dfloat(bins(i-1)) + dfloat(bins(i)-1))
      !print*, i, bins(i-1), bins(i), centroide(i), histogram(i)
   end do
   
   deallocate (bins)

   ! Write histogram to output file
   do j = 1, nbins
      if (hist_bins(j).le.0) cycle
      write (unit=11, fmt="(f12.3,1x,e18.12)", iostat=ierr) centroide(j), histogram(j)
      if (ierr.ne.0) call io_error(3,ierr)
   end do

   deallocate (hist_bins, histogram, centroide)
   
   close (unit=11)
   close (unit=10)

end program binning_over_calculated_histograms

