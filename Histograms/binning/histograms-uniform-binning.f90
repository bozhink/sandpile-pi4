! file: histograms-uniform-binning.f90
! author: Bozhin Karaivanov
! date: 12.02.2014
! last modified: 01.03.2014

program histograms_uniform_binning
! This program performs binning over yet calculated histograms
! of a sandpile model.
! The required format of the input data file is
! <value of avalanche parameter> <number of occurences of this value> <normalized histogram>

   use binning_uniform_module
   use histograms_binning_messages_module

   implicit none

   integer :: max_value, length
   character (len=256) :: ifile, ofile, arg
   integer :: ierr
   integer, allocatable, dimension(:) :: t, nt

   integer, parameter :: nbins = 70 ! Number of bins
   
   double precision, dimension(0:nbins) :: bins ! boundaries of the bins

   integer*8, dimension(nbins) :: hist_bins ! histogram due to the binning

   double precision, dimension(nbins) :: histogram ! normalized histogram
   double precision, dimension(nbins) :: centroide ! centroides of the bins

   integer :: tt, ntt, i, j, tmax
   double precision :: t_min, t_max
   
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
   
   open (unit=10,file=ifile,status='old',action='read',iostat=ierr)
   if (ierr.ne.0) call io_error (0, ierr)
   open (unit=11,file=ofile,status='new',action='write',iostat=ierr)
   if (ierr.ne.0) call io_error (1, ierr)

   allocate (t(length), nt(length))
   ! Initialization
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
   
   t_min = dfloat(t(1))
   tmax = t(1)
   do i = 2, length
      if (t(i).gt.tmax) tmax = t(i)
   end do
   t_max = dfloat(tmax)
 
   call binning_uniform (t_min, t_max, nbins, bins)

   do j = 1, nbins
      hist_bins(j) = 0
   enddo

   do i = 1, length
      do j = 1, nbins
         if ( (bins(j-1).le.t(i)) .and. (t(i).lt.bins(j)) ) then
            hist_bins(j) = hist_bins(j) + nt(i)
            exit
         endif
      enddo
   enddo

   do i = 1, nbins
      histogram(i) = dfloat(hist_bins(i)) / bin_width
      centroide(i) = 0.5d0*(bins(i-1) + bins(i))
   end do

   do j = 1, nbins
      if (hist_bins(j).le.0) cycle
      write (unit=11, fmt="(f12.3,1x,e18.12)", iostat=ierr) centroide(j), histogram(j)
      if (ierr.ne.0) call io_error(3,ierr)
   end do

   deallocate (t, nt)
   close (unit=11)
   close (unit=10)

end program histograms_uniform_binning

